/**
 * Software License Agreement
 *
 * (C)Copyright 2013, AirQ Networks s.r.l. (http://www.airqnetworks.com)
 * All rights reserved.
 *
 * AirQ Networks licenses to you the right to use, modify, copy, and
 * distribute this software/library when used in conjunction with an 
 * AirQ Networks transceiver to interface AirQ Networks wireless devices
 * (transceivers, sensors, control boards and other devices produced 
 * by AirQ Networks). Other uses, either express or implied, are prohibited.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * AIRQ NETWORKS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 */
	 
#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <Arduino.h>

#define MSG_TYPE_CONFIRM 	0x40
#define MSG_TYPE_CONFIRMED 	0x80	 
#define MSG_MAX_DATA_LEN	0xA

typedef struct {
     char	 preamble[4];
	 uint8_t rcvid[4];
	 uint8_t type;
	 uint8_t devid[4];
	 uint8_t subtype;
	 uint8_t pkt;
	 uint8_t fwd;	 
	 int8_t  RSSI;
	 uint8_t LQI;
	 uint8_t data[MSG_MAX_DATA_LEN];
	 uint8_t datalen;
 } __data_message;
 
class sNET;

class DataMessage {
	friend class sNET;
	friend class AIRQBaseDevice;
	
private:
	__data_message rawmessage;
	bool umessage;	

protected:
	sNET *snet;

	DataMessage();
	DataMessage(__data_message &rawmessage);
	~DataMessage() {};
	void setSNETReference(sNET *snetptr) {snet = snetptr;};
	void updateFromRawMessage(__data_message *message);
	
public:
	uint8_t *getDeviceID();
	uint8_t getForward() {return rawmessage.fwd;};
	uint8_t *getReceiverID();
	uint8_t getPacketNumber();
	int8_t getRSSI();
	uint8_t getLQI();
	uint8_t getSubType();
	uint8_t getType();
	virtual uint8_t *getData();
	virtual uint8_t getDataLen() {return rawmessage.datalen;};
	void setData(uint8_t *data, uint8_t len);
	bool updated();
	
};

/*
 * AirQ 100
 *
 */

class AIRQ100DataMessage : public DataMessage {
friend class sNET;

protected:
	AIRQ100DataMessage(__data_message &rawmessage) : DataMessage(rawmessage) {};
	~AIRQ100DataMessage() {};
	
public:
	uint8_t getDataLen() {return 3;};
	uint8_t getTEMP();
	int getBATT();
};

/*
 * AirQ 101
 *
 */

class AIRQ101DataMessage : public DataMessage {
friend class sNET;

protected:
	AIRQ101DataMessage(__data_message &rawmessage) : DataMessage(rawmessage) {};
	~AIRQ101DataMessage() {};
	
public:
	uint8_t getDataLen() {return 6;};
	float getTEMP();
	int getBATT();
};

/*
 * AirQ 300
 *
 */
#define AIRQ300_IN1_MASK    0x1
#define AIRQ300_IN2_MASK    0x2
#define AIRQ300_IN3_MASK    0x4
#define AIRQ300_IN4_MASK    0x8
#define AIRQ300_RELAY1_MASK 0x10
#define AIRQ300_RELAY2_MASK 0x20
#define AIRQ300_POWER_MASK  0x40

class AIRQ3XXDataMessage : public DataMessage {
friend class sNET;

private:
	uint8_t risingStatus;

protected:
	AIRQ3XXDataMessage(__data_message &rawmessage) : DataMessage(rawmessage) {};
	~AIRQ3XXDataMessage() {};
	
public:
	uint8_t getDataLen() {return 1;};
	bool getIOStatus(uint8_t mask);
	bool risingIOStatus(uint8_t mask);
};

