/**
 * Software License Agreement
 *
 * (C)Copyright 2013, AirQ Networks s.r.l. (http://www.airqnetworks.com)
 * All rights reserved.
 *
 * AirQ Networks licenses to you the right to use, modify, copy, and
 * distribute this software/library when used in conjuction with an 
 * AirQ Networks trasceiver to interface AirQ Networks wireless devices
 * (sensors, control boards and other devices produced by AirQ Networks).
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
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Carmine Noviello    15/02/13  Original
 */

#ifndef __SNET_H
#define __SNET_H

#include <Arduino.h>

#include <inttypes.h>
#include <SoftwareSerial.h>
	  
#define SNET_SERIAL_BAUDRATE 19200
#define SNET_MAX_MESSAGE_SIZE   30
	  
typedef struct {
     char	 preamble[4];
	 uint8_t rcvid[4];
	 uint8_t type;
	 uint8_t devid[4];
	 uint8_t pkt;
	 int8_t  RSSI;
	 uint8_t LQI;
	 uint8_t *data;
} __data_message;

class AIRQBaseBoard {
private:
    static AIRQBaseBoard* instance_ptr;
    AIRQBaseBoard() { };

public:
    ~AIRQBaseBoard() {};
    static AIRQBaseBoard* get_instance() {
            if (instance_ptr == NULL) {
                    instance_ptr = new AIRQBaseBoard;
            }
            return instance_ptr;
    }
    bool method() { return true; };
};

class sNET  {
public:
	 sNET();
	 sNET(uint8_t rxPin, uint8_t txPin);
     ~sNET();

     void processMessages();
			 
private:
	 SoftwareSerial serial;
	 
	 void init(uint8_t rxPin, uint8_t txPin);
	 uint8_t readSNETMessage(SoftwareSerial *serial, __data_message *msg);
};

class DataMessage {
	friend class sNET;
	
private:
	__data_message rawmessage;
protected:
	DataMessage(__data_message &rawmessage);
	
public:
	int getDeviceID();
};

#endif //__SNET_H
