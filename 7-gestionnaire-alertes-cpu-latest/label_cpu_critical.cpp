#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./label_cpu_critical <pod_name> <namespace>\n";
        return 1;
    }

    std::string pod = argv[1];
    std::string ns = argv[2];

    std::string cmd = "kubectl label pod " + pod + " --namespace=" + ns + " fogsla=ok status=panne --overwrite";
    std::cout << "🔥 CPU en panne. Command: " << cmd << std::endl;
    std::system(cmd.c_str());
    return 0;
}
