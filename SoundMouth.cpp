/*
  SoundMouth.cpp
*/

#include "SoundMouth.h"

SoundMouth::SoundMouth(const int pins[5])
{
	// we can't assign directly because pins is a const, unlike _pins
	for (int i = 0; i < 5; ++i)
	{
		_pins[i] = pins[i];
		pinMode(pins[i], OUTPUT);

		// start at high by default
		digitalWrite(pins[i], HIGH);
		_pinStates[i] = true;
	}
}


int SoundMouth::play(unsigned long currentTime_us, int sound_index, unsigned long delay_period_ms)
{
	// initialize the start time when first called
	if (!_playStarted)
	{
		_playStartTime = currentTime_us;
		_playNumberOfPlays = 0;
		_playStarted = true;

		_playOptoCouplerTriggerTime = currentTime_us;
	}

	unsigned long next_event_time = 1000*delay_period_ms;
	// whenever we hit next event time
	if ((currentTime_us - _playStartTime) >= next_event_time) {
		// increment play count
		_playNumberOfPlays++;
		_playStartTime = currentTime_us;

		_playOptoCouplerTriggerTime = currentTime_us;
	}

	// make pin LOW for 80 ms (too small and the soundboard doesn't detect the trigger)
	if ((currentTime_us - _playOptoCouplerTriggerTime) < (unsigned long)80*1000)
	{
		setState(false, sound_index);
	} else { // the rest of the time, keep it high
		setState(true, sound_index);
	}

	return _playNumberOfPlays;
}

void SoundMouth::setState(bool state, int optoNumber)
{
	// only call digitalWrite when state is different than current one
	if (state != _pinStates[optoNumber])
	{
		if (state)
		{
			digitalWrite(_pins[optoNumber], HIGH);
		} else {
			digitalWrite(_pins[optoNumber], LOW);
		}
		_pinStates[optoNumber] = state;
	}
}


void SoundMouth::reset()
{
	_playStarted = false;
}