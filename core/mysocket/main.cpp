#include <assert.h>
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
//#include <uWS/uWS.h>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "uWs/uWS.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <zconf.h>
#include <stdlib.h>

using nlohmann::json;

int server_socket;
int client_socket;
bool is_socket_connected;

enum Type {
    TYPE_AUDIO_STREAM = 100,
    TYPE_AUDIO_START = 101,
    TYPE_AUDIO_PAUSE = 102,
    TYPE_AUDIO_PREVIEW = 103,
    TYPE_AUDIO_NEXT = 104,
    TYPE_MEDIA_START = 201,
    TYPE_MEDIA_PAUSE = 202,
    TYPE_MEDIA_PREVIEW = 203,
    TYPE_MEDIA_NEXT = 204,
};

void send_socket(uWS::WebSocket<uWS::CLIENT> *client, const int type) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    std::cout << "start send: " << ms << std::endl;
    std::ifstream i("socket.json");
    json j;
    i >> j;
    j["file_type"] = type;
    j["message"] = "client test message";
    if (type != TYPE_AUDIO_STREAM) {
        const char *str = j.dump().c_str();
        client->send(str);
    } else {
        const char *file_name = "../Websocket-client/audio.pcm";
        j["file_name"] = file_name;
        std::ifstream file(file_name, std::ios::in | std::ios::binary | std::ios::ate);

        if (!file.is_open()) {
            std::cout << "Error: could not open file " << std::endl;
            return;
        }

        long size = file.tellg();
        j["file_size"] = size;
        file.seekg(0, std::ios::beg);
        char *buffer = new char[size];

        file.read(buffer, size);
        client->send(buffer, size, uWS::OpCode::BINARY);
        file.close();
    }

    gettimeofday(&tp, NULL);
    std::cout << "spent: " << tp.tv_sec * 1000 + tp.tv_usec / 1000 - ms << std::endl;
}

void setup_socket(uWS::WebSocket<uWS::CLIENT> *client) {
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
    server_address.sin_port = htons(3001);

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

    is_socket_connected = true;

    printf("accept client %s\n", inet_ntoa(client_address.sin_addr));
    char *welcome = "Welcome to my server\n";
    send(client_socket, welcome, strlen(welcome), 0);

    // receive and send message back to client
    while ((len = recv(client_socket, buf, BUFSIZ, 0)) > 0) {
        printf("received %s from client\n", buf);
        printf("len: %d\n", len);
        client->send(buf, len, uWS::OpCode::TEXT);
        if (send(client_socket, buf, (size_t) len, 0) < 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
    }
    close(client_socket);
    close(server_socket);
}


void setup_websocket() {
    uWS::Hub h;
    std::mutex m;
    uWS::WebSocket<uWS::CLIENT> *client;


    h.onConnection([&client, &m](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        client = ws;
        std::cout << "Client Connected" << std::endl;
        if (!is_socket_connected) {
            setup_socket(ws);
        }
        //send_socket(client, TYPE_AUDIO_STREAM);

//        while (1) {
//            int i;
//            std::cout << "input type: ";
//            std::cin >> i;
//            switch (i) {
//                case 1:
//                    send_socket(client, TYPE_AUDIO_START);
//                    break;
//                case 2:
//                    send_socket(client, TYPE_AUDIO_PAUSE);
//                    break;
//                case 3:
//                    send_socket(client, TYPE_AUDIO_PREVIEW);
//                    break;
//                case 4:
//                    send_socket(client, TYPE_AUDIO_NEXT);
//                    break;
//                case 5:
//                    send_socket(client, TYPE_MEDIA_START);
//                    break;
//                case 6:
//                    send_socket(client, TYPE_MEDIA_PAUSE);
//                    break;
//                case 7:
//                    send_socket(client, TYPE_MEDIA_PREVIEW);
//                    break;
//                case 8:
//                    send_socket(client, TYPE_MEDIA_NEXT);
//                    break;
//            }
//        }
    });

    h.onDisconnection([](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        std::cout << "CLIENT CLOSE: " << code << std::endl;
        close(client_socket);
        close(server_socket);
        is_socket_connected = false;
    });

    h.onError([](int port) {
        close(client_socket);
        close(server_socket);
        is_socket_connected = false;
        switch (port) {
            case 80:
                std::cout << "Server emits error listening to port 80 (permission denied)" << std::endl;
                break;
            case 3000:
                std::cout << "Server emits error listening to port 3000 twice" << std::endl;
                break;
            default:
                std::cout << "FAILURE: port " << port << " should not emit error" << std::endl;
                exit(-1);
        }
    });

    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::string message_str(message);
        std::cout << "Client onMessage: "
                  << message_str.substr(0, length) << ", length: " << length << ", code: " << opCode << std::endl;
    });

    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        res->end("test", 1);
    });

    h.listen(3000);
//    h.connect("ws://127.0.0.1:3000");
//    h.connect("ws://192.168.1.4:3000");
    h.connect("ws://192.168.43.12:3000");
    h.run();
}


int main() {
    setup_websocket();
}

