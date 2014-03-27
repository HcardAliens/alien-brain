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

		void sendDataAtSampleRate(unsigned long currentTime_us, unsigned long samplePeriod_ms, int leftPotVal, int rightPotVal);
		void stopData();

	private:
		void sendData(unsigned long currentTime_us, int leftPotVal, int rightPotVal);

		unsigned long _startDataTime;
		bool _startedSendingData;

};

#endif
