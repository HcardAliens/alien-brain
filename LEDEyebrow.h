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
		void On(); // max brightness
		void Off(); // turn off completely
		void ToMinBrightness(); // min brightness

		// change the min and max brightness values for the LEDs
		void setMaxBrightnessValue(int val);
		void setMinBrightnessValue(int val);

		void setBrightPercent(int perc);


		void setStateBasedOnPotValue(int val);

		// **** Effect functions below

		// resets effect variables
		// you MUST call this in between effects
		void resetEffects();

		// returns the number of pulses so far
		int pulseAll(unsigned long currentTime_us, unsigned long on_glow_ms, unsigned long on_fade_ms, unsigned long off_spacing_ms, unsigned long initial_delay_ms);

		// simulates a runway
		void runWayAll(unsigned long currentTime_us, unsigned long total_on_duration_ms, unsigned long delay_between_runs_ms);


	private:
		LED _leds[3];

		// used in pulse
		unsigned long _pulseTime;
		bool _pulseAlreadyDelayed;
		int _pulseNumFlashes;
		bool _pulseStarted;
		int _pulseState; // can be -1 (fading), 0 (off) or 1 (brightening)


		// used in runway
		bool _runStarted;
		unsigned long _runwayTime;
		bool _runAlreadyDelayed;

	private:


};

#endif