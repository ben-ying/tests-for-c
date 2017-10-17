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
#include <string>
#include <tsd/common/types/SmartPtr.hpp>
#include "tsd/sds/android/app/ClientManager.hpp"
#include "tsd/sds/android/app/audio/SyncAudioOut.hpp"

#include <tsd/dataformats/data/visitor.hpp>
#include <tsd/dataformats/data/object.hpp>
namespace data = tsd::dataformats::data;


int main(int argc, char *argv[]) {
    std::cout << "App Starting..." << std::endl;
//    std::string str = "{\"key\": 1}";
//    std::unique_ptr<tsd::dataformats::data::Value> value =
//            tsd::dataformats::json::valueFromJsonString(str);

//    tsd::dataformats::json::Parser p;
//    tsd::dataformats::json::Tokenizer tok(p);
//    tok.handleData(buf);
//
//    tsd::dataformats::data::Value* val = NULL;
//    bool result = p.extractValue(val);
//
//
//    Visitor vis;
//    vis.visit(val);

//    std::cout << "object: " << vis.str() << std::endl;

//    tsd::sds::android::app::ClientManager *cm =
//            new tsd::sds::android::app::ClientManager();
//    tsd::common::types::SmartPtr<tsd::sds::android::app::audio::IAudioFactory> af
//            = cm->getAudioFactory();
//    std::set<std::string> &channelIds = af->getAudioSourceIds();
//    std::cout << "channelIds size: " << channelIds.size() << std::endl;
//    for (auto element : channelIds) {
//        std::cout << "channelId: " << element << std::endl;
//        // sync audio out, that shall be used in the core to write samples
////        tsd::common::types::SmartPtr<tsd::sds::android::core::backend::playback::IAudioOut> audioOut = af.getAudioOut(element);
////        if ("TTS" == element){
////            audioOut->writeSamples(samples, len); //填充要写入的sample
////        }
//    }

    std::shared_ptr<WebSocketTest> webSocketTest(new WebSocketTest());

    webSocketTest->testSimple();
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
//    webSocketTest->testTransfers();

    // Linux-only feature
#ifdef __linux__
    webSocketTest->testReusePort();
#endif

    //testAutobahn();
}
