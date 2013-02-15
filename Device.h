#ifndef __DEVICE_H
#define __DEVICE_H

#include <Arduino.h>
#include "Message.h"

class DataMessage;

class AIRQBaseDevice {
friend class sNET;

protected:
	DataMessage *status;
	
	AIRQBaseDevice(DataMessage *message);
	void updateFromRawMessage(__data_message *message);
	
public:
	~AIRQBaseDevice();
	uint8_t *getDeviceID();
	DataMessage *getLastMessage();
	uint8_t getLQI();
	int8_t getRSSI();
	virtual bool statusChanged() {return false;};
};

class AIRQ300 : public AIRQBaseDevice {
friend class sNET;

private:
	uint8_t lstatus;
	
protected:
	AIRQ300(DataMessage *message);
	
public: 
	bool getIN1() {return ((AIRQ300DataMessage*)status)->getIN1();};
	virtual bool statusChanged();
};

#endif //__DEVICE_H