// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8080
#define BUF_SIZE 32

void send_number(const char *hostname, int number) {
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd;

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "ERROR: No such host: %s\n", hostname);
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(PORT);
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        close(sockfd);
        return;
    }

    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "%d\n", number);
    write(sockfd, buf, strlen(buf));
    close(sockfd);
}

int main() {
    const char *MACHINE1 = "sla-project-machine-1-service";
    const char *MACHINE2 = "sla-project-machine-2-service";

    srand(time(NULL));

    while (1) {
        int rand_number = rand() % 1000;
        printf("Generated number: %d\n", rand_number);

        send_number(MACHINE1, rand_number);
        send_number(MACHINE2, rand_number);

        sleep(10);
    }

    return 0;
}
