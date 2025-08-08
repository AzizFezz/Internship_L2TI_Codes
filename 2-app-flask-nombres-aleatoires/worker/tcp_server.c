#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    FILE *fp;

    printf("Starting TCP server on port %d...\n", PORT);
    fflush(stdout);

    // Créer la socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    // Permettre la réutilisation de l'adresse
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind la socket au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    // Écouter les connexions
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    fflush(stdout);

    // Boucle infinie pour accepter les connexions
    while (1) {
        printf("Waiting for connection...\n");
        fflush(stdout);

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            fflush(stderr);
            continue;
        }

        printf("Connection accepted from %s:%d\n", 
               inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        fflush(stdout);

        // Lire les données
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread > 0) {
            buffer[valread] = '\0';  // Null-terminate
            printf("Received: %s", buffer);
            fflush(stdout);

            // Écrire la donnée reçue dans un fichier partagé
            fp = fopen("/tmp/worker_data.txt", "a");
            if (fp != NULL) {
                fprintf(fp, "%s\n", buffer);
                fclose(fp);
            }
        }

        // Fermer la connexion
        close(new_socket);
        memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    }

    return 0;
}
