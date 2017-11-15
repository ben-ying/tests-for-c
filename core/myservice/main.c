#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
//#include <zconf.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in client_address;
    char buf[BUFSIZ];
    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_address.sin_port = htons(6666);

    // create client socket
    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // connect to server
    if (connect(client_socket, (struct sockaddr *) &client_address, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("connected to server\n");
    recv(client_socket, buf, BUFSIZ, 0);
    printf("%s", buf);

    // send and receive message
    while (1) {
        printf("Enter string to send:");
        scanf("%s", buf);
        if (!strcmp(buf, "quit"))
            break;
        send(client_socket, buf, strlen(buf), 0);
        recv(client_socket, buf, BUFSIZ, 0);

        printf("received %s from server\n", buf);
    }
    // close(client_socket);

    return 0;
}


