#include <iostream>
#include <cstdio>
#include <memory>
#include <string>
#include <array>
#include <thread>
#include <chrono>
#include <sstream>
#include <vector>
#include <map> // N'oublie pas cet include

// Fonction pour exécuter une commande shell et récupérer la sortie
std::string runCommand(const std::string &cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}

// Fonction pour retirer les espaces en début et fin de chaîne
static inline std::string trim(const std::string &s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start))
    {
        start++;
    }
    auto end = s.end();
    do
    {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

// Exécute une commande shell et récupère sa sortie
std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

struct Deployment
{
    std::string namespace_;
    std::string name;
};

// Récupère tous les Deployments avec label fogsla=ok
std::vector<Deployment> getDeployments()
{
    std::vector<Deployment> deployments;
    std::string cmd = "kubectl get deployments --all-namespaces -l fogsla=ok -o custom-columns=NS:.metadata.namespace,NAME:.metadata.name --no-headers";
    std::string output = exec(cmd.c_str());
    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line))
    {
        std::istringstream ls(line);
        Deployment d;
        ls >> d.namespace_ >> d.name;
        if (!d.namespace_.empty() && !d.name.empty())
        {
            deployments.push_back(d);
        }
    }
    return deployments;
}

// Patch un Deployment avec les ressources CPU et mémoire demandées (patch 'add')
void patchDeployment(const Deployment &d, const std::string &cpuLimit, const std::string &memLimit, const std::string &patchZone)
{
    std::string patchJson = R"([{"op":"add","path":"/spec/template/spec/containers/0/resources","value":{"limits":{"cpu":")" + cpuLimit +
                            R"(","memory":")" + memLimit + R"("}}}])";

    std::string cmd = "kubectl patch deployment " + d.name + " -n " + d.namespace_ + " --type=json -p '" + patchJson + "'";

    std::cout << "Patch deployment " << d.namespace_ << "/" << d.name
              << " avec cpu=" << cpuLimit << " mem=" << memLimit << std::endl;

    std::string output = exec(cmd.c_str());

    if (output.find("error") != std::string::npos || output.find("Error") != std::string::npos)
    {
        std::cout << "Patch peut avoir échoué ou non nécessaire : " << output << std::endl;
    }
    
    // Label patch (ahouter ou modifier)
    std::string labelCmd = "kubectl label deployment " + d.name + " -n " + d.namespace_ +
                           " patch=" + patchZone + " --overwrite";
    std::string labelOutput = exec(labelCmd.c_str());
    std::cout << "🏷️  Label patch result: " << labelOutput << std::endl;
}

// Fonction fictive (à remplacer par ta propre logique)
float getClusterCpuLoad()
{
    // Récup somme limites CPU en m
    std::string cpuLimitsCmd = R"(
kubectl get pods --all-namespaces -o jsonpath="{range .items[*].spec.containers[*]}{.resources.limits.cpu}{'\n'}{end}" | sed 's/m//g' | awk '
{
  if ($1 ~ /^[0-9]+$/) {
    total += $1
  } else if ($1 ~ /^[0-9]+\.[0-9]+$/) {
    total += $1 * 1000
  }
}
END { print total }')";
    std::string cpuLimitsStr = runCommand(cpuLimitsCmd);
    cpuLimitsStr = trim(cpuLimitsStr);

    // Récup somme allocatable CPU en m
    std::string cpuAllocCmd = R"(
kubectl get nodes -o jsonpath="{.items[*].status.allocatable.cpu}" | sed 's/m/ m/g' | tr ' ' '\n' | awk '
{
  if ($1 ~ /^[0-9]+$/) {
    total += $1 * 1000
  } else {
    gsub(/m/, "", $1)
    total += $1
  }
}
END { print total }')";
    std::string cpuAllocStr = runCommand(cpuAllocCmd);
    cpuAllocStr = trim(cpuAllocStr);

    try
    {
        double limitVal = std::stod(cpuLimitsStr);
        double allocVal = std::stod(cpuAllocStr);
        if (allocVal == 0)
            return 0.0;
        return (limitVal / allocVal) * 100.0;
    }
    catch (...)
    {
        std::cerr << "Erreur de conversion CPU load\n";
        return -1.0;
    }
}

