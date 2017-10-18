//
// Created by ben on 17-10-18.
//

#include <iostream>
#include "AudioHandler.hpp"

AudioHandler::AudioHandler() = default;

AudioHandler::~AudioHandler() = default;

void AudioHandler::start() {
    std::cout << "Audio start" << std::endl;
}

void AudioHandler::pause() {
    std::cout << "Audio pause" << std::endl;
}

void AudioHandler::preview() {
    std::cout << "Audio preview" << std::endl;
}

void AudioHandler::next() {
    std::cout << "Audio next" << std::endl;
}
