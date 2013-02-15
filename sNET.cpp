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
	 
#include <Arduino.h>
#include <sNET.h>

#define defaultRXPin 10
#define defaultTXPin 11

sNET::sNET() : serial(defaultRXPin, defaultTXPin) {
	init(defaultRXPin, defaultTXPin);
}

sNET::sNET(uint8_t rxPin, uint8_t txPin) : serial(rxPin, txPin) {
	init(rxPin, txPin);
}

sNET::~sNET() {

}

uint8_t sNET::readSNETMessage(SoftwareSerial *serial, __data_message *message)
{
	uint8_t i = 0;
	uint8_t buf[SNET_MAX_MESSAGE_SIZE];
	
	while(serial->available()) {
		buf[i++] = serial->read();
		if(i > SNET_MAX_MESSAGE_SIZE) {
			Serial.println("WACOOOOOOOOOO");
			/* Message is invalid, we flush serial */
			serial->flush(); 
			return 0;
		}
		if(i > 0 && buf[i-1] == '\n' && buf[i-2] == '$') { /* EOL detected */
			if(i > 19 && memcmp(buf, "AIRQ", 4) == 0) { /* Minimun sNET message is 19 byte long */
				/* Ok, it's a valid sNET message */
				memcpy(message, buf, i-2); /* Let's copy buf to message: EOL sequence isn't copied */
				return i-1;
			} 
			return 0;
		}
	}
}

void sNET::init(uint8_t rxPin, uint8_t txPin) {
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	
	serial.begin(SNET_SERIAL_BAUDRATE);	
}

void sNET::processMessages() {
	__data_message message;
	
	while(serial.available())
		if (readSNETMessage(&serial, &message)) {
			DataMessage msg(message);
			Serial.println(msg.getDeviceID()); 
		}
}

DataMessage::DataMessage(__data_message &rawmessage) : rawmessage(rawmessage) {
	
}

int DataMessage::getDeviceID() {
	return rawmessage.devid[0];
}