/**
 * Software License Agreement
 *
 * (C)Copyright 2013, AirQ Networks s.r.l. (http://www.airqnetworks.com)
 * All rights reserved.
 *
 * AirQ Networks licenses to you the right to use, modify, copy, and
 * distribute this software/library when used in conjunction with an 
 * AirQ Networks transceiver to interface AirQ Networks wireless devices
 * (transceivers, sensors, control sensors and other devices produced 
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

/* To use sNET library we have to include sNET.h in our sketch */
#include <sNET.h>
/* sNET lib uses SoftwareSerial library. 
   Arduino IDE requires that we include the header file here */
#include <SoftwareSerial.h>

/* The following line creates a sNET object. This object will be used
   interact with other device. The integer parameter say to the library
   how many end devices we'll interface */
sNET snet(1);

/* Objects instance of AIRQ101 class allow to interact with an AirQ 101
   wireless temperature sensor. */
AIRQ101 *sensor;

void setup() {
  /* Call the sNET::begin() method is required to inizialize the sNET library */
  snet.begin();
  Serial.begin(19200);
}

void loop() {
  /* sNET::processMessages() is responsibile to process messages coming from 
     AirQ Networks devices and to update corresponding device object (in this
     example, the AIRQ101 object). User code should call this method as soon
     as possible and continuosly */
  snet.processMessages();

  /* We ask the sNET object to give us the reference to the device object
   * corresponding to 101.2.1.0 sensor. The method sNET::getDeviceForDeviceID()
   * will return a pointer to an AIR101 object if the object was already created, 
   * otherwise it returns 0 (NULL). The device object is created as soon as 
   * sNET library captures a message coming from that device */   
  if((sensor = (AIRQ101 *)snet.getDeviceForDeviceID(101,2,1,0)) != 0) {
     /* Ok, the AIRQ101 object was created and we can see detected temperatire */
     if(sensor->getLastMessage()->updated()) { 
       /* updated() method returns true every time the sensors sends a new message */
       Serial.print("Detected temperature: "); Serial.print(sensor->getTEMP()); Serial.println(" degrees C");
       Serial.print("Battery voltage: "); Serial.print(sensor->getBATT()); Serial.println(" mV");
     }
  }
}
