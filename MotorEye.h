/*
	MotorEye.h

*/
#ifndef MotorEye_h
#define MotorEye_h

#include "Arduino.h"

class MotorEye
{
	public:
		MotorEye(const int pin);

		// returns if the duration is done
		bool goGoogly(unsigned long currentTime_us, unsigned long googleOnTime_ms, unsigned long googleOffTime_ms, unsigned long googleVelocity, unsigned long totalGoogleTime);
		void on(unsigned long motorVelocity);
		void off();
	private:
		int _pin;
		bool _isOn;

		// used in goGoogly
		unsigned long _googlyStartTime;
		bool _googlyStarted;
};

#endif