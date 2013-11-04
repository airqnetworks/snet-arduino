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
	 
#ifndef __DEVICE_H
#define __DEVICE_H

#include <Arduino.h>
#include "Message.h"

class DataMessage;
// class _sNET;

class AIRQBaseDevice {
friend class _sNET;

protected:
	DataMessage *status;
	// _sNET *snet;
	
	AIRQBaseDevice(DataMessage *message);
	~AIRQBaseDevice();
	// void setSNETReference(_sNET *snetptr) {snet = snetptr;};
	void updateFromRawMessage(__data_message *message);
	
public:
	uint8_t *getDeviceID();
	DataMessage *getLastMessage();
	uint8_t getLQI();
	int8_t getRSSI();
	virtual bool statusChanged() {return status->updated();};
};


#define AIRQ100 AIRQ101
	
class AIRQ101: public AIRQBaseDevice {
friend class _sNET;

protected:
	AIRQ101(DataMessage *message) : AIRQBaseDevice(message) {};
	~AIRQ101() {};

public:
	
	float getTEMP() {return ((AIRQ101DataMessage*)status)->getTEMP();}
	int getBATT() {return ((AIRQ101DataMessage*)status)->getBATT();}	
};


typedef enum {
	ON = 1,
	OFF = 0
} IO_STATUS;

class AIRQControlBoard: public AIRQBaseDevice {
friend class _sNET;

private:
	uint8_t lstatus;
	uint8_t lconftoken;

protected:
	uint8_t RELAY_XOR_MASK;
	AIRQControlBoard(DataMessage *message);
	~AIRQControlBoard() { };

public:
	virtual bool statusChanged();
	void sendSetMessage(uint8_t subtype, uint8_t *data, uint8_t len, bool confirm=true);
	void setRELAY(uint8_t rmask, IO_STATUS rstatus, uint8_t duration);
	void setIO(uint8_t subtype, uint8_t *data, uint8_t len);
};


/*
 * AirQ 300
 *
 */
class AIRQ300 : public AIRQControlBoard {
friend class _sNET;
	
protected:
	AIRQ300(DataMessage *message);
	~AIRQ300() { };
	
public: 
	IO_STATUS getIN1() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN1();};
	IO_STATUS getIN2() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN2();};
	IO_STATUS getIN3() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN3();};
	IO_STATUS getIN4() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN4();};
	IO_STATUS getPOWER() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getPOWER();};
	IO_STATUS getRELAY1() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getRELAY1();};
	IO_STATUS getRELAY2() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getRELAY2();};
	
	void setRELAY1(IO_STATUS status);
	void setRELAY2(IO_STATUS status);

};
   
/*
 * AirQ 305
 *
 */
class AIRQ305 : public AIRQControlBoard {
friend class _sNET;
	
protected:
	AIRQ305(DataMessage *message);
	~AIRQ305() { };
	
public: 
	IO_STATUS getIN1() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getIN1();};
	IO_STATUS getIN2() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getIN2();};
	IO_STATUS getIN3() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getIN3();};
	IO_STATUS getIN4() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getIN4();};
	IO_STATUS getRELAY1() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getRELAY1();};
	IO_STATUS getRELAY2() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getRELAY2();};
	IO_STATUS getRELAY3() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getRELAY3();};
	IO_STATUS getRELAY4() {return (IO_STATUS)((AIRQ305DataMessage*)status)->getRELAY4();};
	
	void pulseRELAY1();
	void pulseRELAY2();
	void pulseRELAY3();
	void pulseRELAY4();
	
	bool risingIN1() {return ((AIRQ305DataMessage*)status)->risingIOStatus(AIRQ305_IN1_MASK);};
	bool risingIN2() {return ((AIRQ305DataMessage*)status)->risingIOStatus(AIRQ305_IN2_MASK);};	
	bool risingIN3() {return ((AIRQ305DataMessage*)status)->risingIOStatus(AIRQ305_IN3_MASK);};	
	bool risingIN4() {return ((AIRQ305DataMessage*)status)->risingIOStatus(AIRQ305_IN4_MASK);};
				
	void setRELAY1(IO_STATUS status, uint8_t duration = 0);
	void setRELAY2(IO_STATUS status, uint8_t duration = 0);
	void setRELAY3(IO_STATUS status, uint8_t duration = 0);
	void setRELAY4(IO_STATUS status, uint8_t duration = 0);
};
   
/*
 * AirQ 310
 *
 */
class AIRQ310 : public AIRQControlBoard {
friend class _sNET;

protected:
	AIRQ310(DataMessage *message);
	~AIRQ310() { };
	
public: 
	IO_STATUS getPOWER() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getPOWER();};
	IO_STATUS getRELAY1() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY1();};
	IO_STATUS getRELAY2() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY2();};	
	IO_STATUS getRELAY3() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY3();};	
	IO_STATUS getRELAY4() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY4();};	
	IO_STATUS getRELAY5() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY5();};				
	IO_STATUS getRELAY6() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY6();};		

	void pulseRELAY1();
	void pulseRELAY2();
	void pulseRELAY3();
	void pulseRELAY4();
	void pulseRELAY5();
	void pulseRELAY6();

	void setRELAY1(IO_STATUS status);
	void setRELAY2(IO_STATUS status);
	void setRELAY3(IO_STATUS status);
	void setRELAY4(IO_STATUS status);
	void setRELAY5(IO_STATUS status);
	void setRELAY6(IO_STATUS status);			
};

#endif //__DEVICE_H