class AIRQ300DataMessage: public AIRQ3XXDataMessage {
friend class sNET;

protected:
	AIRQ300DataMessage(__data_message &rawmessage) : AIRQ3XXDataMessage(rawmessage) {};
	~AIRQ300DataMessage() {};

public:
	bool getIN1() {return getIOStatus(AIRQ300_IN1_MASK);};
	bool getIN2() {return getIOStatus(AIRQ300_IN2_MASK);};	
	bool getIN3() {return getIOStatus(AIRQ300_IN3_MASK);};	
	bool getIN4() {return getIOStatus(AIRQ300_IN4_MASK);};
	bool getPOWER() {return getIOStatus(AIRQ300_POWER_MASK);};		
	bool getRELAY1() {return getIOStatus(AIRQ300_RELAY1_MASK);};	
	bool getRELAY2() {return getIOStatus(AIRQ300_RELAY2_MASK);};		
};

/*
 * AirQ 305
 *
 */
#define AIRQ305_RELAY1_MASK 0x1
#define AIRQ305_RELAY2_MASK 0x2
#define AIRQ305_RELAY3_MASK 0x4
#define AIRQ305_RELAY4_MASK 0x8
#define AIRQ305_PULSE_RELAY1_MASK 0xC1
#define AIRQ305_PULSE_RELAY2_MASK 0xC2
#define AIRQ305_PULSE_RELAY3_MASK 0xC4
#define AIRQ305_PULSE_RELAY4_MASK 0xC8
#define AIRQ305_IN1_MASK    0x10
#define AIRQ305_IN2_MASK    0x20
#define AIRQ305_IN3_MASK    0x40
#define AIRQ305_IN4_MASK    0x80

class AIRQ305DataMessage: public AIRQ3XXDataMessage {
friend class sNET;

protected:
	AIRQ305DataMessage(__data_message &rawmessage) : AIRQ3XXDataMessage(rawmessage) {};
	~AIRQ305DataMessage() {};

public:
	bool getIN1() {	return getIOStatus(AIRQ305_IN1_MASK);};
	bool getIN2() {return getIOStatus(AIRQ305_IN2_MASK);};	
	bool getIN3() {return getIOStatus(AIRQ305_IN3_MASK);};	
	bool getIN4() {return getIOStatus(AIRQ305_IN4_MASK);};
	bool getRELAY1() {return getIOStatus(AIRQ305_RELAY1_MASK);};
	bool getRELAY2() {return getIOStatus(AIRQ305_RELAY2_MASK);};
	bool getRELAY3() {return getIOStatus(AIRQ305_RELAY3_MASK);};
	bool getRELAY4() {return getIOStatus(AIRQ305_RELAY4_MASK);};
};


/*
 * AirQ 310
 *
 */
#define AIRQ310_RELAY1_MASK 0x1
#define AIRQ310_RELAY2_MASK 0x2
#define AIRQ310_RELAY3_MASK 0x4
#define AIRQ310_RELAY4_MASK 0x8
#define AIRQ310_RELAY5_MASK 0x10
#define AIRQ310_RELAY6_MASK 0x20
#define AIRQ310_PULSE_RELAY1_MASK 0xC1
#define AIRQ310_PULSE_RELAY2_MASK 0xC2
#define AIRQ310_PULSE_RELAY3_MASK 0xC4	 
#define AIRQ310_PULSE_RELAY4_MASK 0xC8	 
#define AIRQ310_PULSE_RELAY5_MASK 0xD0	 
#define AIRQ310_PULSE_RELAY6_MASK 0xE0	 

#define AIRQ310_POWER_MASK  0x40

class AIRQ310DataMessage: public AIRQ3XXDataMessage {
friend class sNET;

protected:
	AIRQ310DataMessage(__data_message &rawmessage) : AIRQ3XXDataMessage(rawmessage) {};
	~AIRQ310DataMessage() {};

public:
	bool getPOWER() {return getIOStatus(AIRQ310_POWER_MASK);};
	bool getRELAY1() {return getIOStatus(AIRQ310_RELAY1_MASK);};
	bool getRELAY2() {return getIOStatus(AIRQ310_RELAY2_MASK);};
	bool getRELAY3() {return getIOStatus(AIRQ310_RELAY3_MASK);};
	bool getRELAY4() {return getIOStatus(AIRQ310_RELAY4_MASK);};
	bool getRELAY5() {return getIOStatus(AIRQ310_RELAY5_MASK);};
	bool getRELAY6() {return getIOStatus(AIRQ310_RELAY6_MASK);};
};


#endif //__MESSAGE_H