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

#include <SoftwareSerial.h>
#include <sNET.h>
#include <LiquidCrystal.h>

float setTemp = 17.5; // This variabile sets the desired ambient temperature
char blank[] = "                "; // Clears a LCD line
char line1[16], line2[16];
uint8_t upBTN = 6;
uint8_t downBTN = 7;

sNET snet(2);
LiquidCrystal lcd(5, 4, 3, 2, 1, 0);

AIRQ305 *board;
AIRQ100 *sensor;

void lcdUpdate(char* line1, char *line2) {
  if(line1 != 0) {
    lcd.setCursor(0,0);
    lcd.print(blank);
    lcd.setCursor(0,0);
    lcd.print(line1);
  }  

  if(line2 != 0) {
    lcd.setCursor(0,1);
    lcd.print(blank);
    lcd.setCursor(0,1);
    lcd.print(line2);
  }  
}

/* Since Arduino sprintf lacks support for float conversion,
   this a convinient routine to convert a float to a string with
   a precision of 1 decimal */
char *float2char(float x) {
  static char str[5];
  int i;
  
  i = x*10; /* 1 digit precision */
  if(i> 0)  
    sprintf(str, "%d.%d", i/10, i%10);
  else
    sprintf(str, "%d.%d", i/10, -i%10);

  return str;
}

void setup()
{
  snet.begin();
  lcd.begin(16, 2);
  pinMode(upBTN, INPUT);
  pinMode(downBTN, INPUT);
  digitalWrite(upBTN, HIGH); /* Enables pull-up resistors on both pins */
  digitalWrite(downBTN, HIGH);

  /* We wait until both AIRQ 305 and AIRQ 100 are detected */  
  lcdUpdate("Waiting R1...", "Waiting sensor...");
  
  while((board = (AIRQ305*)snet.getDeviceForDeviceID(5,0,1,3)) == 0)    
    snet.processMessages();
  
  lcdUpdate("Connected!", "Waiting sensor...");
  
  while((sensor = (AIRQ100*)snet.getDeviceForDeviceID(101,2,1,0)) == 0)    
    snet.processMessages();
  
  lcdUpdate("Connected!", "Connected!");

}

void loop()
{
/* sNET::processMessages() is responsibile to process messages coming from 
   AirQ Networks devices and to update corresponding device object (in this
   example, the AIRQ305 and AIRQ100 objects). User code should call this 
   method as soon as possible and continuosly */
  snet.processMessages();
  delay(150);  //A little bit of debouncing

  if(!digitalRead(upBTN))
      setTemp += 0.1;
  if(!digitalRead(downBTN))
      setTemp -= 0.1;
      
  sprintf(line1, "Temp: %s", float2char(sensor->getTEMP()));
  sprintf(line2, "SET:%s - R1:%d", float2char(setTemp), board->getRELAY1());  
  lcdUpdate(line1, line2);
  
  if(sensor->getTEMP() > setTemp) /* Temperature raised too much: turn off bolier */
    board->setRELAY1(OFF);
  else if(sensor->getTEMP() < setTemp) /* Temperature decreased too much: turn on bolier */
    board->setRELAY1(ON, false);
}


