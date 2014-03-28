// HCARD Alien

/*
	Main .ino file
	See http://arduino.cc/en/Hacking/LibraryTutorial for C++ tips on Arduino
*/

// See this file to define new pins and constants
#include "PinDefinitionsAndConstants.h"

#include "PotAntenna.h"
#include "FSR.h"
#include "LEDEyebrow.h"
#include "MotorEye.h"
#include "SoundMouth.h"
#include "DataAcquisition.h"

PotAntenna potAntennaLeft(POT_LEFT_PIN); // left antenna
PotAntenna potAntennaRight(POT_RIGHT_PIN); // right antenna
FSR FSRAntennaLeft(FSR_LEFT_PIN, FSR_THRESHOLD); // left antenna
FSR FSRAntennaRight(FSR_RIGHT_PIN, FSR_THRESHOLD); // right antenna

LEDEyebrow eyebrowLeft(EYEBROW_LEDS_LEFT_PINS, NUM_EYEBROW_LEFT_PINS, 130, 0); // left eyebrow with max and min analog values
LEDEyebrow eyebrowRight(EYEBROW_LEDS_RIGHT_PINS, NUM_EYEBROW_RIGHT_PINS, 130, 0); // right eyebrow with max and min analog values

LEDEyebrow runwayLeft(RUNWAY_LEFT_PINS, NUM_RUNWAY_LEFT_PINS, 130, 0);
LEDEyebrow runwayRight(RUNWAY_RIGHT_PINS, NUM_RUNWAY_RIGHT_PINS, 130, 0);


// Top of the antenna
LEDEyebrow ballTopLeft(BALLTOP_LEFT_PINS, 1, 130, 0);
LEDEyebrow ballTopRight(BALLTOP_RIGHT_PINS, 1, 130, 0);;

MotorEye motorLeft(MOTOR_LEFT_PIN); // left motor
SoundMouth soundMouth(SOUND_PINS); // random number of pins for now

DataAcquisition blueDataMonster; // for sending data over bluetooth
unsigned long BT_SAMPLING_RATE_MS = 3; // in ms

// global state
State GLOBAL_STATE = POWERUP;
int reward_counter = 0;

PotRotationState leftRewardState;
PotRotationState rightRewardState;

//
// Control code
//

void setup() {
	Serial.begin(9600);

	Serial1.begin(9600);

	// reset all effects to be in known state
	eyebrowLeft.resetEffects();
	eyebrowRight.resetEffects();

	runwayLeft.resetEffects();
	runwayRight.resetEffects();

	ballTopLeft.resetEffects();
	ballTopRight.resetEffects();
}

