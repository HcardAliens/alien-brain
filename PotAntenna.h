/*
	PotAntenna.h

*/
#ifndef PotAntenna_h
#define PotAntenna_h

#include "Arduino.h"
#include "PinDefinitionsAndConstants.h"

class PotAntenna
{
	public:
		PotAntenna(const int pin);
		int value();
		PotRotationState rotationState(int value);
	private:
		int _pin;
};

#endif
