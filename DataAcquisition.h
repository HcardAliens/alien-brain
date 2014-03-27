/*
	DataAcquisition.h

*/
#ifndef DataAcquisition_h
#define DataAcquisition_h

#include "Arduino.h"

class DataAcquisition
{
	public:
		DataAcquisition();

		void sendDataAtSampleRate(unsigned long currentTime_ms, unsigned long samplePeriod_ms, int leftPotVal, int rightPotVal, int leftFSRValue, int rightFSRValue);
		void stopData();

	private:
		void sendData(unsigned long currentTime_ms, int leftPotVal, int rightPotVal, int leftFSRValue, int rightFSRValue);

		unsigned long _startDataTime;
		bool _startedSendingData;

};

#endif
