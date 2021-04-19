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

/**
 * @class MusicPlayer
 * @brief Object that allows to play music
 * @author SFML Game Development
 */
class MusicPlayer : private sf::NonCopyable {
public:
    /**
     * @brief Default constructor
     */
    MusicPlayer();

    /**
     * @brief Plays music
     * @param theme Identifier of the music to play
     */
    void play(Music::ID theme);
    /**
     * @brief Stops the music
     */
    void stop();

    /**
     * @brief If music is playing, pause the music
     * @param pause Flag
     */
    void setPaused(bool pause);
    /**
     * @brief Sets the volume of the music to a certain value
     * @param volume Volume to set the music to
     */
    void setVolume(float volume);

private:
    sf::Music mMusic;
    Music::ID mPlaying;
    std::map <Music::ID, std::string> mFileNames;
    float mVolume;
};


#endif //LIFPROJET_MUSICPLAYER_HPP
