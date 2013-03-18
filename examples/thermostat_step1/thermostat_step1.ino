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

// This variabile sets the desired ambient temperature */
float desiredTemperature = 21.5;

//TMP36 Pin Variables
int sensorPin = 0;   //the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures

sNET snet(1);
AIRQ305 *board;

void setup()
{
  Serial.begin(19200);  //Start the serial connection with the computer
  //to view the result open the serial monitor
  snet.begin();
  analogReference(EXTERNAL);
  pinMode(4, OUTPUT);
}

float getTemperature() {
  float voltage = 0.0;

  // We average the temperature to obtain a more stable value
  for(int i=0; i<10; i++)
    voltage += analogRead(sensorPin) * (3.3/1024.0);
  voltage /= 10.0; 

  return (voltage - 0.5) * 100;
} 

void loop()
{
  /* sNET::processMessages() is responsibile to process messages coming from 
     AirQ Networks devices and to update corresponding device object (in this
     example, the AIRQ305 object). User code should call this method as soon
     as possible and continuosly */
  snet.processMessages();

  float tempC = getTemperature();
  // Let's give some feedback to the console
  Serial.print(tempC); Serial.println(" degrees C");

  // Now convert to Fahrenheight
  float tempF = tempC * (9.0 / 5.0) + 32.0;
  Serial.print(tempF); Serial.println(" degrees F");

  if((board = (AIRQ305*)snet.getDeviceForDeviceID(5,0,1,3)) != 0) {
    if(tempC > desiredTemperature + 0.5) /* Temperature raised too much: turn off bolier */
      board->setRELAY1(OFF);
    else if(tempC < desiredTemperature - 0.5) /* Temperature decreased too much: turn on bolier */
      board->setRELAY1(ON, false);

    if(board->getRELAY1())
      digitalWrite(4, HIGH);
    else
      digitalWrite(4, LOW);
  }

  delay(2000);
}


