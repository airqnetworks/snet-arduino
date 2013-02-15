#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <Arduino.h>


typedef struct {
     char	 preamble[4];
	 uint8_t rcvid[4];
	 uint8_t type;
	 uint8_t devid[4];
	 uint8_t subtype;
	 uint8_t pkt;
	 int8_t  RSSI;
	 uint8_t LQI;
	 uint8_t data[10];
} __data_message;

class DataMessage {
	friend class sNET;
	friend class AIRQBaseDevice;
	
private:
	__data_message rawmessage;
	bool umessage;	

protected:
	DataMessage();
	DataMessage(__data_message &rawmessage);
	void updateFromRawMessage(__data_message *message);
	
public:
	uint8_t *getDeviceID();
	uint8_t *getReceiverID();
	uint8_t getPacketNumber();
	int8_t getRSSI();
	uint8_t getLQI();
	uint8_t getSubType();
	uint8_t getType();
	virtual uint8_t *getData();
	virtual uint8_t getDataLen() {return 0;};
	bool updated();
	
};

#define IN1_MASK    0x1
#define IN2_MASK    0x2
#define IN3_MASK    0x4
#define IN4_MASK    0x8
#define RELAY1_MASK 0x10
#define RELAY2_MASK 0x20
#define POWER_MASK  0x40

class AIRQ300DataMessage: public DataMessage {
friend class sNET;

protected:
	AIRQ300DataMessage(__data_message &rawmessage);

public:
	bool getIOStatus(uint8_t mask);
	
	virtual uint8_t getDataLen() {return 1;};
	bool getIN1() {return getIOStatus(IN1_MASK);};
	bool getIN2() {return getIOStatus(IN2_MASK);};	
	bool getIN3() {return getIOStatus(IN3_MASK);};	
	bool getIN4() {return getIOStatus(IN4_MASK);};
	bool getRELAY1() {return getIOStatus(RELAY1_MASK);};	
	bool getRELAY2() {return getIOStatus(RELAY2_MASK);};		
};

#endif //__MESSAGE_H