#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <zconf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/time.h>
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

char* serialization_socket_json(const struct SocketJson *socket_json) {
    JSON_Value *root_value = json_parse_file("../socket.json");
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;
    json_object_set_number(root_object, "type", socket_json->type);
    json_object_set_number(root_object, "size", socket_json->size);
    json_object_dotset_string(root_object, "name", socket_json->name);
    json_object_dotset_string(root_object, "data", socket_json->data);
    serialized_string = json_serialize_to_string_pretty(root_value);
    puts(serialized_string);
//    json_free_serialized_string(serialized_string);
    json_value_free(root_value);

    return serialized_string;
}

int send_file(const int *client_socket) {
    int fd;
    off_t offset;
    off_t remain_data;
    struct stat file_stat;
    ssize_t sent_bytes = 0;
    char cwd[1024];
    char* file_name = "../client_file";

    // Open file
    fd = open(file_name, O_RDONLY);
    if (fd < 0) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            fprintf(stdout, "Current working dir: %s\n", cwd);
        } else {
            perror("getcwd() error");
        }
        fprintf(stderr, "Error opening file --> %s", strerror(errno));
        return -1;
    }

    // Get file stats
    if (fstat(fd, &file_stat) < 0) {
        fprintf(stderr, "Error fstat --> %s", strerror(errno));
        return -1;
    }

    offset = 0;
    remain_data = file_stat.st_size;
    // send file message
    struct SocketJson socket_json;
    socket_json.type = TYPE_FILE;
    socket_json.size = remain_data;
    socket_json.name = file_name;
    socket_json.data = "";
    socket_json.message = "receive file from client";
    char* json_str = serialization_socket_json(&socket_json);

    if (send(*client_socket, json_str, strlen(json_str), 0) < 0) {
        perror("send file failed");
        return -1;
    }

    usleep(100);
//    char buffer[BUFSIZ];
//    if (recv(*client_socket, buffer, BUFSIZ, 0) < 0) {
//        return -1;
//    }
//    if (strcmp(buffer, SUCCESS) < 0) {
//        return -1;
//    }
    // Sending file data
    while ((sent_bytes = sendfile(*client_socket, fd, &offset, BUFSIZ)) > 0 && (remain_data > 0)) {
        remain_data -= sent_bytes;
    }

    return 0;
}

int receive_file(const int *client_socket) {
    FILE *received_file;
    ssize_t len;
    char buffer[BUFSIZ];

    received_file = fopen("../client.apk", "w");
    if (received_file == NULL) {
        fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
        return -1;
    }

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    while ((len = recv(*client_socket, buffer, BUFSIZ, 0)) > 0) {
        fwrite(buffer, sizeof(char), len, received_file);
    }

    gettimeofday(&stop, NULL);
    printf("took %lu\n", (stop.tv_usec - start.tv_usec) / 1000); // 1-7 milliseconds for 3.4M

    fclose(received_file);

    return 0;
}


int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in client_address;
    char buf[BUFSIZ];
    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_address.sin_port = htons(8080);

    // create client socket
    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    // connect to server
    if (connect(client_socket, (struct sockaddr *) &client_address, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        return 1;
    }

    printf("connected to server\n");
    recv(client_socket, buf, BUFSIZ, 0);
    printf("%s", buf);

    // send and receive message
    while (1) {
        memset(buf, 0, BUFSIZ);
        printf("1 for send file\n2 for request file\nOthers for send message\nEnter string to send:\n");
        scanf("%s", buf);
        if (!strcmp(buf, "quit"))
            break;

        if (strcmp(buf, "1") == 0) {
            // send file
            if (send_file(&client_socket) < 0) {
                perror("send file failed");
                return 1;
            }
        } else if (strcmp(buf, "2") == 0) {
            // request file
            struct SocketJson socket_json;
            socket_json.type = TYPE_REQUEST_FILE;
            socket_json.size = 0;
            socket_json.name = "../client.apk";
            socket_json.data = "";
            socket_json.message = "request file";
            char* json_str = serialization_socket_json(&socket_json);
            if (send(client_socket, json_str, strlen(json_str), 0) < 0) {
                perror("request file failed");
                return 1;
            }
            if (receive_file(&client_socket) < 0) {
                perror("receive file failed");
                return 1;
            }
            printf("received file");
        } else {
            // send message and receive message
            memset(buf, 0, BUFSIZ);
            if (send(client_socket, "{}", strlen("{}"), 0) < 0) {
                perror("send message failed");
                return 1;
            }
            recv(client_socket, buf, BUFSIZ, 0);
            printf("received: %s\n", buf);
        }
    }
    close(client_socket);

    return 0;
}
