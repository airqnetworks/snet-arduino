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

/* sNET 

/* To use sNET library we have to include sNET.h in our sketch */
#include <sNET.h>
/* sNET lib uses SoftwareSerial library. 
   Arduino IDE requires that we include the header file here */
#include <SoftwareSerial.h>

/* The following line creates a sNET object. This object will be used
   interact with other device. The integer parameter say to the library
   how many end devices we'll interface */
sNET snet(3);

void setup() {
  snet.begin();
}

void loop() {
  AIRQ310 *board;

  snet.processMessages();

  /* We ask the sNET object to give us the reference the device object
   * corresponding to 3.0.1.2 board. The method sNET::getDeviceForDeviceID()
   * will return a pointer to an AIR310 object if the object was already created, 
   * otherwise it returns 0 (NULL). The device object is created as soon as 
   * sNET library captures a message coming from that device */   
  if((board = (AIRQ310 *)snet.getDeviceForDeviceID(3,0,1,2)) != 0) {
     /* Ok, the AIRQ310 object was created and we can turn RELAY1 on */
     board->setRELAY1(ON);
     while(1); /* Job done. We stop here */
  }
}