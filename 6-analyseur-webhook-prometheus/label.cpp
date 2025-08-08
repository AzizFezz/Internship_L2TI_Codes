#include <crow.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <json.hpp>

using json = nlohmann::json;

void label_pod(const std::string& pod, const std::string& ns, const std::vector<std::string>& labels) {
    std::stringstream cmd;
    cmd << "kubectl label pod " << pod << " -n " << ns;

    for (const auto& label : labels) {
        cmd << " " << label << "=true --overwrite";
    }

    std::cout << "[INFO] Labeling pod " << pod << " in ns " << ns << " with labels: ";
    for (const auto& l : labels) std::cout << l << " ";
    std::cout << std::endl;

    int ret = std::system(cmd.str().c_str());
    if (ret != 0) {
        std::cerr << "[ERROR] Failed to label pod " << pod << std::endl;
    }
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods("POST"_method)([](const crow::request& req){
        try {
            auto data = json::parse(req.body);
            if (!data.contains("alerts")) {
                return crow::response(400, "Invalid alert format");
            }

            for (const auto& alert : data["alerts"]) {
                std::string pod = alert["labels"].value("pod", "");
                std::string ns  = alert["labels"].value("namespace", "default");

                std::string cpu_str = alert["annotations"].value("cpu_usage", "0");
                float cpu = std::stof(cpu_str);

                std::vector<std::string> labels;

                if (cpu >= 5.0) {
                    labels = {"R3", "S1"};
                } else if (cpu >= 2.0) {
                    labels = {"R2", "S2"};
                } else {
                    labels = {"R1", "S3"};
                }

                label_pod(pod, ns, labels);
            }

            return crow::response(200, "Alerts processed");

        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Exception: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });

    std::cout << "[INFO] Webhook server listening on 0.0.0.0:8080\n";
    app.port(8080).multithreaded().run();
}
