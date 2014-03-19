/*
  AntennaGlobeHandle.cpp
  A class representing any AntennaGlobeHandle that should (at some point !) have many functions needed elsewhere
*/

#include "Arduino.h"
#include "AntennaGlobeHandle.h"
#include "PinDefinitions+Constants.h"
#include "LED.h"

// creates an instance of a AntennaGlobeHandle
AntennaGlobeHandle::AntennaGlobeHandle(const int pinFSR, const int pinLED, int analog_max_level, int analog_min_level)
{
		_pinFSR = pinFSR;
		// pinMode(_pinFSR, INPUT);

		_led = LED(pinLED, analog_max_level, analog_min_level);
}