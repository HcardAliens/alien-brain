/*
  DataAcquisition.cpp
*/

#include "DataAcquisition.h"

DataAcquisition::DataAcquisition()
{
	_startedSendingData = false;
}

void DataAcquisition::sendDataAtSampleRate(unsigned long currentTime_ms, unsigned long samplePeriod_ms, int leftPotVal, int rightPotVal, int leftFSRValue, int rightFSRValue)
{
	if (!_startedSendingData)
	{
		_startedSendingData = true;
		_startDataTime = currentTime_ms;
		// send data straight away: don't wait till next samplePeriod
		sendData(currentTime_ms, leftPotVal, rightPotVal, leftFSRValue, rightFSRValue);
	}


	if ((currentTime_ms - _startDataTime) >= samplePeriod_ms)
	{
		sendData(currentTime_ms, leftPotVal, rightPotVal, leftFSRValue, rightFSRValue);
		_startDataTime = currentTime_ms;
	}
}

void DataAcquisition::sendData(unsigned long currentTime_ms, int leftPotVal, int rightPotVal, int leftFSRValue, int rightFSRValue)
{
	Serial1.write("*\t");
	Serial1.print(currentTime_ms, DEC);
	Serial1.print("\t");
	Serial1.print(leftPotVal, DEC);
	Serial1.print("\t");
	Serial1.print(rightPotVal, DEC);
	Serial1.print("\t");
	Serial1.print(leftFSRValue, DEC);
	Serial1.print("\t");
	Serial1.print(rightFSRValue, DEC);
	Serial1.print("\n");
}

void DataAcquisition::stopData()
{
	_startedSendingData = false;
}
