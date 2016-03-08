#ifndef __EMOTE__
#define __EMOTE__

#include "Headers.h"

#define DECREMENT 0.008

using namespace std;

class Emote {
	public:
		Emote(Shark *_shark) {}
		~Emote(){}
		
		char[5] text;
		void add(const char *_text);
		int draw();
		void update();
	private:
	   Shark *shark;
	   float x;
	   float y;
	   float time;
};
#endif
