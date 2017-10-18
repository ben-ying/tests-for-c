//
// Created by ben on 17-10-18.
//

#include <iostream>
#include "MediaHandler.hpp"

MediaHandler::MediaHandler() = default;

MediaHandler::~MediaHandler() = default;

void MediaHandler::start() {
    std::cout << "Media start" << std::endl;
}

void MediaHandler::pause() {
    std::cout << "Media pause" << std::endl;
}

void MediaHandler::preview() {
    std::cout << "Media preview" << std::endl;
}

void MediaHandler::next() {
    std::cout << "Media next" << std::endl;
}
