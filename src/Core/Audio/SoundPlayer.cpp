#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer()
: mSoundBuffers()
, mSounds()
, mVolume(100) {}

void SoundPlayer::load(Sounds::ID sound, const std::string& path) {
    mSoundBuffers.load(sound, path);
}

void SoundPlayer::play(Sounds::ID effect)
{
    mSounds.emplace_back();
    sf::Sound& sound = mSounds.back();

    sound.setVolume(mVolume);
    sound.setBuffer(mSoundBuffers.get(effect));

    sound.play();
}

void SoundPlayer::setVolume(float volume){
    mVolume = volume;
}

void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
    });
}
