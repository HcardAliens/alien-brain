/*
	SoundMouth.h

*/
#ifndef SoundMouth_h
#define SoundMouth_h

#include "Arduino.h"

class SoundMouth
{
	public:
		SoundMouth(const int pins[5]);

		// call reset between play calls
		void reset();

		// plays a sound every period_ms
		// returns how many times it has been played so far
		int play(unsigned long currentTime_us, int sound_index, unsigned long delay_period_ms);
	private:
		int _pins[5];

		bool _pinStates[5];
		void setState(bool state, int optoNumber);

		// used in play
		unsigned long _playStartTime;
		unsigned long _playOptoCouplerTriggerTime;
		int _playNumberOfPlays;
		bool _playStarted;
};

#endif