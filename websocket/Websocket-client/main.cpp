#include <assert.h>
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#include <uWS/uWS.h>
#include <iostream>
#include <fstream>
#include "json.hpp"

using nlohmann::json;

enum Type {
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
    std::ifstream i("socket.json");
    json j;
    i >> j;
    j["type"] = type;
    j["file_size"] = 0;
    j["name"] = "";
    j["data"] = "";
    j["message"] = "client test message";
    const char *str = j.dump().c_str();
    client->send(str);
}

int main() {
    uWS::Hub h;
    std::mutex m;
    uWS::WebSocket<uWS::CLIENT> *client;

    h.onConnection([&client, &m](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        client = ws;
        std::cout << "Client Connected" << std::endl;
        while (1) {
            int i;
            std::cout << "input type: ";
            std::cin >> i;
            switch (i) {
                case 1:
                    send_socket(client, TYPE_AUDIO_START);
                    break;
                case 2:
                    send_socket(client, TYPE_AUDIO_PAUSE);
                    break;
                case 3:
                    send_socket(client, TYPE_AUDIO_PREVIEW);
                    break;
                case 4:
                    send_socket(client, TYPE_AUDIO_NEXT);
                    break;
                case 5:
                    send_socket(client, TYPE_MEDIA_START);
                    break;
                case 6:
                    send_socket(client, TYPE_MEDIA_PAUSE);
                    break;
                case 7:
                    send_socket(client, TYPE_MEDIA_PREVIEW);
                    break;
                case 8:
                    send_socket(client, TYPE_MEDIA_NEXT);
                    break;
            }
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
    h.connect("ws://127.0.0.1:3000");
//    h.connect("ws://192.168.1.4:3000");
    h.run();
}