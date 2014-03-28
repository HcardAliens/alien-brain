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
bool MotorEye::goGoogly(unsigned long currentTime_us, unsigned long googleOnTime_ms, unsigned long googleOffTime_ms, unsigned long googleVelocity, unsigned long totalGoogleTime)
{
	// initialize the start time when first called
	if (!_googlyStarted)
	{
		_googlyStartTime = currentTime_us;
		_googlyStarted = true;
		
	} else {
		unsigned long next_event_time = 1000*totalGoogleTime;
		// whenever we hit next event time, which is the end

		unsigned long googleSequenceTime = 1000*(googleOnTime_ms + googleOffTime_ms);

		if ((currentTime_us - _googlyStartTime)%googleSequenceTime < googleOnTime_ms )
		{
			on(googleVelocity);
		}



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

void MotorEye::on(unsigned long motorVelocity)
{
	// only turn on if not already on
	if (!_isOn) {
		analogWrite(_pin, motorVelocity);
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