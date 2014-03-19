/*
	LEDEyebrow.h

*/
#ifndef LEDEyebrow_h
#define LEDEyebrow_h

#include "Arduino.h"
#include "LED.h"

class LEDEyebrow
{
	public:
		LEDEyebrow(const int pins[3], int analog_max_level, int analog_min_level);

		// useful shortcuts
		void setAllOff();
		void setAllOn();


		void setStateBasedOnPotValue(int val);

		// **** Effect functions below

		// resets effect variables
		// you MUST call this in between effects
		void resetEffects();

		// returns the number of pulses so far
		int pulseAll(unsigned long currentTime_us, unsigned long on_glow_ms, unsigned long on_fade_ms, unsigned long off_spacing_ms, unsigned long initial_delay_ms);

		LED _leds[3];

};

#endif