//
// Created by ben on 17-10-18.
//

#ifndef WEBSOCKET_SERVER_MEDIAHANDLER_H
#define WEBSOCKET_SERVER_MEDIAHANDLER_H


class MediaHandler {
public:
    MediaHandler();

    virtual ~MediaHandler();

    void start();
    void pause();
    void preview();
    void next();
};


#endif //WEBSOCKET_SERVER_MEDIAHANDLER_H
