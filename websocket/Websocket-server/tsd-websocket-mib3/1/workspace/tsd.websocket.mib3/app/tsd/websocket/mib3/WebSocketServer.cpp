// NOTE: This is not part of the library, this file holds examples and tests
#include "tsd/websocket/mib3/WebSocketTest.hpp"
//#include "tsd/websocket/mib3/uWS.h"
#include "tsd/websocket/uws/uWS.h"

#include <thread>
#include <fstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <atomic>

int main(int argc, char *argv[]) {
    uWS::Hub h;

    h.onConnection([](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        std::cout << "Server Connected" << std::endl;
        ws->send("112233");
    });

    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::cout << "Server onMessage: "
                  << *message << ", length: " << length << ", code: " << opCode << std::endl;
    });

    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        res->end();
    });

    h.listen(3000);
    h.connect("ws://localhost:3000", nullptr);
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
