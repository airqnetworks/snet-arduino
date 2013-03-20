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
	 
#ifndef __SNET_H
#define __SNET_H

#define SNET_ENABLE_CONFIRM

#include <Arduino.h>
#include <SoftwareSerial.h>
	 
#include "Message.h"
#include "Device.h"
	  
#define SNET_SERIAL_BAUDRATE 		19200
#define SNET_MAX_DATAMESSAGE_SIZE   30
#define SNET_MAX_SETMESSAGE_SIZE    30	 
#define SNET_DEV_ADDR_LEN		 	4
#define SNET_DEFAULT_RX_PIN			10	 
#define SNET_DEFAULT_TX_PIN			11
	 
#define SNET_LIBRARY_VERSION		0.4
 
class sNET  {
public:
	 sNET(uint8_t numDevices, uint8_t rxPin=SNET_DEFAULT_RX_PIN, uint8_t txPin=SNET_DEFAULT_TX_PIN);
     ~sNET();

	 void begin();
     void processMessages();
	 AIRQBaseDevice *getDeviceForDeviceID(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4);
	 DataMessage *getMessageForDeviceID(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4);
	 void sendBroadcast(uint8_t *data, uint8_t len);
	 void sendToDevice(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint8_t type, uint8_t subtype, uint8_t *data, uint8_t len);
	 			 
private:
	 SoftwareSerial serial;
	 uint8_t allocatedDevices;
     uint8_t numDevices;
	 AIRQBaseDevice **devices;
	 
	 uint8_t readSNETMessage(SoftwareSerial *serial, __data_message *msg);
 };

#endif //__SNET_H