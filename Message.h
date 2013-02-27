/**
 * Software License Agreement
 *
 * (C)Copyright 2013, AirQ Networks s.r.l. (http://www.airqnetworks.com)
 * All rights reserved.
 *
 * AirQ Networks licenses to you the right to use, modify, copy, and
 * distribute this software/library when used in conjuction with an 
 * AirQ Networks trasceiver to interface AirQ Networks wireless devices
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
	 uint8_t data[10];
	 uint8_t datalen;
} __data_message;

class DataMessage {
	friend class sNET;
	friend class AIRQBaseDevice;
	
private:
	__data_message rawmessage;
	bool umessage;	

protected:
	DataMessage();
	~DataMessage() {};
	DataMessage(__data_message &rawmessage);
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
	bool updated();
	
};

#define AIRQ300_IN1_MASK    0x1
#define AIRQ300_IN2_MASK    0x2
#define AIRQ300_IN3_MASK    0x4
#define AIRQ300_IN4_MASK    0x8
#define AIRQ300_RELAY1_MASK 0x10
#define AIRQ300_RELAY2_MASK 0x20
#define AIRQ300_POWER_MASK  0x40

class AIRQ300DataMessage: public DataMessage {
friend class sNET;

protected:
	AIRQ300DataMessage(__data_message &rawmessage);
	~AIRQ300DataMessage() {};

public:
	uint8_t getDataLen() {return 1;};
	bool getIOStatus(uint8_t mask);
	bool getIN1() {return getIOStatus(AIRQ300_IN1_MASK);};
	bool getIN2() {return getIOStatus(AIRQ300_IN2_MASK);};	
	bool getIN3() {return getIOStatus(AIRQ300_IN3_MASK);};	
	bool getIN4() {return getIOStatus(AIRQ300_IN4_MASK);};
	bool getPOWER() {return getIOStatus(AIRQ300_POWER_MASK);};		
	bool getRELAY1() {return getIOStatus(AIRQ300_RELAY1_MASK);};	
	bool getRELAY2() {return getIOStatus(AIRQ300_RELAY2_MASK);};		
};

#define AIRQ310_RELAY1_MASK 0x1
#define AIRQ310_RELAY2_MASK 0x2
#define AIRQ310_RELAY3_MASK 0x4
#define AIRQ310_RELAY4_MASK 0x8
#define AIRQ310_RELAY5_MASK 0x10
#define AIRQ310_RELAY6_MASK 0x20
#define AIRQ310_POWER_MASK  0x40

class AIRQ310DataMessage: public DataMessage {
friend class sNET;

protected:
	AIRQ310DataMessage(__data_message &rawmessage);
	~AIRQ310DataMessage() {};

public:
	
	uint8_t getDataLen() {return 1;};
	bool getIOStatus(uint8_t mask);
	bool getPOWER() {return getIOStatus(AIRQ310_POWER_MASK);};
	bool getRELAY1() {return getIOStatus(AIRQ310_RELAY1_MASK);};
	bool getRELAY2() {return getIOStatus(AIRQ310_RELAY2_MASK);};
	bool getRELAY3() {return getIOStatus(AIRQ310_RELAY3_MASK);};
	bool getRELAY4() {return getIOStatus(AIRQ310_RELAY4_MASK);};
	bool getRELAY5() {return getIOStatus(AIRQ310_RELAY5_MASK);};
	bool getRELAY6() {return getIOStatus(AIRQ310_RELAY6_MASK);};
};


#endif //__MESSAGE_H