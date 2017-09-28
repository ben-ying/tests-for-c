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
#include <sys/time.h>
#include <stdbool.h>
#include "parson.h"

#define SUCCESS "success"

enum Type {
    TYPE_DATA, TYPE_FILE, TYPE_REQUEST_FILE
};

struct SocketJson {
    int type;
    long size;
    char *name;
    char *data;
    char *message;
};

int send_file(int *client_socket, const int *server_socket, const struct SocketJson *socket_json) {
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
    fd = open(socket_json->name, O_RDONLY);
    if (fd < 0) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            fprintf(stdout, "Current working dir: %s\n", cwd);
        } else {
            perror("getcwd() error");
        }
        fprintf(stdout, "open file failed %s", strerror(errno));
        return -1;
    }

    // Get file stats
    if (fstat(fd, &file_stat) < 0) {
        perror("fstat failed");
        return -1;
    }

    fprintf(stdout, "Server sent %ld bytes for the size\n", len);

    offset = 0;
    remain_data = file_stat.st_size;
    // Sending file data
    while ((sent_bytes = sendfile(*client_socket, fd, &offset, BUFSIZ)) > 0 && (remain_data > 0)) {
        remain_data -= sent_bytes;
    }
}

int receive_file(const int *client_socket, const struct SocketJson *socket_json) {
    int file_size;
    FILE *received_file;
    int remain_data = 0;
    ssize_t len;
    char buffer[BUFSIZ];

    printf("connected to server\n");

    file_size = socket_json->size;
    fprintf(stdout, "\nFile size : %d\n", file_size);
    received_file = fopen(socket_json->name, "w");
    if (received_file == NULL) {
        fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
        return -1;
    }
    remain_data = file_size;

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    while (((len = recv(*client_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0)) {
        printf("buffer: %s", buffer);
        fwrite(buffer, sizeof(char), len, received_file);
        remain_data -= len;
    }

    gettimeofday(&stop, NULL);
    printf("took %lu\n", (stop.tv_usec - start.tv_usec) / 1000); // 1-7 milliseconds for 3.4M

    fclose(received_file);

    return 0;
}


int main(int argc, char *argv[]) {
    int server_socket;
    int client_socket;
    ssize_t len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t sin_size;
    char buf[BUFSIZ];
    struct SocketJson socket_json;
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
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
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

    listen(server_socket, 5);

    sin_size = sizeof(struct sockaddr_in);

    if ((client_socket = accept(server_socket,
                                (struct sockaddr *) &client_address, &sin_size)) < 0) {
        perror("accept");
        return 1;
    }

    printf("accept client %s\n", inet_ntoa(client_address.sin_addr));
    char *welcome = "Welcome to my server\n";
    send(client_socket, welcome, strlen(welcome), 0);

    // file
//    int file_size;
//    FILE *received_file;
//    int remain_data = 0;
//    ssize_t len;
//    char buffer[BUFSIZ];
//
//    printf("connected to server\n");
//
//    file_size = socket_json->size;
//    fprintf(stdout, "\nFile size : %d\n", file_size);
//    received_file = fopen(socket_json->name, "w");
//    if (received_file == NULL) {
//        fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
//        return -1;
//    }
//    remain_data = file_size;

    // receive and send message back to client
    int remain_data = 0;
    char buffer[BUFSIZ];
    while (memset(buf, 0, BUFSIZ), (len = recv(client_socket, buf, BUFSIZ, 0)) > 0) {
        if (buf[0] == '{' && buf[len - 1] == '}' || buf[0] == '[' && buf[len - 1] == ']') {
            printf("json str");
            JSON_Value *root_value = json_parse_string(buf);
            JSON_Object *root_object = json_value_get_object(root_value);

            socket_json.type = json_object_get_number(root_object, "type");
            socket_json.size = json_object_get_number(root_object, "size");
            socket_json.name = json_object_get_string(root_object, "name");
            socket_json.data = json_object_get_string(root_object, "data");
            socket_json.message = json_object_get_string(root_object, "message");
            remain_data = socket_json.size;

            if (socket_json.type == TYPE_DATA) {
                if (send(client_socket, buf, strlen(buf), 0) < 0) {
                    perror("send message failed");
                    return 1;
                }
            }
        } else if (remain_data > 0) {
            printf("file str");
            FILE *received_file;
            received_file = fopen(socket_json.name, "w");
            if (received_file == NULL) {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
                return -1;
            }
            fwrite(buffer, sizeof(char), len, received_file);
            remain_data -= len;
            if (remain_data <= 0) {
                fclose(received_file);
            }
        }
//        fwrite(buffer, sizeof(char), len, received_file);
//        printf("received: %s\n", buf);
//        printf("length: %d\n", strlen(buf));
//        JSON_Value *root_value = json_parse_string(buf);
//        JSON_Object *root_object = json_value_get_object(root_value);
//
//        socket_json.type = json_object_get_number(root_object, "type");
//        socket_json.size = json_object_get_number(root_object, "size");
//        socket_json.name = json_object_get_string(root_object, "name");
//        socket_json.data = json_object_get_string(root_object, "data");
//        socket_json.message = json_object_get_string(root_object, "message");
//
//        if (socket_json.type == TYPE_FILE) {
////             start receive file
//            receive_file(&client_socket, &socket_json);
//        } else if (socket_json.type == TYPE_REQUEST_FILE) {
//            // send file
//            send_file(&client_socket, &server_socket, &socket_json);
//        } else {
//            // send back message
//            if (send(client_socket, buf, len, 0) < 0) {
//                perror("send message failed");
//                return 1;
//            }
//        }
    }


    close(client_socket);
    close(server_socket);

    return 0;
}