// Récupère la charge CPU = somme limites CPU / somme allocatable CPU * 100
double getCpuLoadPercent()
{
    // Récup somme limites CPU en m
    std::string cpuLimitsCmd = R"(
kubectl get pods --all-namespaces -o jsonpath="{range .items[*].spec.containers[*]}{.resources.limits.cpu}{'\n'}{end}" | sed 's/m//g' | awk '
{
  if ($1 ~ /^[0-9]+$/) {
    total += $1
  } else if ($1 ~ /^[0-9]+\.[0-9]+$/) {
    total += $1 * 1000
  }
}
END { print total }')";
    std::string cpuLimitsStr = runCommand(cpuLimitsCmd);
    cpuLimitsStr = trim(cpuLimitsStr);

    // Récup somme allocatable CPU en m
    std::string cpuAllocCmd = R"(
kubectl get nodes -o jsonpath="{.items[*].status.allocatable.cpu}" | sed 's/m/ m/g' | tr ' ' '\n' | awk '
{
  if ($1 ~ /^[0-9]+$/) {
    total += $1 * 1000
  } else {
    gsub(/m/, "", $1)
    total += $1
  }
}
END { print total }')";
    std::string cpuAllocStr = runCommand(cpuAllocCmd);
    cpuAllocStr = trim(cpuAllocStr);

    try
    {
        double limitVal = std::stod(cpuLimitsStr);
        double allocVal = std::stod(cpuAllocStr);
        if (allocVal == 0)
            return 0.0;
        return (limitVal / allocVal) * 100.0;
    }
    catch (...)
    {
        std::cerr << "Erreur de conversion CPU load\n";
        return -1.0;
    }
}

int main()
{

    // ✅ CPU allocatable minimum (en milliCPU)
    std::string minCpuCmd = R"(
kubectl get nodes -o jsonpath="{.items[*].status.allocatable.cpu}" | \
sed 's/m/ m/g' | tr ' ' '\n' | awk '
{
  if ($1 ~ /^[0-9]+$/) {
    val = $1 * 1000
  } else {
    gsub(/m/, "", $1)
    val = $1
  }
  if (min == "" || val < min) min = val
}
END { print min }')";
    std::string minCpu = runCommand(minCpuCmd);
    std::cout << "🧠 CPU allocatable minimum (m) : " << minCpu;

    // ✅ Mémoire allocatable minimum (en Mi)
    std::string minMemCmd = R"(
kubectl get nodes -o jsonpath="{.items[*].status.allocatable.memory}" | \
tr ' ' '\n' | sed 's/Ki//g' | awk '
{
  val = $1 / 1024
  if (min == "" || val < min) min = val
}
END { print int(min) }')";
    std::string minMem = runCommand(minMemCmd);
    std::cout << " 🧠 Mémoire allocatable minimum (Mi) : " << minMem << std::endl;

    std::map<std::string, std::string> lastPatch; // Pour éviter les patchs répétés inutiles

    while (true)
    {
        float charge = getClusterCpuLoad();
        std::cout << "Charge CPU du cluster : " << charge << "%" << std::endl;

        std::vector<Deployment> deployments = getDeployments();

        for (const auto &d : deployments)
        {
            std::string key = d.namespace_ + "/" + d.name;

            std::string patchZone;
            if (charge >= 0 && charge < 33)
            {
                patchZone = "low";
            }
            else if (charge >= 33 && charge < 66)
            {
                patchZone = "medium";
            }
            else if (charge >= 66)
            {
                patchZone = "high";
            }

            if (lastPatch[key] == patchZone)
            {
                std::cout << "Pas de patch nécessaire pour " << key << " (zone " << patchZone << " inchangée)" << std::endl;
                continue;
            }

            /*if (patchZone == "low") {
                 patchDeployment(d, "100m", "10Mi");
             } else if (patchZone == "medium") {
                 patchDeployment(d, "200m", "200Mi");
             } else if (patchZone == "high") {
                 patchDeployment(d, "300m", "300Mi");
             }*/
            if (!minCpu.empty() && !minMem.empty())
            {
                int baseCpu = std::stoi(minCpu); // en mCPU
                int baseMem = std::stoi(minMem); // en Mi

                // On prend 80% comme valeur de base
                int cpuVal = static_cast<int>(baseCpu * 0.8);
                int memVal = static_cast<int>(baseMem * 0.8);

                if (patchZone == "low")
                {
                    // 100% des 80%
                    patchDeployment(d, std::to_string(cpuVal) + "m", std::to_string(memVal) + "Mi", patchZone);
                }
                else if (patchZone == "medium")
                {
                    // 80% des 80%
                    int newCpu = static_cast<int>(cpuVal * 0.8);
                    int newMem = static_cast<int>(memVal * 0.8);
                    patchDeployment(d, std::to_string(newCpu) + "m", std::to_string(newMem) + "Mi", patchZone);
                }
                else if (patchZone == "high")
                {
                    // 60% des 80%
                    int newCpu = static_cast<int>(cpuVal * 0.6);
                    int newMem = static_cast<int>(memVal * 0.6);
                    patchDeployment(d, std::to_string(newCpu) + "m", std::to_string(newMem) + "Mi", patchZone);
                }
            }

            lastPatch[key] = patchZone;
        }

        std::cout << "Attente 1 minute avant prochaine vérification..." << std::endl;
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}
