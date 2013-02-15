#include <sNET.h>
#include <SoftwareSerial.h>

sNET snet(2);
AIRQ300DataMessage *msg;
AIRQ300 *board;
uint8_t data;
uint8_t oldpkt = 0;

void setup() {
  Serial.begin(19200);
  Serial.println("OEKEIO");
  snet.getMessageForDeviceID(4,0,0,0);
}

void loop() {
  snet.processMessages();
  if((board = (AIRQ300 *)snet.getDeviceForDeviceID(3,0,1,0)) != 0) {
    if(board->statusChanged()) {
      Serial.print("IN1: ");
      Serial.println(board->getIN1());
    }
  }
}


