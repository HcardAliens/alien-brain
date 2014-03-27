/*
  FSR.cpp
*/

#include "FSR.h"

// creates an instance of
FSR::FSR(const int pin)
{
	_pin = pin; // keep a reference to the pin
	pinMode(pin, INPUT); // to read the potentiometer value
}

int FSR::value()
{
	return analogRead(_pin);
}

FSRTouchState FSR::touchState(int value)
{
	if (value <= FSR_THRESHOLD) {
		return TOUCHED;
	} else {
		return UNTOUCHED;
	}
}
