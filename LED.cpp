/*
  LED.cpp
  A class representing any LED that should (at some point !) have many functions needed elsewhere
*/

#include "Arduino.h"
#include "LED.h"
#include "PinDefinitionsAndConstants.h"

// only provided for array initialization, not useful otherwise
LED::LED() {
}

// creates an instance of a LED
LED::LED(const int pin, const int analog_max, const int analog_min)
{
		_pin = pin;
		pinMode(pin, OUTPUT);

		_analog_max_level = analog_max;
		_analog_min_level = analog_min;

		// turn off when starting
		analogWrite(pin, _analog_min_level);
		_currentLevel = _analog_min_level;

}

// pass in level from 0 to 100
// sets brightness as a percentage of analog high

// see link below on how to avoid float computations
// https://stackoverflow.com/questions/8859589/quick-alternatives-to-floating-point-multiplication-to-calculate-percentages
void LED::setBrightPercent(int perc)
{
	unsigned long level = ((unsigned long)perc * ((unsigned long)_analog_max_level - (unsigned long)_analog_min_level)) / 100 + _analog_min_level;
	if (_currentLevel != level)
	{
		Serial.print("Wrote brightness: ");
		Serial.println((int)level);
		analogWrite(_pin, (int)level);
		_currentLevel = level;
	}
}

void LED::setOn()
{
	if (_currentLevel < _analog_max_level)
	{
		analogWrite(_pin, _analog_max_level);
		_currentLevel = _analog_max_level;
	}
}

void LED::setOff()
{
	if (_currentLevel > _analog_min_level)
	{
		analogWrite(_pin, _analog_min_level);
		_currentLevel = _analog_min_level;
	}
}

void LED::setMaxAnalog(int val)
{
	_analog_max_level = val;
}

void LED::setMinAnalog(int val)
{
	_analog_min_level = val;
}


void LED::resetEffects()
{
	// pulse related effects
	_pulseStarted = false;
}

// returns the number of pulses so far
//  _pulseState (int) can be -1 (fading), 0 (off) or 1 (brightening)
int LED::pulse(unsigned long currentTime_us, unsigned long on_glow_ms, unsigned long on_fade_ms, unsigned long off_spacing_ms, unsigned long initial_delay_ms)
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
		setBrightPercent(0.0);
	}
	return _pulseNumFlashes;
}
