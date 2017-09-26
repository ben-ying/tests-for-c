#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <zconf.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in client_address;
    char buffer[BUFSIZ];

    int file_size;
    FILE *received_file;
    int remain_data = 0;
    ssize_t len;

    // init
    memset(&client_address, 0, sizeof(client_address));
    // IPv4
    client_address.sin_family = AF_INET;
    // IP address
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // port
    client_address.sin_port = htons(8080);

    // create client socket
    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // connect to server
    if (connect(client_socket, (struct sockaddr *) &client_address, sizeof(struct sockaddr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("connected to server\n");
    recv(client_socket, buffer, BUFSIZ, 0);
    printf("%s", buffer);

    file_size = atoi(buffer);
    fprintf(stdout, "\nFile size : %d\n", file_size);
    received_file = fopen("../test.apk", "w");
    if (received_file == NULL) {
        fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    remain_data = file_size;

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    while (((len = recv(client_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0)) {
        fwrite(buffer, sizeof(char), len, received_file);
        remain_data -= len;
    }

    gettimeofday(&stop, NULL);
    printf("took %lu\n", (stop.tv_usec - start.tv_usec) / 1000); // 2-7 milliseconds for 3.4M

    fclose(received_file);

    close(client_socket);

    return 0;
}
