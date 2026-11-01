#pragma once
#include <SFML/Audio.hpp>
#include <string>
namespace ds {
class AudioManager {
    sf::Music music_;
    sf::SoundBuffer buttonBuffer_; // Must outlive the sound referencing it.
    sf::Sound button_;
    float musicVolume_=30, effectsVolume_=60;
    bool musicReady_=false, effectsReady_=false;
public:
    void load(const std::string& directory);
    void playButton();
    void setMusicVolume(float volume);
    void setEffectsVolume(float volume);
    float musicVolume() const { return musicVolume_; }
    float effectsVolume() const { return effectsVolume_; }
    bool musicReady() const { return musicReady_; }
    bool effectsReady() const { return effectsReady_; }
    bool musicPlaying() const { return music_.getStatus()==sf::SoundSource::Playing; }
};
}
