#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    int client_sock_fd;
    struct sockaddr_in client_address;
    char buf[BUFSIZ];
    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_address.sin_port = htons(8080);

    // create client socket
    if ((client_sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    // connect to server
    if (connect(client_sock_fd, (struct sockaddr *) &client_address, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        return 1;
    }

    printf("connected to server\n");
    recv(client_sock_fd, buf,BUFSIZ,0);
    printf("%s", buf);

    // send and receive message
    while (1) {
        printf("Enter string to send:");
        scanf("%s", buf);
        if (!strcmp(buf, "quit"))
            break;
        send(client_sock_fd, buf, strlen(buf), 0);
        recv(client_sock_fd, buf, BUFSIZ, 0);

        printf("received: %s\n", buf);
    }
    close(client_sock_fd);

    return 0;
}
