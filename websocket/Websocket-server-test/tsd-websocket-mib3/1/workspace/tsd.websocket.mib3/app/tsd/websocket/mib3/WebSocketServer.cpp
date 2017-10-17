// NOTE: This is not part of the library, this file holds examples and tests
#include "tsd/websocket/mib3/WebSocketTest.hpp"
#include "tsd/websocket/mib3/json.hpp"
//#include "tsd/websocket/mib3/uWS.h"
#include "tsd/websocket/uws/uWS.h"

#include <thread>
#include <fstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <atomic>
using nlohmann::json;

enum Type {
    TYPE_MESSAGE, TYPE_FILE, TYPE_REQUEST_FILE
};

void send_socket(uWS::WebSocket<uWS::SERVER> *ws) {
    std::ifstream i("../socket.json");
    json j;
    i >> j;
    j["type"] = TYPE_MESSAGE;
    j["file_size"] = 0;
    j["name"] = "";
    j["data"] = "";
    j["message"] = "server test message";
    ws->send(j.dump().c_str());
}

int main(int argc, char *argv[]) {
    uWS::Hub h;

    h.onConnection([](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        std::cout << "Server Connected" << std::endl;
    });

    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::string message_str(message);
        std::cout << "Server onMessage: "
                  << message_str.substr(0, length) << ", length: " << length << ", code: " << opCode << std::endl;
        send_socket(ws);
    });

    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        res->end();
    });

    h.onDisconnection([](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        std::cout << "CLIENT CLOSE: " << code << std::endl;
    });

    h.onError([](int port) {
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

    h.listen(3000);
    h.connect("ws://127.0.0.1:3000", nullptr);
//    h.connect("ws://192.168.1.10:3000", nullptr);
    h.run();

    std::shared_ptr<WebSocketTest> webSocketTest(new WebSocketTest());
    //serveEventSource();
    //serveHttp();
    //serveBenchmark();

//#ifdef UWS_THREADSAFE
//    testThreadSafety();
//#endif

    // These will run on Travis OS X
//    std::cout << "---------testReceivePerformance-------" << std::endl;
//    testReceivePerformance();
//    std::cout << "---------testStress-------" << std::endl;
//    testStress();
//    std::cout << "---------testHTTP-------" << std::endl;
//    testHTTP();
//    std::cout << "---------testSmallSends-------" << std::endl;
//    testSmallSends();
//    std::cout << "---------testSendCallback-------" << std::endl;
//    testSendCallback();
//    std::cout << "---------testRouting-------" << std::endl;
//    testRouting();
//    std::cout << "---------testClosing-------" << std::endl;
//    testClosing();
//    std::cout << "---------testListening-------" << std::endl;
//    testListening();
//    std::cout << "---------testBroadcast-------" << std::endl;
//    testBroadcast();
//    std::cout << "---------testMessageBatch-------" << std::endl;
//    testMessageBatch();
//    std::cout << "---------testAutoPing-------" << std::endl;
//    testAutoPing();
//    std::cout << "---------testConnections-------" << std::endl;
//    testConnections();
//    std::cout << "---------testTransfers-------" << std::endl;
    webSocketTest->testTransfers();

    // Linux-only feature
#ifdef __linux__
    webSocketTest->testReusePort();
#endif

    //testAutobahn();
}
