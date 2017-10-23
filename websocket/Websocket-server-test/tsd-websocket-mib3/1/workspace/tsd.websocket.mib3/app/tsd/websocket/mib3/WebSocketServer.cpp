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

void send_socket(uWS::WebSocket<uWS::SERVER> *ws) {
    std::ifstream i("../socket.json");
    json j;
    i >> j;
    j["type"] = TYPE_AUDIO_START;
    j["message"] = "server test message";
    ws->send(j.dump().c_str());
}

int main(int argc, char *argv[]) {
    uWS::Hub h;

    h.onConnection([](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        std::cout << "Server Connected" << std::endl;
    });

    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        if (length > 0) {
            struct timeval tp;
            gettimeofday(&tp, NULL);
            long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
            if (opCode == uWS::OpCode::TEXT) {
                std::string message_str(message);
                std::string str = message_str.substr(0, length);
                std::cout << "Server onMessage: "
                          << str << ", length: " << length << ", code: " << opCode << std::endl;
                send_socket(ws);
                json j = json::parse(str);
                int type = j["type"];
            } else if (opCode == uWS::OpCode::BINARY) {
//                if (type == TYPE_AUDIO_STREAM) {
                    std::ofstream file;
////                    long remain_data = j["remaining_data"];
////                    if (remain_data + BUFSIZ == j["file_size"]) {
                        file.open("test.pcm", std::fstream::out);
                        if (!file.fail()) {
                            for (int i = 0; i < length; i++) {
                                file.put(message[i]);
                            }
                            file.close();
                            gettimeofday(&tp, NULL);
                            std::cout << "spent: " << tp.tv_sec * 1000 + tp.tv_usec / 1000 - ms << std::endl;
                            std::cout << "end received: " << ms << std::endl;
                        } else{
                            std::cout << "test" << std::endl;
                        }
//                    }
//                    if (j["remaining_data"] <= 0) {
//                        file.close();
//                    }
//                }
            }
        }
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
