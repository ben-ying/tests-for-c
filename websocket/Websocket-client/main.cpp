#include <assert.h>
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#include <uWS/uWS.h>
#include <iostream>
#include <fstream>
#include "json.hpp"

using nlohmann::json;

enum Type {
    TYPE_MESSAGE, TYPE_FILE, TYPE_REQUEST_FILE
};

void send_socket(uWS::WebSocket<uWS::CLIENT> *client) {
    std::ifstream i("socket.json");
    json j;
    i >> j;
    j["type"] = TYPE_MESSAGE;
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
//        send_socket(client);
        client->send("111122222");
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