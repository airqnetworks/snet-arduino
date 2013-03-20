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


uint8_t buf[SNET_MAX_DATAMESSAGE_SIZE];
 
sNET::sNET(uint8_t numDevices, uint8_t rxPin, uint8_t txPin) : serial(rxPin, txPin), numDevices(numDevices) {
 	pinMode(rxPin, INPUT);
 	pinMode(txPin, OUTPUT);
}

sNET::~sNET() {
	for(uint8_t i = 0; i < numDevices; i++)
		delete devices[i];
}

AIRQBaseDevice *sNET::getDeviceForDeviceID(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4)
{
	uint8_t addr[] = {octet1, octet2, octet3, octet4};

	for(uint8_t i = 0; i < allocatedDevices; i++)
		if(memcmp(devices[i]->getDeviceID(), addr, SNET_DEV_ADDR_LEN) == 0)
			return devices[i];

	return 0;	
}


DataMessage *sNET::getMessageForDeviceID(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4)
{
	uint8_t addr[] = {octet1, octet2, octet3, octet4};
	
	for(uint8_t i = 0; i < allocatedDevices; i++)
		if(memcmp(devices[i]->getDeviceID(), addr, SNET_DEV_ADDR_LEN) == 0)
			return devices[i]->status;

	return 0;	
}

uint8_t sNET::readSNETMessage(SoftwareSerial *serial, __data_message *message)
{
	uint8_t i = 0;
	while(serial->available()) {
		buf[i++] = serial->read();
		if(i > SNET_MAX_DATAMESSAGE_SIZE) {
			/* Message is invalid, we flush serial */
			memset(buf, 0, sizeof(__data_message));	
			serial->flush(); 
			return 0;
		}
		if(i > 0 && buf[i-1] == '\n' && buf[i-2] == '$') { /* EOL detected */
			if(i > 19 && memcmp(buf, "AIRQ", 4) == 0) { /* Minimun sNET message is 19 byte long */
				/* Ok, it's a valid sNET message */
				memcpy(message, buf, i-2); /* Let's copy buf to message: EOL sequence isn't copied */
				message->datalen = i-20;
				memset(buf, 0, sizeof(__data_message));	
				return i-1;
			} 
			return 0;
		}
	}
}

void sNET::begin() {
	__data_message nullmsg;
	memset(&nullmsg, 0, sizeof(__data_message));
	
	devices = new AIRQBaseDevice*[numDevices];
	
	serial.begin(SNET_SERIAL_BAUDRATE);	
	serial.write("$\n");
	randomSeed(analogRead(0));
	delay(1000);
}

void sNET::processMessages() {
	__data_message message;
	bool found = false;
	
	while(serial.available())
		if (readSNETMessage(&serial, &message)) {
			for(uint8_t i = 0; i < allocatedDevices; i++) {
				if(memcmp(devices[i]->getDeviceID(), message.devid, SNET_DEV_ADDR_LEN) == 0){
					devices[i]->updateFromRawMessage(&message);
					found = true;
				}
			}
			if(not found and allocatedDevices < numDevices) {
				switch(message.devid[0]) {
				case 3:
					devices[allocatedDevices] = new AIRQ300(new AIRQ300DataMessage(message));
					break;
				case 4:
					devices[allocatedDevices] = new AIRQ310(new AIRQ310DataMessage(message));
					break;
				case 5:
					devices[allocatedDevices] = new AIRQ305(new AIRQ305DataMessage(message));
					break;
				case 191:
					devices[allocatedDevices] = new AIRQBaseDevice(new DataMessage(message));
					break;
				default: /* Unknown message */
					return;					
				}
				devices[allocatedDevices]->setSNETReference(this);
				devices[allocatedDevices++]->getLastMessage()->setSNETReference(this);
			}
		} 
		
}

void sNET::sendBroadcast(uint8_t *data, uint8_t len) {
	uint8_t message[SNET_MAX_SETMESSAGE_SIZE] = {'A', 'T', 'S', 0xF, 0x1};
	
	if(len > 10)
		return;

	memcpy(message+5, data, len);
	memcpy(message+5+len, "$\n", 2);
	serial.flush();
	serial.write(message, 7+len);
	serial.flush();
	delay(60);	
}

void sNET::sendToDevice(uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint8_t type, uint8_t subtype, uint8_t *data, uint8_t len) {
	uint8_t message[SNET_MAX_SETMESSAGE_SIZE] = {'A', 'T', 'S', type, subtype, octet1, octet2, octet3, octet4};

	if(len > 10)
		return;

	memcpy(message+9, data, len);
	memcpy(message+9+len, "$\n", 2);
	serial.flush();
	serial.write(message, 11+len);
	serial.flush();
	delay(100);
}