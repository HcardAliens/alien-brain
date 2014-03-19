/*
  DataAcquisition.cpp
*/

#include "DataAcquisition.h"

DataAcquisition::DataAcquisition()
{
	_startedSendingData = false;
	resetPrevious();
}

void DataAcquisition::sendData(unsigned long currentTime_us, unsigned long samplePeriod_ms, int leftPotVal)
{
	if (!_startedSendingData)
	{
		_startedSendingData = true;
		_startDataTime = currentTime_us;
	}


	if ((currentTime_us - _startDataTime) >= samplePeriod_ms)
	{
		// don't send the exact same thing twice
		if (_previous_leftPotVal == leftPotVal) {
			return;
		}
		_startDataTime = currentTime_us;
		Serial.print("DATA\r\n");
		Serial.print(leftPotVal, DEC);
		Serial.print("\r\n");

		// store previous values
		_previous_leftPotVal = leftPotVal;
	}
}

void DataAcquisition::stopData()
{
	_startedSendingData = false;
	resetPrevious();
}

void DataAcquisition::resetPrevious()
{
	_previous_leftPotVal = -1;
}