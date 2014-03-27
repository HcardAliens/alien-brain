/*
  FSR.cpp
*/

#include "FSR.h"

// creates an instance of
FSR::FSR(const int pin, int threshold)
{
	_pin = pin; // keep a reference to the pin
	pinMode(pin, INPUT); // to read the FSR value

	_threshold = threshold;
}

int FSR::value()
{
	return analogRead(_pin);
}

FSRTouchState FSR::touchState(int value)
{
	if (value <= _threshold) {
		return TOUCHED;
	} else {
		return UNTOUCHED;
	}
}
