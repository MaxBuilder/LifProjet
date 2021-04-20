//
// Created by 33771 on 07/02/2021.
//

#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer()
: mMusic()
, mFileNames()
, mVolume(100.f)
, mPlaying(Music::None)
{
    mFileNames[Music::MainTheme] = "data/Audio/menuTheme.ogg";
    mFileNames[Music::BattleTheme] = "data/Audio/battleTheme.ogg";
}

void MusicPlayer::play(Music::ID theme) {
    if(mPlaying == theme)
        return;

    std::string filename = mFileNames[theme];

    if(!mMusic.openFromFile(filename))
        throw std::runtime_error("Music " + filename + " could not be opened.");

    mMusic.setVolume(mVolume);
    mMusic.setLoop(true);
    mMusic.play();
    mPlaying = theme;
}

void MusicPlayer::stop() {
    mMusic.stop();
}

void MusicPlayer::setPaused(bool pause) {
    pause ? mMusic.pause() : mMusic.play();
}

void MusicPlayer::setVolume(float volume) {
    mMusic.setVolume(volume);
    mVolume = volume;
}
