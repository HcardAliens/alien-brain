/*
	LED.h

*/
#ifndef LED_h
#define LED_h

#include "Arduino.h"

class LED
{
	public:
		LED(); // only provided for array initialization, not useful otherwise
		LED(const int pin, const int analog_max, const int analog_min); // always use this constructor

		// useful shortcuts
		void On(); // max brightness
		void Off(); // turn off completely
		void ToMinBrightness(); // min brightness

		// change the min and max brightness values for the LED
		void setMaxBrightnessValue(int val);
		void setMinBrightnessValue(int val);

		void setBrightPercent(int perc);

	private:
		int _pin;
		int _analog_max_level;
		int _analog_min_level;
		bool _currentLevel;
};

#endif