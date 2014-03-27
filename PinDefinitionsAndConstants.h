#ifndef PinDefintionsAndConstants_h
#define PinDefintionsAndConstants_h

#include <Arduino.h>

/******
	All pin defintions for easy editing !
******/

// potentiometer analog read pin
const int POT_LEFT_PIN = A4;
const int POT_RIGHT_PIN = A5;

// FSR analog read pin
const int FSR_LEFT_PIN = A6;
const int FSR_RIGHT_PIN = A7;


// LED write pins
const int NUM_EYEBROW_LEFT_PINS = 3;
const int EYEBROW_LEDS_LEFT_PINS[NUM_EYEBROW_LEFT_PINS] = {2, 3, 4};

const int NUM_EYEBROW_RIGHT_PINS = 3;
const int EYEBROW_LEDS_RIGHT_PINS[NUM_EYEBROW_RIGHT_PINS] = {7, 8, 9};

// Motors
const int MOTOR_LEFT_PIN = 11;

// Sound
const int SOUND_PINS[5] = {31,33,34,37,39}; // random for now
// sounds correspond to the pins above
const int IDLE_SOUND_INDEX = 1;
const int BOTH_ANTENNAS_TOUCHED_SOUND_INDEX = 0;
const int REWARD_SOUND_INDEX = 2;



/******
	Constants too !
******/
// NOTE: make this bigger than the biggest LED array used anywhere
#define MAX_LED_ARRAY_SIZE_EVER 10

const int POT_THRESHOLDS[3] = {250, 500, 800};
const int FSR_THRESHOLD = 900;


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

// FSR states
typedef enum FSRTouchState {
	TOUCHED,
	UNTOUCHED,
};

#endif
