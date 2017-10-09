#include <assert.h>
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#include <uWS/uWS.h>
#include <iostream>

int main() {
    uWS::Hub h;
    std::mutex m;
    uWS::WebSocket<uWS::CLIENT> *client;

    h.onConnection([&client, &m](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        client = ws;
        std::cout << "Client2 Connected" << std::endl;
        client->send("22");
    });

    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::cout << "Client2 onMessage: "
                  << *message << ", length: " << length << ", code: " << opCode << std::endl;
    });

    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        res->end("test2", 1);
    });

    h.listen(3000);
    h.connect("ws://localhost:3000");
    h.run();
}