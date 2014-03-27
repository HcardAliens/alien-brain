/*
  DataAcquisition.cpp
*/

#include "DataAcquisition.h"

DataAcquisition::DataAcquisition()
{
	_startedSendingData = false;
}

void DataAcquisition::sendDataAtSampleRate(unsigned long currentTime_us, unsigned long samplePeriod_ms, int leftPotVal, int rightPotVal)
{
	if (!_startedSendingData)
	{
		_startedSendingData = true;
		_startDataTime = currentTime_us;
		// send data straight away: don't wait till next samplePeriod
		sendData(currentTime_us, leftPotVal, rightPotVal);
	}


	if ((currentTime_us - _startDataTime) >= samplePeriod_ms)
	{
		sendData(currentTime_us, leftPotVal, rightPotVal);
		_startDataTime = currentTime_us;
	}
}

void DataAcquisition::sendData(unsigned long currentTime_us, int leftPotVal, int rightPotVal)
{
	Serial.write("*\t");
	Serial.print(currentTime_us, DEC);
	Serial.print("\t");
	Serial.print(leftPotVal, DEC);
	Serial.print("\t");
	Serial.print(rightPotVal, DEC);
	Serial.print("\n");
}

void DataAcquisition::stopData()
{
	_startedSendingData = false;
}
