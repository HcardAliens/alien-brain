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

		void sendData(unsigned long currentTime_us, unsigned long samplePeriod_ms, int leftPotVal, int rightPotVal);
		void stopData();

		void resetPrevious();
	private:

		unsigned long _startDataTime;
		bool _startedSendingData;

		int _previous_leftPotVal;
                int _previous_rightPotVal;

};

#endif
