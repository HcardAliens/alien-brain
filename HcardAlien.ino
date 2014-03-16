// HCARD Alien

/*
	Main .ino file
	See http://arduino.cc/en/Hacking/LibraryTutorial for C++ tips on Arduino
*/

// See this file to define new pins and constants
#include "PinDefinitions+Constants.h"

#include "PotAntenna.h"
#include "LEDEyebrow.h"
#include "MotorEye.h"
#include "SoundMouth.h"
#include "DataAcquisition.h"
#include "AntennaGlobeHandle.h"

PotAntenna potAntennaLeft(POT_LEFT_PIN); // left antenna
LEDEyebrow eyebrowLeft(EYEBROW_LEDS_LEFT_PINS, 3); // left eyebrow with max analog value of 60
MotorEye motorLeft(MOTOR_LEFT_PIN); // left motor
SoundMouth soundMouth(SOUND_PINS); // random number of pins for now
// AntennaGlobeHandle globeHandleLeft(TODO_PIN_FSR, TODO_PIN_LED, 50); // class to use for FSR + LED in the antenna handle
DataAcquisition blueDataMonster; // for sending data over bluetooth

// global state
State GLOBAL_STATE = POWERUP;

//
// Control code
//
void setup() {
	Serial.begin(9600);

	// reset all effects to be in known state
	eyebrowLeft.resetEffects();
}

void loop() {
	switch (GLOBAL_STATE) {
		// spend 3s in powerup
		case POWERUP:
		{
			// pulse eyebrows, duration 1s with 500 ms spacing
			int numberOfTimesPulsed = eyebrowLeft.pulseAll(micros(), 1000, 50);
			if(numberOfTimesPulsed == 1){ // pulse once
				// don't forget to reset
				eyebrowLeft.resetEffects();
				// turn off all LEDs just in case
				eyebrowLeft.setAllOff();
		        GLOBAL_STATE = IDLE;
			}
			break;
		}

		case IDLE:
		{
			// pulse eyebrows 3s with 5s spacing for now
			// eyebrowLeft.pulseAll(micros(), 3000, 5000);
			// loop sound at 8s intervals
			// soundMouth.play(micros(), INVITING_SOUND_INDEX, 8*1000);

			GLOBAL_STATE = BOTH_ANTENNAS_TOUCHED;
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
			int potentiometerValue = potAntennaLeft.value();

			// send data with 1ms sampling
			blueDataMonster.sendData(micros(), 1000, potentiometerValue);

			// toggle leds based on potentiometer value
			eyebrowLeft.setStateBasedOnPotValue(potentiometerValue);

			// get rotation state from pot value
			PotRotationState rotationState = potAntennaLeft.rotationState(potentiometerValue);
			// change to reward when twisted completely
			if(rotationState == POT_MAX_INWARD){
				GLOBAL_STATE = REWARD;
				// stop sending data
				blueDataMonster.stopData();
			}
			break;
		}

		case REWARD:
		{
			// go googly for 3s
			bool isMotorOver = motorLeft.goGoogly(micros(), 3000);
			// pulse in the meantime
			int numberOfTimesPulsed = eyebrowLeft.pulseAll(micros(), 100, 100);
			if(isMotorOver && numberOfTimesPulsed >= 5){
				eyebrowLeft.resetEffects();
				eyebrowLeft.setAllOff();
			    GLOBAL_STATE = POWERUP;
			}
			break;
		}

		case MEGA_REWARD:
		{
			break;
		}
	};
}
