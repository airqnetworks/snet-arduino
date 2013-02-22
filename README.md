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

asdasd
##Requirements
This library requires an AirQ ShielD. It was tested under Arduino IDE 1.5.x.


#How it works


#Usage
The library was designed to minimize the use of both Arduino RAM and ROM memory. This is translated in just one requirement: developer must know how many AirQ Networks devices will be 

Let's suppose we want to interface two AirQ Networks 
```cpp
sNET snet(2);
```