void loop() {

	// get FSR value
	int leftFSRValue = FSRAntennaLeft.value();
	int rightFSRValue = FSRAntennaRight.value();

	// get touch state from FSR value
	FSRTouchState leftTouchState = FSRAntennaLeft.touchState(leftFSRValue);
	FSRTouchState rightTouchState = FSRAntennaRight.touchState(rightFSRValue);

	// get pot value
	int leftPotentiometerValue = potAntennaLeft.value();
	int rightPotentiometerValue = potAntennaRight.value();

	// get rotation state from pot value
	PotRotationState leftRotationState = potAntennaLeft.rotationState(leftPotentiometerValue);
	PotRotationState rightRotationState = potAntennaRight.rotationState(rightPotentiometerValue);

	// bluetooth transfer as close as possible for "accurate" timing
	// and because it has no effect on anything else
	if(GLOBAL_STATE == IDLE){
		// stop sending data when in idle
		blueDataMonster.stopData();
	} else {
		// send data
		// blueDataMonster.sendDataAtSampleRate(millis(), BT_SAMPLING_RATE_MS, leftPotentiometerValue, rightPotentiometerValue, leftFSRValue, rightFSRValue);
	}

	blueDataMonster.sendDataAtSampleRate(millis(), BT_SAMPLING_RATE_MS, leftPotentiometerValue, rightPotentiometerValue, leftFSRValue, rightFSRValue);


	switch (GLOBAL_STATE) {
		// spend 3s in powerup
		case POWERUP:
		{
			// pulse eyebrows, glowing 1s, fading 200ms with 600 ms spacing between pulses and an initial delay of 200ms
			int numberOfTimesPulsed_Left = eyebrowLeft.pulseAll(micros(), 1000, 200, 600, 200);
			int numberOfTimesPulsed_Right = eyebrowRight.pulseAll(micros(), 1000, 200, 600, 200);
			if(numberOfTimesPulsed_Left == 3){ // pulse three times
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

			if(leftTouchState == TOUCHED || rightTouchState == TOUCHED)
			{
				GLOBAL_STATE = ONE_ANTENNA_TOUCHED;
				soundMouth.reset();
				runwayLeft.resetEffects();
				break;
			}

			// eyebrowLeft.setMinBrightnessValue(40); // change min analog
			// eyebrowLeft.setMaxBrightnessValue(200); // change max analog
			// eyebrowRight.setMinBrightnessValue(1); // change min analog
			// eyebrowRight.setMaxBrightnessValue(200); // change max analog

			// pulse eyebrows 3s glow/1s fade with 1s spacing and 0s initial delay for now
			// eyebrowLeft.pulseAll(micros(), 1000, 500, 3000, 0);
			// eyebrowRight.pulseAll(micros(), 3000, 1000, 1000, 0);
			// loop sound at 8s intervals
			 soundMouth.play(micros(), IDLE_SOUND_INDEX, 10*1000);

			 runwayLeft.runWayAll(micros(), 300, 1500); // runway with delay in between each run
			 runwayRight.runWayAll(micros(), 300, 1500); // runway with delay in between each run

			// GLOBAL_STATE = BOTH_ANTENNAS_TOUCHED;
			// eyebrowLeft.resetEffects();
			// soundMouth.reset();
			break;

		}

		case ONE_ANTENNA_TOUCHED:
		{
			if(leftTouchState == TOUCHED && rightTouchState == TOUCHED)
			{
				GLOBAL_STATE = BOTH_ANTENNAS_TOUCHED;
				ballTopLeft.resetEffects();
				ballTopRight.resetEffects();
				break;
			}
			if(leftTouchState == UNTOUCHED && rightTouchState == UNTOUCHED)
			{
				ballTopLeft.resetEffects();
				ballTopRight.resetEffects();
				GLOBAL_STATE = IDLE;
				break;
			}
			if(leftTouchState == TOUCHED)
			{
				ballTopLeft.pulseAll(micros(), 1000, 3000, 2000, 0);
			}
			else if (rightTouchState == TOUCHED)
			{
				ballTopRight.pulseAll(micros(), 1000, 3000, 2000, 0);
			}

			break;
		}

		case BOTH_ANTENNAS_TOUCHED:
		{
			if(leftTouchState == UNTOUCHED && rightTouchState == UNTOUCHED)
			{
				GLOBAL_STATE = IDLE;
				soundMouth.reset();
				break;
			}
			if(leftTouchState == UNTOUCHED || rightTouchState == UNTOUCHED)
			{
				GLOBAL_STATE = ONE_ANTENNA_TOUCHED;
				soundMouth.reset();
				break;
			}

			// use "exact" duration of sound so that play has no delay afterwards
			int numberOfTimesPlayed = soundMouth.play(micros(), BOTH_ANTENNAS_TOUCHED_SOUND_INDEX, 200); // 200 ms minimum
			if(numberOfTimesPlayed == 1){ // only play sound once
				GLOBAL_STATE = ANTENNAS_TWISTED;
				// don't forget to reset sound
				soundMouth.reset();
			}
			break;
		}

		case ANTENNAS_TWISTED:
		{
			if(leftTouchState == UNTOUCHED && rightTouchState == UNTOUCHED)
			{
				GLOBAL_STATE = IDLE;
				eyebrowLeft.resetEffects();
				eyebrowRight.resetEffects();

				ballTopRight.resetEffects();
				ballTopLeft.resetEffects();
				ballTopLeft.Off();
				ballTopRight.Off();
				break;
			}
			if(leftTouchState == UNTOUCHED || rightTouchState == UNTOUCHED)
			{
				GLOBAL_STATE = ONE_ANTENNA_TOUCHED;
				eyebrowLeft.resetEffects();
				eyebrowRight.resetEffects();

				ballTopRight.resetEffects();
				ballTopLeft.resetEffects();
				ballTopLeft.Off();
				ballTopRight.Off();
				break;
			}

			// change to reward when twisted completely
			if( (rightRotationState != POT_IN_BETWEEN) && (leftRotationState != POT_IN_BETWEEN) ){
				GLOBAL_STATE = REWARD;
				leftRewardState = leftRotationState;
				rightRotationState = rightRotationState;
				eyebrowLeft.resetEffects();
				eyebrowRight.resetEffects();

				ballTopRight.resetEffects();
				ballTopLeft.resetEffects();
				ballTopLeft.Off();
				ballTopRight.Off();
				break;
			}

			// toggle leds based on potentiometer value
			eyebrowLeft.setStateBasedOnPotValue(leftPotentiometerValue);
			eyebrowRight.setStateBasedOnPotValue(rightPotentiometerValue);

			// pulse stuff
			ballTopLeft.pulseAll(micros(), 1000, 3000, 2000, 0);
			ballTopRight.pulseAll(micros(), 1000, 3000, 2000, 0);


			break;
		}

		case REWARD:
		{
			if(reward_counter == 5){
				GLOBAL_STATE = MEGA_REWARD;
				break;
			}

			int soundIndex;
			if(leftRewardState == POT_MAX_OUTWARD){
			    if(rightRewardState == POT_MAX_INWARD){
			    	soundIndex = REWARD1_SOUND_INDEX;
			    } else {
			    	soundIndex = REWARD2_SOUND_INDEX;
			    }
			} else {
				if(rightRewardState == POT_MAX_INWARD){
					soundIndex = REWARD3_SOUND_INDEX;
				} else {
					soundIndex = REWARD4_SOUND_INDEX;
				}
			}


			// go googly for 3s
			bool isMotorOver = motorLeft.goGoogly(micros(), 100, 100, 100, 3000);
			// pulse in the meantime
			int numberOfTimesPulsed_Left = eyebrowLeft.pulseAll(micros(), 150, 150, 100, 0);
			int numberOfTimesPulsed_Right = eyebrowRight.pulseAll(micros(), 150, 150, 100, 0);
			int numberOfTimesPlayed = soundMouth.play(micros(), soundIndex, 1500); // 1.5s

			if(isMotorOver && numberOfTimesPlayed >= 1 && numberOfTimesPulsed_Left >= 5){
				eyebrowLeft.resetEffects();
				eyebrowLeft.Off();
				eyebrowRight.resetEffects();
				eyebrowRight.Off();
				soundMouth.reset();
				reward_counter++; // bump counter
				GLOBAL_STATE = IDLE;
			}

			break;
		}

		case MEGA_REWARD:
		{
			reward_counter = 0; // reset reward counter

			// go mega googly for 6s
			bool isMotorOver = motorLeft.goGoogly(micros(), 100, 100, 100, 6000);

			// pulse in the meantime
			int numberOfTimesPulsed_Left = eyebrowLeft.pulseAll(micros(), 150, 150, 100, 0);
			int numberOfTimesPulsed_Right = eyebrowRight.pulseAll(micros(), 150, 150, 100, 0);

			runwayLeft.runWayAll(micros(), 300, 500);
			runwayRight.runWayAll(micros(), 300, 500);

			ballTopLeft.pulseAll(micros(), 1000, 3000, 2000, 0);
			ballTopLeft.pulseAll(micros(), 1000, 3000, 2000, 0);

			int numberOfTimesPlayed = soundMouth.play(micros(), MEGA_REWARD_SOUND_INDEX, 1500); // 1.5s

			if(isMotorOver && numberOfTimesPlayed >= 1 && numberOfTimesPulsed_Left >= 5){
				eyebrowLeft.resetEffects();
				eyebrowLeft.Off();
				eyebrowRight.resetEffects();
				eyebrowRight.Off();

				ballTopRight.resetEffects();
				ballTopRight.Off();
				ballTopLeft.resetEffects();
				ballTopLeft.Off();

				runwayLeft.resetEffects();
				runwayLeft.Off();
				runwayRight.resetEffects();
				runwayRight.Off();

				soundMouth.reset();
				reward_counter++; // bump counter
				GLOBAL_STATE = IDLE;
			}


			break;
		}
	};
}
