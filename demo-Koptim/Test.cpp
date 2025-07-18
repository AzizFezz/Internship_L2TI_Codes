#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include <csignal>
#include <cmath>

class CPUConsumer {
private:
    std::atomic<bool> running{true};
    std::vector<std::thread> workers;
    int numCPUs;

public:
    CPUConsumer(int cpus) : numCPUs(cpus) {}

    void start() {
        std::cout << "Démarrage de " << numCPUs << " threads pour consommer " << numCPUs << " CPU(s)..." << std::endl;
       
        // Créer les threads de travail
        for (int i = 0; i < numCPUs; ++i) {
            workers.emplace_back([this, i]() {
                this->cpuIntensiveTask(i);
            });
        }
       
        std::cout << "Threads démarrés. Appuyez sur Ctrl+C pour arrêter." << std::endl;
    }

    void stop() {
        running = false;
        std::cout << "\nArrêt des threads..." << std::endl;
       
        // Attendre que tous les threads se terminent
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
       
        std::cout << "Tous les threads sont arrêtés." << std::endl;
    }

private:
    void cpuIntensiveTask(int threadId) {
        std::cout << "Thread " << threadId << " démarré sur CPU" << std::endl;
       
        // Variables pour le calcul intensif
        volatile double result = 0.0;
        long long counter = 0;
       
        auto startTime = std::chrono::high_resolution_clock::now();
       
        while (running) {
            // Calculs intensifs pour consommer du CPU
            for (int i = 0; i < 100000 && running; ++i) {
                result += std::sin(counter) * std::cos(counter);
                result += std::sqrt(counter % 1000 + 1);
                counter++;
            }
           
            // Afficher les statistiques toutes les 5 secondes
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
           
            if (duration.count() % 5 == 0 && duration.count() > 0) {
                std::cout << "Thread " << threadId << " - Temps: " << duration.count()
                         << "s, Opérations: " << counter << std::endl;
                startTime = currentTime; // Reset pour éviter le spam
            }
        }
       
        std::cout << "Thread " << threadId << " terminé après " << counter << " opérations." << std::endl;
    }
};

// Variables globales pour la gestion des signaux
CPUConsumer* globalConsumer = nullptr;

void signalHandler(int signal) {
    if (globalConsumer) {
        globalConsumer->stop();
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    int numCPUs = 4; // Valeur par défaut
   
    // Lire le paramètre de ligne de commande
    if (argc > 1) {
        numCPUs = std::atoi(argv[1]);
        if (numCPUs <= 0) {
            std::cerr << "Erreur: Le nombre de CPU doit être un entier positif." << std::endl;
            return 1;
        }
    }
   
    // Vérifier que le nombre de CPU demandé est raisonnable
    int maxCPUs = std::thread::hardware_concurrency();
    if (numCPUs > maxCPUs) {
        std::cout << "Attention: " << numCPUs << " CPU(s) demandé(s), mais seulement "
                  << maxCPUs << " CPU(s) disponible(s)." << std::endl;
    }
   
    std::cout << "=== Consommateur de CPU ===" << std::endl;
    std::cout << "Nombre de CPU à consommer: " << numCPUs << std::endl;
    std::cout << "CPU disponibles sur le système: " << maxCPUs << std::endl;
   
    // Créer l'instance du consommateur
    CPUConsumer consumer(numCPUs);
    globalConsumer = &consumer;
   
    // Configurer la gestion des signaux
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
   
    // Démarrer la consommation
    consumer.start();
   
    // Attendre indéfiniment (jusqu'à interruption)
    try {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (...) {
        consumer.stop();
    }
   
    return 0;
}