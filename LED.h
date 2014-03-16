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
		LED(const int pin, const int analog_max); // always use this constructor

		// useful shortcuts
		void setOn();
		void setOff();

		void setBrightPercent(int perc);

		// **** Effect functions below

		// resets all effect variables
		// you MUST call this in between each effect call
		void resetEffects();

		// returns the number of pulses so far
		int pulse(unsigned long currentTime_us, unsigned long on_period_ms, unsigned long off_spacing_ms);

	private:
		int _pin;
		int _analog_max_level;
		bool _currentLevel;

		// used in pulse
		unsigned long _pulseTime;
		int _pulseNumFlashes;
		bool _pulseStarted;
		bool _pulseState;
};

#endif