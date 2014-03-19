/*
	LED.h

*/
#ifndef LED_h
#define LED_h

#include "Arduino.h"

class LED
{
	public:
		LED(); // only provided for array initialization, not useful otherwise
		LED(const int pin, const int analog_max, const int analog_min); // always use this constructor

		// useful shortcuts
		void setOn();
		void setOff();

		void setMaxAnalog(int val);
		void setMinAnalog(int val);

		void setBrightPercent(int perc);

		// **** Effect functions below

		// resets all effect variables
		// you MUST call this in between each effect call
		void resetEffects();

		// returns the number of pulses so far
		int pulse(unsigned long currentTime_us, unsigned long on_glow_ms, unsigned long on_fade_ms, unsigned long off_spacing_ms, unsigned long initial_delay_ms);

	private:
		int _pin;
		int _analog_max_level;
		int _analog_min_level;
		bool _currentLevel;

		// used in pulse
		unsigned long _pulseTime;
		bool _pulseAlreadyDelayed;
		int _pulseNumFlashes;
		bool _pulseStarted;
		int _pulseState; // can be -1 (fading), 0 (off) or 1 (brightening)
};

#endif