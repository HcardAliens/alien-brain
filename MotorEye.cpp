/*
  MotorEye.cpp
*/

#include "MotorEye.h"

MotorEye::MotorEye(const int pin)
{
	_pin = pin; // keep a reference to the pin
	pinMode(pin, OUTPUT);

	_isOn = false;
}

// returns whether the googly time is over
bool MotorEye::goGoogly(unsigned long currentTime_us, unsigned long duration_ms)
{
	// initialize the start time when first called
	if (!_googlyStarted)
	{
		_googlyStartTime = currentTime_us;
		_googlyStarted = true;
		on();
	} else {
		unsigned long next_event_time = 1000*duration_ms;
		// whenever we hit next event time, which is the end
		if ((currentTime_us - _googlyStartTime) >= next_event_time) {
			// stop the motor
			off();
			// reset state back
			_googlyStarted = false;
			// return that we are done
			return true;
		}
	}
	return false;
}

void MotorEye::on()
{
	// only turn on if not already on
	if (!_isOn) {
		digitalWrite(_pin, HIGH);
		_isOn = true;
	}
}

void MotorEye::off()
{
	// only turn off if not already off...
	if (_isOn) {
		digitalWrite(_pin, LOW);
		_isOn = false;
	}
}