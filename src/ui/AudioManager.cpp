#include "dictatorsim/ui/AudioManager.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
namespace ds {
void AudioManager::load(const std::string& directory){
    music_.stop();button_.stop();
    musicReady_=music_.openFromFile(directory+"/background.mp3");
    effectsReady_=buttonBuffer_.loadFromFile(directory+"/button-press.mp3");
    if(musicReady_){music_.setLoop(true);music_.setVolume(musicVolume_);music_.play();}
    else std::cerr<<"Audio: cannot open "<<directory<<"/background.mp3\n";
    if(effectsReady_){button_.setBuffer(buttonBuffer_);button_.setVolume(effectsVolume_);}
    else std::cerr<<"Audio: cannot open "<<directory<<"/button-press.mp3\n";
}
void AudioManager::playButton(){if(effectsReady_ && effectsVolume_>0)button_.play();}
void AudioManager::setMusicVolume(float volume){
    if(!std::isfinite(volume))return;
    musicVolume_=std::round(std::clamp(volume,0.f,100.f));music_.setVolume(musicVolume_);
}
void AudioManager::setEffectsVolume(float volume){
    if(!std::isfinite(volume))return;
    effectsVolume_=std::round(std::clamp(volume,0.f,100.f));button_.setVolume(effectsVolume_);
    if(effectsVolume_==0)button_.stop();
}
}
