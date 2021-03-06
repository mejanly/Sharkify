#include "Emote.h"

Emote::Emote(Shark *_shark) {
   shark = _shark;
   
   x = shark->loc.x;
   y = shark->loc.y;
}

// May not need this...probably doing a wrapper class to hold all the emotes 
// so in main you can have the keyboard recognition function just call one
// set function for "emotes"
void Emote::add(const char *_text) {
   time = 1.0;
   text = _text;
}

int Emote::draw() {
   fontEngine->useFont("oswald", 36);   
   // Draws emote if time is left
   if (time > 0) {
      fontEngine->display(glm::vec4(1.0, 0.9, 0.1, time), text, x-(fontEngine->getTextWidth(text)), y-time);
      return 1;
   }
   // Otherwise, signals to destroy
   else {
      return 0;
   }
}

void Emote::update() {
   time -= DECREMENT;
}
