/*
  LEDEyebrow.cpp
*/

#include "Arduino.h"
#include "LEDEyebrow.h"
#include "PinDefinitionsAndConstants.h"
#include "LED.h"

// creates an instance of a LEDEyebrow
LEDEyebrow::LEDEyebrow(const int pins[], int numberPins, int analog_max_level, int analog_min_level)
{
	_numberPins = numberPins;
	// create some LED instances from the general LED class
	for (int i = 0; i < _numberPins; ++i)
	{
		// int p = pins[i];
		// LED led(p);
		_leds[i] = LED(pins[i], analog_max_level, analog_min_level);
	}
}

void LEDEyebrow::On()
{
	for(int i=0; i < _numberPins;i++) {
		_leds[i].On();
	}
}

void LEDEyebrow::Off()
{
	for(int i=0; i < _numberPins;i++) {
		_leds[i].Off();
	}
}

void LEDEyebrow::ToMinBrightness()
{
	for(int i=0; i < _numberPins;i++) {
		_leds[i].ToMinBrightness();
	}
}

void LEDEyebrow::setMaxBrightnessValue(int val)
{
	for(int i=0; i < _numberPins;i++) {
		_leds[i].setMaxBrightnessValue(val);
	}
}

void LEDEyebrow::setMinBrightnessValue(int val)
{
	for(int i=0; i < _numberPins;i++) {
		_leds[i].setMinBrightnessValue(val);
	}
}

void LEDEyebrow::setBrightPercent(int perc)
{
	for(int i=0; i < _numberPins;i++) {
		_leds[i].setBrightPercent(perc);
	}
}

void LEDEyebrow::resetEffects()
{
	// reset pulse related stuff
	_pulseStarted = false;

	// reset runway stuff
	_runStarted = false;
}

// returns the number of pulses so far
//  _pulseState (int) can be -1 (fading), 0 (off) or 1 (brightening)
int LEDEyebrow::pulseAll(unsigned long currentTime_us, unsigned long on_glow_ms, unsigned long on_fade_ms, unsigned long off_spacing_ms, unsigned long initial_delay_ms)
{
	// initialize the start time when first called
	if (!_pulseStarted)
	{
		// reset to defaults
		_pulseNumFlashes = 0;
		_pulseStarted = true;

		_pulseTime = currentTime_us;

		_pulseAlreadyDelayed = false;

		// LED pulses on at the beginning
		_pulseState = 1;
	}

	// return straight away if there is a delay
	if (!_pulseAlreadyDelayed) {
		if ((currentTime_us - _pulseTime) <= initial_delay_ms) {
			return _pulseNumFlashes;
		} else {
			// delay has been passed
			_pulseAlreadyDelayed = true;
			_pulseTime = currentTime_us;
		}
	}

	unsigned long next_event_time;
	int next_state;
	switch(_pulseState) {
		case -1: // fading
			next_event_time = 1000*on_fade_ms;
			next_state = 0;
			break;

		case 0: // off
			next_event_time = 1000*off_spacing_ms;
			next_state = 1;
			break;

		case 1: // brightening
			next_event_time = 1000*on_glow_ms;
			next_state = -1;
			break;
	}

	// whenever we hit next event time, change state
	if ((currentTime_us - _pulseTime) >= next_event_time) {
		// change state
		_pulseState = next_state;
		// reset start timer
		_pulseTime = currentTime_us;

		// bump number of flashes every off toggle
		if (_pulseState == 0) {
			_pulseNumFlashes++;
		}
	}

	// if fading or glowing
	if (_pulseState != 0) {
		// set brightness based on time and current state
		unsigned long perc = (100 * (currentTime_us - _pulseTime)) / next_event_time;
		// if fading swap around percentage
		if (_pulseState == -1)
		{
			perc = 100-perc;
		}
		// perc = 50*(sin(perc/4) + 1);
		setBrightPercent((int)perc);
	} else {
		setBrightPercent(0);
	}
	return _pulseNumFlashes;
}


void LEDEyebrow::runWayAll(unsigned long currentTime_us, unsigned long total_on_duration_ms, unsigned long delay_between_runs_ms)
{
	if (!_runStarted)
	{
		// reset to defaults
		_runStarted = true;
		_runwayTime = currentTime_us;

		_runAlreadyDelayed = false;
	}

	// delay if we need to between runs
	if (!_runAlreadyDelayed) {
		if ((currentTime_us - _runwayTime) <= 1000*delay_between_runs_ms) {
			return;
		} else {
			// delay has been passed
			_runAlreadyDelayed = true;
			_runwayTime = currentTime_us;
		}
	}

	unsigned long on_duration_us = 1000 * (total_on_duration_ms / _numberPins);
	for (int i = 0; i < _numberPins; ++i)
	{
		// when to turn on next
		unsigned long next_turn_on_time = i * on_duration_us;

		// when to turn off next
		unsigned long next_turn_off_time = next_turn_on_time + on_duration_us;

		if ((currentTime_us - _runwayTime) >= next_turn_off_time) {
			_leds[i].Off(); // maybe use MinBrightness() instead ?
		} else if ((currentTime_us - _runwayTime) >= next_turn_on_time) {
			_leds[i].On();
		}
	}

	// when all leds have triggered
	if ((currentTime_us - _runwayTime) >= 1000 * total_on_duration_ms)
	{
		 // start a new run
		_runStarted = false;
	}
}

void LEDEyebrow::setStateBasedOnPotValue(int val)
{
	bool state;
	for(int i=0; i < _numberPins;i++) {
		state = val > (POT_THRESHOLDS[i]) ? true : false;
		if (state) {
			_leds[i].On();
		} else {
			_leds[i].Off();
		}
	}
}
