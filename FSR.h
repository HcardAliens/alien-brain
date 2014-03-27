/*
	FSR.h

*/
#ifndef FSR_h
#define FSR_h

#include "Arduino.h"
#include "PinDefinitionsAndConstants.h"

class FSR
{
	public:
		FSR(const int pin);
		int value();
		FSRTouchState touchState(int value);
	private:
		int _pin;
};

#endif
