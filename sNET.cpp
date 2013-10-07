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
	 

#include <Arduino.h>
#include "sNET.h"
#include "Message.h"



void processInterrupts() {
	sNET._processMessages();
}
 
_sNET::_sNET()  {
}

_sNET::~_sNET() {
	for(uint8_t i = 0; i < numDevices; i++)
		delete devices[i];
}

AIRQBaseDevice *_sNET::getDeviceForDeviceID(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4)
{
	uint8_t addr[] = {octet1, octet2, octet3, octet4};

	for(uint8_t i = 0; i < allocatedDevices; i++)
		if(memcmp(devices[i]->getDeviceID(), addr, SNET_DEV_ADDR_LEN) == 0)
			return devices[i];

	return 0;	
}


DataMessage *_sNET::getMessageForDeviceID(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4)
{
	uint8_t addr[] = {octet1, octet2, octet3, octet4};
	
	for(uint8_t i = 0; i < allocatedDevices; i++)
		if(memcmp(devices[i]->getDeviceID(), addr, SNET_DEV_ADDR_LEN) == 0)
			return devices[i]->status;

	return 0;	
}

uint8_t _sNET::SPIRead(__data_message* message) {
	uint8_t *data = (uint8_t*)message;
	uint8_t len = 0;
	
	digitalWrite(SSN, LOW);
	delay(1);
	len = SPI.transfer(0x0) - 1;
	for(int i = 0; i < len; i++)
		data[i] = SPI.transfer(0x0);
	digitalWrite(SSN, HIGH);	
	
	return len;
}

void _sNET::SPIWrite(uint8_t *message, uint8_t len) {
	uint8_t buf[SNET_MAX_MESSAGE_SIZE];	
	memset(buf, 0, SNET_MAX_MESSAGE_SIZE);
	memcpy(buf, message, len);

	while(digitalRead(SNET_DEFAULT_DRDY_PIN) == LOW)
		processMessages();

	digitalWrite(SSN, LOW);
	delay(1);
	for(int i = 0; i < SNET_MAX_MESSAGE_SIZE; i++) {
		SPI.transfer(buf[i]);
	}
	digitalWrite(SSN, HIGH);
}

void _sNET::reset() {
	SPIWrite((uint8_t*)"ATR$\n", strlen("ATR$\n"));	
}

void _sNET::begin(uint8_t numdevices, uint8_t ssn) {
	__data_message nullmsg;
	numDevices = numdevices;
	
	SPI.begin();
	SPI.setDataMode(SPI_MODE0); 
    SPI.setClockDivider(SPI_CLOCK_DIV8);
	
	pinMode(SNET_DEFAULT_DRDY_PIN, INPUT_PULLUP);

#ifdef SNET_USE_INTERRUPTS
	attachInterrupt(0, processInterrupts, FALLING);
#endif
	
	SSN = ssn;
	pinMode(SSN, OUTPUT);
	memset(&nullmsg, 0, sizeof(__data_message));
	
	devices = new AIRQBaseDevice*[numDevices];
	
	randomSeed(analogRead(0));
	reset();
}

void _sNET::processMessages(bool wait) { 
#ifndef SNET_USE_INTERRUPTS
	if(digitalRead(SNET_DEFAULT_DRDY_PIN) == LOW) {
		_processMessages();
		return;
	}

	while(digitalRead(SNET_DEFAULT_DRDY_PIN) == HIGH)
		if(!wait)
			break;
	
	if(digitalRead(SNET_DEFAULT_DRDY_PIN) == LOW)
		_processMessages();
#endif
}

void _sNET::_processMessages() {
	__data_message message;
	bool found = false;

	if (SPIRead(&message) > 0) {
		for(uint8_t i = 0; i < allocatedDevices; i++) {
			if(memcmp(devices[i]->getDeviceID(), message.devid, SNET_DEV_ADDR_LEN) == 0){
				devices[i]->updateFromRawMessage(&message);
				found = true;
			}
		}
		
		if(not found and allocatedDevices < numDevices) {
			switch(message.devid[0]) {
			case 3:
				devices[allocatedDevices++] = new AIRQ300(new AIRQ300DataMessage(message));
				break;
			case 4:
				devices[allocatedDevices++] = new AIRQ310(new AIRQ310DataMessage(message));
				break;
			case 5:
				devices[allocatedDevices++] = new AIRQ305(new AIRQ305DataMessage(message));
				break;
			case 100:
				devices[allocatedDevices++] = new AIRQ100(new AIRQ100DataMessage(message));
				break;
			case 101:
				devices[allocatedDevices++] = new AIRQ101(new AIRQ101DataMessage(message));
				break;
			case 191:
				devices[allocatedDevices++] = new AIRQBaseDevice(new DataMessage(message));
				break;
			default: /* Unknown message */
				return;					
			}
		}
	}
}

void _sNET::sendBroadcast(uint8_t *data, uint8_t len) {
	uint8_t message[SNET_MAX_MESSAGE_SIZE] = {'A', 'T', 'S', 0xF, 0x1};
	
	if(len > 10)
		return;

	memcpy(message+5, data, len);
	memcpy(message+5+len, "$\n", 2);
	delay(60);	
}

void _sNET::sendToDevice(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint8_t type, uint8_t subtype, uint8_t conft, uint8_t *data, uint8_t len) {
	uint8_t message[SNET_MAX_MESSAGE_SIZE] = {'A', 'T', 'S', type, subtype, conft, octet1, octet2, octet3, octet4};

	if(len > 10)
		return;

	memcpy(message+10, data, len);
	memcpy(message+10+len, "$\n", 2);
	SPIWrite(message, 12+len);
	delay(100);
}

_sNET sNET;