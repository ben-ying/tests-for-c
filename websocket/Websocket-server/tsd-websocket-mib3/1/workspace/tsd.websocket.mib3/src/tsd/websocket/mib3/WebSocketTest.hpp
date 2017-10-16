#include <iostream>


#ifndef WEBSOCKET_SERVER_WEBSOCKETTEST_H
#define WEBSOCKET_SERVER_WEBSOCKETTEST_H


class WebSocketTest {
public:
    WebSocketTest();
    virtual ~WebSocketTest();

public:
    void testSimple();
    int countOccurrences(std::string word, std::string &document);
    void serveBenchmark();
    void measureInternalThroughput(unsigned int payloadLength, int echoes, bool ssl);
    void testStress();
    void testConnections();
    void testListening();
    void testClosing();
    void testBroadcast();
    void testRouting();
    void testReusePort();
    void testTransfers();
    void testSendCallback();
    void testAutoPing();
    void testSmallSends();
    void testMessageBatch();
    void testHTTP();
    void serveEventSource();
    void serveHttp();
    void testReceivePerformance();
    void testThreadSafety();
};


#endif //WEBSOCKET_SERVER_WEBSOCKETTEST_H
