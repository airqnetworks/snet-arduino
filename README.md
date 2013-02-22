sNET library for Arduino platform - &copy;AirQ Networks s.r.l. 2013

# About
With the AirQ ShielD, this library allows an Arduino board to interact with AirQ Networks devices(wireless sensors and control boards). Thanks to this library, there is no need to know the underlying sNET protocol (sNET is the protocol used by AirQ Networks devices to exchange messages and it's described on the AirQ Networks technical wiki -- http://wiki.airqnetworks.com/index.php/SNET_Protocol).

Arduino communicates with the shield using SoftwareSerial library. This means that AirQ ShielD interacts with arduino through an UART interface. Digital pins **10** and **11** of the Uno board are used. However, library can be set up to use different pins if needed.

#Installation
sNET library need to be installed before using it. First step is download the library from this repository. Next, open your Arduino sketchbook folder. If there is already a folder there called libraries, place the library folder in there. If not, create a folder called libraries in the sketchbook folder, and drop the library folder in there. Then re-start the Arduino programming environment, and you should see your new library in the Sketch > Import Library menu.
To download the library from this repository you can use one of the following ways:
* **Using git** (preferred way.). Download the library using git. This will allow you to keep updated with the main repository every time a new release is pushed. To download the library, simply run the command:

	```bash
	$ git clone https://github.com/airqnetworks/snet-arduino.git sNET
	```

	and then copy the sNET folder into library subfolder of your sketchbook folder.
* **Downloading the zip**. Click on the following link to download the whole library as a single zip file: 

	https://github.com/airqnetworks/snet-arduino/archive/master.zip


##Requirements
This library requires an AirQ ShielD. It was tested under Arduino IDE 1.5.x.


#How it works
sNET library does a really simple task: it translates every message coming from a wireless device (sensor or control board) to a "high level" structure. Arduino programmes can use these structures to interact with devices. For each "physical" device (eg. an AirQ 310 control board) a corresponding device object is created (following the previuos example, an object instance of the <code>AIRQ310</code> object is created). Moreover, for each "message" generated by the device, a corresponding data message is created (<code>AIRQ310DataMessage</code> for the AirQ 310 control board.) 

The following picture clearly shows how it works.

<img width="50%" src="http://wiki.airqnetworks.com/images/5/50/Shield-messages.jpg" />

The picture shows this scenario: three AirQ 310 control boards are used; the AirQ ShielD on the Arduino Uno board collects messages coming from control boards. For each control board, the sNET library generate an AIRQ310 object, and for each message the corresponding message is created.

sNET library was designed to minimize usage of both Arduino RAM and ROM memory. This means that:

+ *There is no queuing mechanism of received messages in sNET*. Only the last received message is available for a given device. 
+ Programmes have to specify the number of remote devices on the same sensor networks that will interact with the shield (and so with the library).
 
All the dirty job is done by the <code>sNET</code> class. Instances of this class are responsibile to:

+ Generate instances of corrsponding *device objects* for each detected wireless device;
+ Process messages generated on the UART and update the corresponding device oject.

Moreover, since messages exchanged between devices are completely asynchronous, messages generated on the shield UART are asynchronous too. Programmers have to ensure that these messages aren't lost allowing the sNET library to process them on the UART. This job is done by the <code>sNET::processMessages()</code> method and it should be called cyclically during the application lifecycle.

#Usage

Let's suppose we want to interface the three AirQ 310 control boards of the above picture. First, we have to instantiate a new <code>sNET</code> object, giving as argument the number of devices connected to the shield (and so, the number *3*).

```cpp
#include <sNET.h>
/* sNET lib uses SoftwareSerial library. Arduino IDE requires that we include the header file here */
#include <SoftwareSerial.h> 
sNET snet(3);
```

Next, we have to call the <code>sNET::begin()</code> method. This method does all the required stuff to propert configure the library. The <code>setup()</code> function inside the main sketch is the best place to call it.
```cpp

void setup() {
  snet.begin();
}
```
Next, as described in the previous paragraph, we have to cyclically call the <code>sNET::processMessages()</code> method, which is responsibile to process messages generated on the shield UART and to crate the corresponding device objects. The <code>lopp()</code> function inside the main sketch is the best place to call it.
```cpp

void loop() {
  snet.processMessages();
  ...
}
```
Now, let's suppose that we want to turn on the RELAY1 of the second AirQ 310 board (the one with network address 3.0.1.2). sNET library allows to do this in a really simple way:
```cpp
void loop() {
  AIRQ310 *board;

  snet.processMessages();

  /* We ask the sNET object to give us the reference the device object
   * corresponding to 3.0.1.2 board. The method sNET::getDeviceForDeviceID()
   * will return a pointer to an AIR310 object if the object was alredy created, 
   * otherwise it return 0 (NULL). The device object is created as soon as sNET library
   * captures a message coming from that device */   
  if((board = (AIRQ310 *)snet.getDeviceForDeviceID(3,0,1,2)) != 0) {
     /* Ok, the AIRQ310 object was created and we can turn RELAY1 on */
     board->setRELAY1(ON);
     while(1); /* Job done. We stop here */
  }
}
```