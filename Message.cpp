#include "Message.h"

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

AIRQ300DataMessage::AIRQ300DataMessage(__data_message &rawmessage) : DataMessage(rawmessage)
{
	
}

bool AIRQ300DataMessage::getIOStatus(uint8_t mask) {
	return ((getData()[0]) & mask) == mask;
}