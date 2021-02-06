//
// Created by 33771 on 06/02/2021.
//

#ifndef LIFPROJET_SOUNDPLAYER_HPP
#define LIFPROJET_SOUNDPLAYER_HPP

#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>

class SoundPlayer : private sf::NonCopyable
{
public:
    SoundPlayer();

    void play(Sounds::ID effect);
    void load(Sounds::ID sound, std::string path);
    void removeStoppedSounds();

private:
    SoundBufferHolder			mSoundBuffers;
    std::list<sf::Sound>		mSounds;
};

#endif //LIFPROJET_SOUNDPLAYER_HPP
