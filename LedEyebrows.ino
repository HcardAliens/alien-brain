// LedEyebrows

boolean EYEBROWS_ALL_ON[3] = {true, true, true};
boolean EYEBROWS_ALL_OFF[3] = {false, false, false};

// sets the led pin modes and flashes the LEDS once for some visual feedback
void setupLedEyebrows() {
	for(int i=0; i < 3;i++) {
		pinMode(eyeBrowLeftLEDs[i], OUTPUT);
	}
	// flash eyebrows for 1s
	setEyeBrows(EYEBROWS_ALL_ON);
	delay(1000);
	setEyeBrows(EYEBROWS_ALL_OFF);
}

// sets the state array values using the potentiometer value
void eyebrowStateFromPotValue(int val, boolean state[3]) {
	// thresholds to turn on each led
	int thresholds[3] = {250, 500, 800}; // range is 0-1023
	for(int i=0; i < 3;i++) {
		state[i] = val > (thresholds[i]) ? true : false;
	}
}

// toggles the eyebrow LEDs depending on state
void setEyeBrows(boolean state[3]) {
	for(int i=0; i < 3;i++) {
		if(state[i]) {
			digitalWrite(eyeBrowLeftLEDs[i], HIGH);
		} else {
			digitalWrite(eyeBrowLeftLEDs[i], LOW);
		}
	}
}
