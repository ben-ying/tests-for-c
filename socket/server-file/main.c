#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <zconf.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

void send_file(int *client_socket, int *server_socket) {
    struct sockaddr_in client_address;
    ssize_t len;
    char buffer[BUFSIZ];
    socklen_t sin_size;
    int fd;
    off_t offset;
    off_t remain_data;
    struct stat file_stat;
    ssize_t sent_bytes = 0;
    char cwd[1024];

    // Open file
    fd = open("../app-debug.apk", O_RDONLY);
    if (fd < 0) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            fprintf(stdout, "Current working dir: %s\n", cwd);
        } else {
            perror("getcwd() error");
        }
        fprintf(stderr, "Error opening file --> %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Get file stats
    if (fstat(fd, &file_stat) < 0) {
        fprintf(stderr, "Error fstat --> %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "File Size: \n%ld bytes\n", file_stat.st_size);

    sin_size = sizeof(struct sockaddr_in);

    if ((*client_socket = accept(*server_socket,
                                (struct sockaddr *) &client_address, &sin_size)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if (*client_socket < 0) {
        fprintf(stderr, "Error on accept --> %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Accept client --> %s\n", inet_ntoa(client_address.sin_addr));
    sprintf(buffer, "%ld", file_stat.st_size);
    // Sending file size
    len = send(*client_socket, buffer, sizeof(buffer), 0);
    if (len < 0) {
        fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Server sent %ld bytes for the size\n", len);

    offset = 0;
    remain_data = file_stat.st_size;
    // Sending file data
    while ((sent_bytes = sendfile(*client_socket, fd, &offset, BUFSIZ)) > 0 && (remain_data > 0)) {
        remain_data -= sent_bytes;
    }

    *server_socket = 0;
    *client_socket = 0;
}

int main(int argc, char *argv[]) {
    int server_socket;
    int client_socket;
    struct sockaddr_in server_address;

    // init
    memset(&server_address, 0, sizeof(server_address));
    // IPv4
    server_address.sin_family = AF_INET;
    // Address to accept any incoming messages
//    server_address.sin_addr.s_addr = INADDR_ANY;
    // IP address
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Port
    server_address.sin_port = htons(8080);

    // create server socket
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
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

    // listen
    if (listen(server_socket, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    send_file(&client_socket, &server_socket);

    close(client_socket);
    close(server_socket);

    return 0;
}