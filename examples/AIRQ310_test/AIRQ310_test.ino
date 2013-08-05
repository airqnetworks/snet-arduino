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

/* To use sNET library we have to include sNET.h in our sketch */
#include <sNET.h>
/* sNET lib uses SPI library. 
   Arduino IDE requires that we include the header file here */
#include <SPI.h>

/* Objects instance of AIRQ305 class allow to interact with an AirQ 305 board. */
AIRQ310 *board;

void setup() {
  /* Call the sNET.begin() method is required to inizialize the sNET library 
     The integer parameter say to the library how many end devices we'll interface */
  sNET.begin(1);
}

void loop() {
  /* We ask the sNET object to give us the reference to the device object
   * corresponding to 4.0.1.0 board. The method sNET::getDeviceForDeviceID()
   * will return a pointer to an AIR305 object if the object was already created, 
   * otherwise it returns 0 (NULL). The device object is created as soon as 
   * sNET library captures a message coming from that device */   
  if((board = (AIRQ310 *)sNET.getDeviceForDeviceID(4,0,1,0)) != 0) {
     /* Ok, the AIRQ310 object was created and we can turn all relays on */
     board->setRELAY1(ON);
     board->setRELAY2(ON);
     board->setRELAY3(ON);
     board->setRELAY4(ON);
     board->setRELAY5(ON);
     board->setRELAY6(ON);
     delay(1000);
     /* Let's turn them off */     
     board->setRELAY6(OFF);
     board->setRELAY5(OFF);
     board->setRELAY4(OFF);
     board->setRELAY3(OFF);
     board->setRELAY2(OFF);
     board->setRELAY1(OFF);
     delay(1000);
     /* Now, let's test pulse */
     board->pulseRELAY1();
     board->pulseRELAY2();
     board->pulseRELAY3();
     board->pulseRELAY4();
     board->pulseRELAY5();
     board->pulseRELAY6();
     while(1); /* Job done. We stop here */
  }
}