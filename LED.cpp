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

void LED::On()
{
	if (_currentLevel != _analog_max_level)
	{
		analogWrite(_pin, _analog_max_level);
		_currentLevel = _analog_max_level;
	}
}

void LED::Off()
{
	if (_currentLevel != 0)
	{
		analogWrite(_pin, 0);
		_currentLevel = 0;
	}
}

void LED::ToMinBrightness()
{
	if (_currentLevel != _analog_min_level)
	{
		analogWrite(_pin, _analog_min_level);
		_currentLevel = _analog_min_level;
	}
}

void LED::setMaxBrightnessValue(int val)
{
	_analog_max_level = val;
}

void LED::setMinBrightnessValue(int val)
{
	_analog_min_level = val;
}
