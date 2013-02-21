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
	 
#include "sNET.h"
#include "Device.h"

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
}

void AIRQControlBoard::sendSetMessage(uint8_t *data, uint8_t len) {
	uint8_t *addr = getDeviceID();
	snet->sendToDevice(addr[0], addr[1], addr[2], addr[3], 0x2, data, len);
	delay(50);
}

#ifdef SNET_ENABLE_CONFIRM

void AIRQControlBoard::setIO(uint8_t *data, uint8_t len, bool check, uint8_t timeout) {
	sendSetMessage(data, len);
	delay(250);

	if(check) {
		uint8_t times = 0;
		snet->processMessages();
		uint8_t *cdata = status->getData();
		while(memcmp(data, cdata, len) != 0) {
			if(status->updated()) {
				sendSetMessage(data, 1);
				times++;
			}
			snet->processMessages();
			cdata = status->getData();
			if(timeout >= 0 and (times >= timeout))
				return;
		}
	}
}

#else

void AIRQControlBoard::setIO(uint8_t *data, uint8_t len) {
	sendSetMessage(data, len);
}
#endif


bool AIRQControlBoard::statusChanged() {
	bool changed = false;
	if(lstatus != status->getData()[0]) {
		lstatus = status->getData()[0];
		changed = true;
	}
	return changed;
}

AIRQ300::AIRQ300(DataMessage *message) :
 AIRQControlBoard(message) {
	 
}

void AIRQ300::setRELAY1(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x10;
	else
		data &= 0xFF ^0x10;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}

void AIRQ300::setRELAY2(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x20;
	else
		data &= 0xFF ^ 0x20;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}


AIRQ310::AIRQ310(DataMessage *message) :
 AIRQControlBoard(message) {
	 
}


void AIRQ310::setRELAY1(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x1;
	else
		data &= 0xFF ^0x1;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}

void AIRQ310::setRELAY2(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x2;
	else
		data &= 0xFF ^ 0x2;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}


void AIRQ310::setRELAY3(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x4;
	else
		data &= 0xFF ^ 0x4;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}


void AIRQ310::setRELAY4(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x8;
	else
		data &= 0xFF ^ 0x8;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}


void AIRQ310::setRELAY5(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x10;
	else
		data &= 0xFF ^ 0x10;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}


void AIRQ310::setRELAY6(IO_STATUS rstatus, bool check, uint8_t timeout) {
	uint8_t data = status->getData()[0];
	
	if(rstatus)
		data |= 0x20;
	else
		data &= 0xFF ^ 0x20;

#ifdef SNET_ENABLE_CONFIRM	
	setIO(&data, 1, check, timeout);
#else
	setIO(&data, 1);
#endif //SNET_ENABLE_CONFIRM
}