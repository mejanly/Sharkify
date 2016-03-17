#include "Sound.h"

Sound::Sound() {
   curSound = NULL;
}

Sound::~Sound() {
   engine->drop();
}

void Sound::initSound() {
   engine = createIrrKlangDevice();

   if (engine == NULL) {
      exit(-1);
   }
}

void Sound::playBackgroundMusic() {
   if (curSound != NULL) {
      curSound->drop();
      curSound = NULL;
   }
   engine->stopAllSounds();
   
   // play some sound stream, looped
   engine->play2D("sounds/41Moonsetter.wav", true);
}

void Sound::pauseSong() {
    engine->stopAllSounds();
}
void Sound::setVolume(float volume) {
    engine->setSoundVolume(volume);
}

void Sound::playJumpSound() {
    engine->play2D("sounds/jump.wav", false);
}
void Sound::playContactSound() {
    engine->play2D("sounds/bell.wav", false);
}
void Sound::playBuzzerSound() {
   engine->play2D("sounds/buzz.wav", false);
}
void Sound::playThwackSound() {
   engine->play2D("sounds/thud.wav", false);
}
void Sound::playSplatSound() {
   engine->play2D("sounds/splat.wav", false);
}
void Sound::playCorrectSound() {
    engine->play2D("sounds/correct.wav", false);
}
void Sound::playIncorrectSound() {
    engine->play2D("sounds/incorrect.wav", false);    
}
void Sound::playSoftDing() {
    engine->play2D("sounds/ding_soft.wav", false);
}
void Sound::playLoudDing() {
    engine->play2D("sounds/ding_high.wav", false);
}
