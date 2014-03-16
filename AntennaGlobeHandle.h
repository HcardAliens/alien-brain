/*
	AntennaGlobeHandle.h

*/
#ifndef AntennaGlobeHandle_h
#define AntennaGlobeHandle_h

#include "Arduino.h"
#include "LED.h"

class AntennaGlobeHandle
{
	public:
		// this class takes care of the globe-y handle thing on top of the antenna
		// it includes the LED and the FSR
		AntennaGlobeHandle(const int pinFSR, const int pinLED, int analog_max_level);

	private:
		int _pinFSR;
		LED _led;
};

#endif