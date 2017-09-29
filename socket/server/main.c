#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <zconf.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int server_socket;
    int client_socket;
    ssize_t len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t sin_size;
    char buf[BUFSIZ];
    // init
    memset(&server_address, 0, sizeof(server_address));
    // IPv4
    server_address.sin_family = AF_INET;
    // Address to accept any incoming messages
    // server_address.sin_addr.s_addr = INADDR_ANY;
    // IP address
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Port
    server_address.sin_port = htons(6666);

    // create server socket
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket failed");
        exit(EXIT_FAILURE);
    }

    // reuse port
    int on = 1;
    if ((setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to server address
    if (bind(server_socket,
             (struct sockaddr *) &server_address, sizeof(struct sockaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    sin_size = sizeof(struct sockaddr_in);

    if ((client_socket = accept(server_socket,
                                (struct sockaddr *) &client_address, &sin_size)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("accept client %s\n", inet_ntoa(client_address.sin_addr));
    char *welcome = "Welcome to my server\n";
    send(client_socket, welcome, strlen(welcome), 0);

    // receive and send message back to client
    while ((len = recv(client_socket, buf, BUFSIZ, 0)) > 0) {
        printf("received %s from client\n", buf);
        if (send(client_socket, buf, (size_t) len, 0) < 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
    }
    close(client_socket);
    close(server_socket);

    return 0;
}