#include <sNET.h>
#include <SoftwareSerial.h>

sNET snet(2);
AIRQ300DataMessage *msg;
AIRQBaseDevice *board;
AIRQ300 *board2;
uint8_t data;
uint8_t oldpkt = 0;
int ledPin = 12;

void setup() {
  pinMode(ledPin, OUTPUT); 
  Serial.begin(19200);
  Serial.println("OEKEIO");
}

void loop() {
  char *data = "CIAOCIAOCIAOCIAO";
//  delay(1000);    
  snet.processMessages();
  snet.sendToDevice(4,0,0,0, 1, (uint8_t*)data, 1);
  //snet.sendBroadcast((uint8_t*)data, 4);
//  //Serial.println("MAINDO");
  digitalWrite(ledPin,!digitalRead(ledPin));
 delay(1000);
}
//  while(1);

//////  while(1) {
//////    data = data ? 0x0 : 0x10;
//////    snet.sendToDevice(3,0,0,0,0x2, &data, 1);
//////    delay(7000);
//////  }
//  if((board = snet.getDeviceForDeviceID(191,0,0,0xa)) != 0) {
//    if(board->getLastMessage()->updated()) {
//      for(int i = 0; i < board->getLastMessage()->getDataLen(); i++)
//        Serial.print(board->getLastMessage()->getData()[i]);
//      Serial.println("");
//      snet.sendToDevice(191,0,0,0xA, 1, board->getLastMessage()->getData(), board->getLastMessage()->getDataLen());
//    }
//  }
////  if((board2 = (AIRQ300 *)snet.getDeviceForDeviceID(3,0,0,0)) != 0) {
////      board2->setRELAY1((IO_STATUS)!board2->getRELAY1(), false);
////  }
//}
//      board->setRELAY1((IO_STATUS)!board->getRELAY1(), false);
//  }
//  if((board2 = (AIRQ310 *)snet.getDeviceForDeviceID(4,0,0,0)) != 0) {
//      board2->setRELAY6((IO_STATUS)!board2->getRELAY6(), false);
//  }

//}
////      Serial.print("R1: ");
////      Serial.println(board->getRELAY1());
//      board->setRELAY1((IO_STATUS)!board->getRELAY1(), false);
//      board->setRELAY2((IO_STATUS)!board->getRELAY2(), false);
      
//    }
//  }
//  if((board = (AIRQ300 *)snet.getDeviceForDeviceID(4,0,0,0)) != 0) {
//    if(1) {
////      Serial.print("R1: ");
////      Serial.println(board->getRELAY1());
//      board->setRELAY1((IO_STATUS)!board->getRELAY1(), true );
//      
//    }
//  }
