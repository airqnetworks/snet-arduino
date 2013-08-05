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
	 
#include "sNET.h"
#include "Device.h"

extern _sNET sNET;

AIRQBaseDevice::AIRQBaseDevice(DataMessage *message) {
	status = message;
} 

AIRQBaseDevice::~AIRQBaseDevice() {
	delete status;
}

uint8_t *AIRQBaseDevice::getDeviceID() {
	return status->getDeviceID();
}

DataMessage *AIRQBaseDevice::getLastMessage()
{
	return status;
	
}
uint8_t AIRQBaseDevice::getLQI() {
	return status->getLQI();
}

int8_t AIRQBaseDevice::getRSSI() {
	return status->getRSSI();
}

void AIRQBaseDevice::updateFromRawMessage(__data_message *message) {
	status->updateFromRawMessage(message);
}


AIRQControlBoard::AIRQControlBoard(DataMessage *message) :
 AIRQBaseDevice(message) {
	 lstatus = 0xFF; /* The first time statusChanged return always true */
	 lconftoken = 0;
	 RELAY_XOR_MASK = 0xFF;
}

void AIRQControlBoard::sendSetMessage(uint8_t subtype, uint8_t *data, uint8_t len, bool confirm) {
	uint8_t *addr = getDeviceID();
	if(confirm) {
		uint8_t conft = 0;
		while(conft == lconftoken || conft == 0)
			conft = random(1, 254);
		lconftoken = conft;
		uint8_t dly = 1;
		while(true) {
			if(++dly >= 10)
				dly = 2;
			sNET.sendToDevice(addr[0], addr[1], addr[2], addr[3], 0x42, subtype, conft, data, len);
#ifndef SNET_USE_INTERRUPTS
			sNET.processMessages(true);
#endif
			delay(1000*dly);
			if(status->updated()) {
				if (status->getConfToken() == conft) {
					return;
				}
			}
		}
	}
	else
		sNET.sendToDevice(addr[0], addr[1], addr[2], addr[3], 0x2, subtype, 0, data, len);
}


void AIRQControlBoard::setIO(uint8_t subtype, uint8_t *data, uint8_t len) {
	sendSetMessage(subtype, data, len, true);
}


void AIRQControlBoard::setRELAY(uint8_t rmask, IO_STATUS rstatus, uint8_t duration) {
	if(duration > 0) {
		uint8_t data[] = {rmask, duration};
		setIO(0x2, data, 2);
		return;
	}

	uint8_t data = status->getData()[0] & RELAY_XOR_MASK;
	
	if(rstatus)
		data |= rmask;
	else
		data &= 0xFF ^rmask;

	if(data == (status->getData()[0] & RELAY_XOR_MASK))
		return;

	setIO(0x1, &data, 1);
}


bool AIRQControlBoard::statusChanged() {
	bool changed = false;
	if(lstatus != status->getData()[0]) {
		lstatus = status->getData()[0];
		changed = true;
	}
	return changed;
}

/*
 * AirQ 300
 *
 */
AIRQ300::AIRQ300(DataMessage *message) :
 AIRQControlBoard(message) {
	 RELAY_XOR_MASK = 0x30;
}

void AIRQ300::setRELAY1(IO_STATUS rstatus) {
	setRELAY(AIRQ300_RELAY1_MASK, rstatus, 0);
}

void AIRQ300::setRELAY2(IO_STATUS rstatus) {
	setRELAY(AIRQ300_RELAY2_MASK, rstatus, 0);
}

/*
 * AirQ 305
 *
 */
AIRQ305::AIRQ305(DataMessage *message) :
 AIRQControlBoard(message) {
	 RELAY_XOR_MASK = 0xF;
}

void AIRQ305::pulseRELAY1() {
	setRELAY(AIRQ305_PULSE_RELAY1_MASK, ON, 0);
}

void AIRQ305::pulseRELAY2() {
	setRELAY(AIRQ305_PULSE_RELAY2_MASK, ON, 0);
}

void AIRQ305::pulseRELAY3() {
	setRELAY(AIRQ305_PULSE_RELAY3_MASK, ON, 0);
}

void AIRQ305::pulseRELAY4() {
	setRELAY(AIRQ305_PULSE_RELAY4_MASK, ON, 0);
}

void AIRQ305::setRELAY1(IO_STATUS rstatus, uint8_t duration) {
	setRELAY(AIRQ305_RELAY1_MASK, rstatus, duration);
}

void AIRQ305::setRELAY2(IO_STATUS rstatus, uint8_t duration) {
	setRELAY(AIRQ305_RELAY2_MASK, rstatus, duration);
}

void AIRQ305::setRELAY3(IO_STATUS rstatus, uint8_t duration) {
	setRELAY(AIRQ305_RELAY3_MASK, rstatus, duration);
}

void AIRQ305::setRELAY4(IO_STATUS rstatus, uint8_t duration) {
	setRELAY(AIRQ305_RELAY4_MASK, rstatus, duration);
}

/*
 * AirQ 310
 *
 */
AIRQ310::AIRQ310(DataMessage *message) :
 AIRQControlBoard(message) {
	 RELAY_XOR_MASK = 0x3F;	 
}


void AIRQ310::pulseRELAY1() {
	setRELAY(AIRQ310_PULSE_RELAY1_MASK, ON, 0);
}

void AIRQ310::pulseRELAY2() {
	setRELAY(AIRQ310_PULSE_RELAY2_MASK, ON, 0);
}

void AIRQ310::pulseRELAY3() {
	setRELAY(AIRQ310_PULSE_RELAY3_MASK, ON, 0);
}

void AIRQ310::pulseRELAY4() {
	setRELAY(AIRQ310_PULSE_RELAY4_MASK, ON, 0);
}

void AIRQ310::pulseRELAY5() {
	setRELAY(AIRQ310_PULSE_RELAY5_MASK, ON, 0);
}

void AIRQ310::pulseRELAY6() {
	setRELAY(AIRQ310_PULSE_RELAY6_MASK, ON, 0);
}

void AIRQ310::setRELAY1(IO_STATUS rstatus) {
	setRELAY(AIRQ310_RELAY1_MASK, rstatus, 0);
}

void AIRQ310::setRELAY2(IO_STATUS rstatus) {
	setRELAY(AIRQ310_RELAY2_MASK, rstatus, 0);
}


void AIRQ310::setRELAY3(IO_STATUS rstatus) {
	setRELAY(AIRQ310_RELAY3_MASK, rstatus, 0);
}


void AIRQ310::setRELAY4(IO_STATUS rstatus) {
	setRELAY(AIRQ310_RELAY4_MASK, rstatus, 0);
}


void AIRQ310::setRELAY5(IO_STATUS rstatus) {
	setRELAY(AIRQ310_RELAY5_MASK, rstatus, 0);
}


void AIRQ310::setRELAY6(IO_STATUS rstatus) {
	setRELAY(AIRQ310_RELAY6_MASK, rstatus, 0);
}