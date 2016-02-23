#ifndef __Sound__
#define __Sound__

#include "Headers.h"
#include <irrKlang.h>
   
using namespace irrklang;

class Song {
public:
    char song_name[50]; // name of the song
    char song_file[50]; // name of the .wav sound file
    char vid_file[50];  // name of the .avi video file
    float bpm;          // # of beats per minute
    int price;          // price of the song in the shop
    bool unlocked;      // whether or not the song can be played
};

class Sound {
public:
    Sound();
    virtual ~Sound();
    
    ISoundEngine* engine;
    
    void initSound();
    void playBackgroundMusic();
    void pauseSong();
    void setVolume(float volume);
    
    void playContactSound();
    void playBuzzerSound();
    void playThwackSound();
    void playSplatSound();
    void playJumpSound();
    void playCorrectSound();
    void playIncorrectSound();
    void playSoftDing();
    void playLoudDing();
    
private:
    ISound* curSound;
};

#endif
