// HCARD Alien

#include "PinDefinitions.h"

void setup() {
	Serial.begin(9600);

	// potentiometer pin setup
	setupPotentiometer();

	// setup led pins and flash eyebrows
	setupLedEyebrows();
}

void loop() {
	int potentiometerValue = potValue();

	// toggle leds
	boolean browState[3];
	eyebrowStateFromPotValue(potentiometerValue, browState);
	setEyeBrows(browState);
}
