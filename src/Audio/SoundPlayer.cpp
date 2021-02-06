#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer()
: mSoundBuffers()
, mSounds() {}

void SoundPlayer::load(Sounds::ID sound, std::string path) {
    mSoundBuffers.load(sound, path);
}

void SoundPlayer::play(Sounds::ID effect)
{
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();

    sound.setBuffer(mSoundBuffers.get(effect));

    sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
    });
}
