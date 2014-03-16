/*
  PotAntenna.cpp
*/

#include "PotAntenna.h"

// creates an instance of
PotAntenna::PotAntenna(const int pin)
{
	_pin = pin; // keep a reference to the pin
	pinMode(pin, INPUT); // to read the potentiometer value
}

int PotAntenna::value()
{
	return analogRead(_pin);
}

PotRotationState PotAntenna::rotationState(int value)
{
	if (value == 0) {
		return POT_MAX_OUTWARD;
	} else if (value == 1023) {
		return POT_MAX_INWARD;
	} else {
		return POT_IN_BETWEEN;
	}
}