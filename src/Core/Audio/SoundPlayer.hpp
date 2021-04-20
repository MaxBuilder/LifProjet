//
// Created by 33771 on 06/02/2021.
//

#ifndef LIFPROJET_SOUNDPLAYER_HPP
#define LIFPROJET_SOUNDPLAYER_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "../../Game/Resources/ResourceHolder.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>

/**
 * @class SoundPlayer
 * @brief Object that allows to play sounds
 * @author SFML Game Development
 */
class SoundPlayer : private sf::NonCopyable
{
public:
    /**
     * @brief Default constructor
     */
    SoundPlayer();

    /**
     * @brief Plays a sound effect
     * @param effect Identifier of the sound to play
     */
    void play(Sounds::ID effect);
    /**
     * @brief Loads of sound into the sound player
     * @param sound Identifiers of the sound
     * @param path Path to the file
     */
    void load(Sounds::ID sound, const std::string& path);
    /**
     * @brief set the volume of effects
     * @param volume
     */
    void setVolume(float volume);
    /**
     * @brief Clears the internal vector of played sounds
     */
    void removeStoppedSounds();

private:
    float mVolume;
    SoundBufferHolder			mSoundBuffers;
    std::list<sf::Sound>		mSounds;
};

#endif //LIFPROJET_SOUNDPLAYER_HPP
