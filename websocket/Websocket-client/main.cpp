#include <assert.h>
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#include <uWS/uWS.h>

int main() {
    uWS::Hub h;

    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        ws->send(message, length, opCode);
    });

    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        res->end("test", 1);
    });

    if (h.listen(3000)) {
        h.run();
    }
//    using easywsclient::WebSocket;
//    WebSocket::pointer ws = WebSocket::from_url("ws://echo.websocket.org");
//    assert(ws);
//    while (true) {
//        ws->poll();
//        ws->send("hello");
////        ws->dispatch("11");
//    }
//    delete ws; // alternatively, use unique_ptr<> if you have C++11
//    return 0;
}