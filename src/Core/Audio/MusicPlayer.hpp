//
// Created by 33771 on 07/02/2021.
//

#ifndef LIFPROJET_MUSICPLAYER_HPP
#define LIFPROJET_MUSICPLAYER_HPP

#include "../../Game/Resources/ResourceHolder.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"

#include <SFML/Audio.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <map>
#include <string>

class MusicPlayer : private sf::NonCopyable {
public:
    MusicPlayer();

    void play(Music::ID theme);
    void stop();

    void setPaused(bool pause);
    void setVolume(float volume);

private:
    sf::Music mMusic;
    std::map <Music::ID, std::string> mFileNames;
    float mVolume;
};


#endif //LIFPROJET_MUSICPLAYER_HPP
