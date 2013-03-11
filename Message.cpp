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

#include <Arduino.h>
#include "Message.h"

bool CHECK_MASK(uint8_t data, uint8_t mask) {
	return (data & mask) == mask;
}

DataMessage::DataMessage(__data_message &rawmessage) : rawmessage(rawmessage), umessage(true) {
	
}

DataMessage::DataMessage() {
	memset(&rawmessage, 0, sizeof(__data_message));
}

uint8_t *DataMessage::getData()
{
	return rawmessage.data;
}

uint8_t *DataMessage::getDeviceID() {
	return rawmessage.devid;
}

uint8_t DataMessage::getLQI() {
	return rawmessage.LQI;
}

uint8_t DataMessage::getPacketNumber() {
	return rawmessage.pkt;
}

uint8_t *DataMessage::getReceiverID() {
	return rawmessage.rcvid;
}

int8_t DataMessage::getRSSI() {
	return rawmessage.RSSI;
}

uint8_t DataMessage::getSubType() {
	return rawmessage.subtype;
}

uint8_t DataMessage::getType() {
	return rawmessage.type;
}

void DataMessage::updateFromRawMessage(__data_message *message) {
	memcpy(&rawmessage, message, sizeof(__data_message));
	umessage = true;
}

bool DataMessage::updated() {
	if(umessage) {
		umessage = false;
		return true;
	}
	return false;
}

AIRQ300DataMessage::AIRQ300DataMessage(__data_message &rawmessage) : DataMessage(rawmessage) {
	
}

bool AIRQ300DataMessage::getIOStatus(uint8_t mask) {
	return CHECK_MASK(getData()[0], mask);
}

AIRQ305DataMessage::AIRQ305DataMessage(__data_message &rawmessage) : DataMessage(rawmessage) {
	
}

bool AIRQ305DataMessage::getIOStatus(uint8_t mask) {
	return CHECK_MASK(getData()[0], mask);
}

AIRQ310DataMessage::AIRQ310DataMessage(__data_message &rawmessage) : DataMessage(rawmessage) {
	
}

bool AIRQ310DataMessage::getIOStatus(uint8_t mask) {
	return CHECK_MASK(getData()[0], mask);
}

