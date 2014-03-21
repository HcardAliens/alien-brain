// HCARD Alien

/*
	Main .ino file
	See http://arduino.cc/en/Hacking/LibraryTutorial for C++ tips on Arduino
*/

// See this file to define new pins and constants
#include "PinDefinitionsAndConstants.h"

#include "PotAntenna.h"
#include "LEDEyebrow.h"
#include "MotorEye.h"
#include "SoundMouth.h"
#include "DataAcquisition.h"

PotAntenna potAntennaLeft(POT_LEFT_PIN); // left antenna
PotAntenna potAntennaRight(POT_RIGHT_PIN); // right antenna
LEDEyebrow eyebrowLeft(EYEBROW_LEDS_LEFT_PINS, NUM_EYEBROW_LEFT_PINS, 130, 0); // left eyebrow with max and min analog values
LEDEyebrow eyebrowRight(EYEBROW_LEDS_RIGHT_PINS, NUM_EYEBROW_RIGHT_PINS, 130, 0); // right eyebrow with max and min analog values
MotorEye motorLeft(MOTOR_LEFT_PIN); // left motor
SoundMouth soundMouth(SOUND_PINS); // random number of pins for now

DataAcquisition blueDataMonster; // for sending data over bluetooth
unsigned long BT_SAMPLING_RATE_US = 1000; // 1ms

// global state
State GLOBAL_STATE = IDLE;
int reward_counter = 0;

//
// Control code
//

void setup() {
	Serial.begin(9600);

	// reset all effects to be in known state
	eyebrowLeft.resetEffects();
	eyebrowRight.resetEffects();
}

void loop() {
	switch (GLOBAL_STATE) {
		// spend 3s in powerup
		case POWERUP:
		{
			// pulse eyebrows, glowing 1s, fading 200ms with 600 ms spacing between pulses and an initial delay of 20ms
			int numberOfTimesPulsed_Left = eyebrowLeft.pulseAll(micros(), 1000, 200, 600, 20);
			int numberOfTimesPulsed_Right = eyebrowRight.pulseAll(micros(), 1000, 200, 600, 20);
			if(numberOfTimesPulsed_Left == 1){ // pulse once
				// don't forget to reset
				eyebrowLeft.resetEffects();
				eyebrowRight.resetEffects();
				// turn off all LEDs just in case
				eyebrowLeft.Off();
				eyebrowRight.Off();

				GLOBAL_STATE = IDLE;
			}
			break;
		}

		case IDLE:
		{
			eyebrowLeft.setMinBrightnessValue(1); // change min analog
			eyebrowLeft.setMaxBrightnessValue(200); // change max analog
			eyebrowRight.setMinBrightnessValue(1); // change min analog
			eyebrowRight.setMaxBrightnessValue(200); // change max analog

			// pulse eyebrows 3s glow/1s fade with 1s spacing and 0s initial delay for now
			// eyebrowLeft.pulseAll(micros(), 3000, 1000, 1000, 0);
			// eyebrowRight.pulseAll(micros(), 3000, 1000, 1000, 0);
			// loop sound at 8s intervals
			 soundMouth.play(micros(), INVITING_SOUND_INDEX, 5*1000);

			 eyebrowLeft.runWayAll(micros(), 300, 1500); // runway with delay in between each run

			// GLOBAL_STATE = BOTH_ANTENNAS_TOUCHED;
			// eyebrowLeft.resetEffects();
			// soundMouth.reset();
			break;
		}

		case ONE_ANTENNA_TOUCHED:
		{
			break;
		}

		case BOTH_ANTENNAS_TOUCHED:
		{
			// use "exact" duration of sound so that play has no delay afterwards
			int numberOfTimesPlayed = soundMouth.play(micros(), INVITING_SOUND_INDEX, 1500); // 1.5s
			if(numberOfTimesPlayed == 1){ // only play sound once
				GLOBAL_STATE = ANTENNAS_TWISTED;
				// don't forget to reset sound
				soundMouth.reset();
			}
			break;
		}

		case ANTENNAS_TWISTED:
		{
			// get pot value
			int leftPotentiometerValue = potAntennaLeft.value();
			int rightPotentiometerValue = potAntennaRight.value();

			// send data with 1ms sampling
			blueDataMonster.sendDataAtSampleRate(micros(), BT_SAMPLING_RATE_US, leftPotentiometerValue, rightPotentiometerValue);


			// toggle leds based on potentiometer value
			eyebrowLeft.setStateBasedOnPotValue(leftPotentiometerValue);
			eyebrowRight.setStateBasedOnPotValue(rightPotentiometerValue);

			// get rotation state from pot value
			PotRotationState leftRotationState = potAntennaLeft.rotationState(leftPotentiometerValue);
			PotRotationState rightRotationState = potAntennaRight.rotationState(rightPotentiometerValue);

			// change to reward when twisted completely
			if( (rightRotationState == POT_MAX_INWARD) && (leftRotationState == POT_MAX_INWARD) ){
				GLOBAL_STATE = REWARD;
				// stop sending data
				blueDataMonster.stopData();
			}
			break;
		}

		case REWARD:
		{
			if(reward_counter == 5){
				GLOBAL_STATE = MEGA_REWARD;
				break;
			}
			// go googly for 3s
			bool isMotorOver = motorLeft.goGoogly(micros(), 3000);
			// pulse in the meantime
			int numberOfTimesPulsed_Left = eyebrowLeft.pulseAll(micros(), 50, 50, 100, 0);
			int numberOfTimesPulsed_Right = eyebrowRight.pulseAll(micros(), 50, 50, 100, 0);
			if(isMotorOver && numberOfTimesPulsed_Left >= 5){
				eyebrowLeft.resetEffects();
				eyebrowLeft.Off();
				eyebrowRight.resetEffects();
				eyebrowRight.Off();
				reward_counter++; // bump counter
				GLOBAL_STATE = IDLE;
			}
			break;
		}

		case MEGA_REWARD:
		{
			GLOBAL_STATE = IDLE;
			reward_counter = 0;
			break;
		}
	};
}
