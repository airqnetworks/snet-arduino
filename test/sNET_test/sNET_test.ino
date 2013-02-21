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

#include <sNET.h>
#include <SoftwareSerial.h>

/* The following line creates a sNET object. This object will be used
   interact with other device. The integer parameter say to the library
   how many end devices we'll interface */
sNET snet(1);
AIRQ310 *board;

void setup() {
  snet.begin();
}

void loop() {
  snet.processMessages();

  if((board = (AIRQ310 *)snet.getDeviceForDeviceID(4,0,0,0)) != 0) {
    if(board->statusChanged()) { 
      board->setRELAY1((IO_STATUS)!board->getRELAY1(), true);
      delay(1000);
    }
  }
}
