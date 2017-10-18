//
// Created by ben on 17-10-18.
//

#ifndef WEBSOCKET_SERVER_AUDIOHANDLER_H
#define WEBSOCKET_SERVER_AUDIOHANDLER_H


class AudioHandler {
public:
    AudioHandler();

    virtual ~AudioHandler();

    void start();
    void pause();
    void preview();
    void next();
};


#endif //WEBSOCKET_SERVER_AUDIOHANDLER_H
