/*
  LED.cpp
  A class representing any LED that should (at some point !) have many functions needed elsewhere
*/

#include "Arduino.h"
#include "LED.h"
#include "PinDefinitions+Constants.h"

static const int ANALOG_LED_LOW = 0;

// only provided for array initialization, not useful otherwise
LED::LED() {
}

// creates an instance of a LED
LED::LED(const int pin, const int analog_max)
{
		_pin = pin;
		pinMode(pin, OUTPUT);

		_analog_max_level = analog_max;

		// turn off when starting
		analogWrite(pin, ANALOG_LED_LOW);
		_currentLevel = ANALOG_LED_LOW;

}

// pass in level from 0 to 100
// sets brightness as a percentage of analog high

// see link below on how to avoid float computations
// https://stackoverflow.com/questions/8859589/quick-alternatives-to-floating-point-multiplication-to-calculate-percentages
void LED::setBrightPercent(int perc)
{
	unsigned long level = (655 * (unsigned long)perc * (unsigned long)_analog_max_level) / 65536;
	if (_currentLevel != level)
	{
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
	if (_currentLevel > ANALOG_LED_LOW)
	{
		analogWrite(_pin, ANALOG_LED_LOW);
		_currentLevel = ANALOG_LED_LOW;
	}
}


void LED::resetEffects()
{
	// pulse related effects
	_pulseStarted = false;
}

// returns the number of pulses so far
int LED::pulse(unsigned long currentTime_us, unsigned long on_period_ms, unsigned long off_spacing_ms)
{
	// initialize the start time when first called
	if (!_pulseStarted)
	{
		// reset to defaults
		_pulseNumFlashes = 0;
		_pulseStarted = true;

		_pulseTime = currentTime_us;

		// LED pulses on at the beginning
		_pulseState = true;
	}
	else {
		unsigned long next_event_time = _pulseState ? 1000*on_period_ms : 1000*off_spacing_ms;
		// whenever we hit next event time, change
		if ((currentTime_us - _pulseTime) >= next_event_time) {
			// toggle state
			_pulseState = !_pulseState;
			// reset start timer
			_pulseTime = currentTime_us;

			if (!_pulseState) {
				// bump number of flashes every off toggle
				_pulseNumFlashes++;
			}
		}

		if (_pulseState){
			// set brightness based on time and current state
			unsigned long perc = (100 * (currentTime_us - _pulseTime)) / next_event_time;
			if (perc <= 50) { // map [0-50] to 0-100
				perc *= 2;
			} else { // map ]50-100] to 50-0
				perc = 2*(100-perc);
			}
			setBrightPercent((int)perc);
		} else {
			setBrightPercent(0.0);
		}
	}
	return _pulseNumFlashes;
}