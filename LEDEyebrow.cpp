/*
  LEDEyebrow.cpp
*/

#include "Arduino.h"
#include "LEDEyebrow.h"
#include "PinDefinitionsAndConstants.h"
#include "LED.h"

// creates an instance of a LEDEyebrow
LEDEyebrow::LEDEyebrow(const int pins[3], int analog_max_level, int analog_min_level)
{
	// create some LED instances from the general LED class
	for (int i = 0; i < 3; ++i)
	{
		// int p = pins[i];
		// LED led(p);
		_leds[i] = LED(pins[i], analog_max_level, analog_min_level);
	}
}

void LEDEyebrow::setAllOn()
{
	for(int i=0; i < 3;i++) {
		_leds[i].setOn();
	}
}

void LEDEyebrow::setAllOff()
{
	for(int i=0; i < 3;i++) {
		_leds[i].setOff();
	}
}

void LEDEyebrow::resetEffects()
{
	// reset LEDs
	for (int i = 0; i < 3; ++i)
	{
		_leds[i].resetEffects();
	}
}

// returns the number of pulses so far
int LEDEyebrow::pulseAll(unsigned long currentTime_us, unsigned long on_glow_ms, unsigned long on_fade_ms, unsigned long off_spacing_ms, unsigned long initial_delay_ms)
{
	int numPulsesSoFar;
	for (int i = 0; i < 3; ++i)
	{
		numPulsesSoFar = _leds[i].pulse(currentTime_us, on_glow_ms, on_fade_ms, off_spacing_ms, initial_delay_ms);
	}
	return numPulsesSoFar;
}

void LEDEyebrow::setStateBasedOnPotValue(int val)
{
	bool state;
	for(int i=0; i < 3;i++) {
		state = val > (POT_THRESHOLDS[i]) ? true : false;
		if (state) {
			_leds[i].setOn();
		} else {
			_leds[i].setOff();
		}
	}
}
