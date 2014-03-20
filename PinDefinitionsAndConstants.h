#ifndef PinDefintions+Constants_h
#define PinDefintions+Constants_h

#include <Arduino.h>

/******
	All pin defintions for easy editing !
******/

// potentiometer analog read pin
const int POT_LEFT_PIN = A4;
const int POT_RIGHT_PIN = A5;


// LED write pins
const int EYEBROW_LEDS_LEFT_PINS[3] = {2, 3, 4};
const int EYEBROW_LEDS_RIGHT_PINS[3] = {5, 6, 7};

// Motors
const int MOTOR_LEFT_PIN = 11;

// Sound
const int SOUND_PINS[5] = {30,31,32,33,34}; // random for now
// sounds correspond to the pins above
const int INVITING_SOUND_INDEX = 0;
const int TOUCHED_SOUND_INDEX = 1;


/******
	Constants too !
******/
const int POT_THRESHOLDS[3] = {250, 500, 800};

typedef enum State {
	POWERUP,
	IDLE,
	ONE_ANTENNA_TOUCHED,
	BOTH_ANTENNAS_TOUCHED,
	ANTENNAS_TWISTED,
	REWARD,
	MEGA_REWARD
};

// potentiometer states
typedef enum PotRotationState {
	POT_IN_BETWEEN,
	POT_MAX_OUTWARD,
	POT_MAX_INWARD,
};

#endif
