#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <zconf.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int server_sock_fd;
    int client_sock_fd;
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
    server_address.sin_port = htons(8080);

    // create server socket
    if ((server_sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    // reuse port
    int on = 1;
    if ((setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to server address
    if (bind(server_sock_fd,
             (struct sockaddr *) &server_address, sizeof(struct sockaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_sock_fd, 5);

    sin_size = sizeof(struct sockaddr_in);

    if ((client_sock_fd = accept(server_sock_fd,
                                 (struct sockaddr *) &client_address, &sin_size)) < 0) {
        perror("accept");
        return 1;
    }

    printf("accept client %s\n", inet_ntoa(client_address.sin_addr));
    char *welcome = "Welcome to my server\n";
    send(client_sock_fd, welcome, strlen(welcome), 0);

    // receive and send message back to client
    while ((len = recv(client_sock_fd, buf, BUFSIZ, 0)) > 0) {
        printf("received: %s\n", buf);
        if (send(client_sock_fd, buf, len, 0) < 0) {
            perror("write");
            return 1;
        }
    }
    close(client_sock_fd);
    close(server_sock_fd);

    return 0;
}