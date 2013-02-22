sNET library for Arduino platform - &copy;AirQ Networks s.r.l. 2013

# About
With the AirQ ShielD, this library allows an Arduino board to interact with AirQ Networks devices(wireless sensors and control boards). Thanks to this library, there is no need to know the underlying sNET protocol. This protocol is used by AirQ Networks devices to exchange messages and it's described on the AirQ Networks technical wiki (wiki.airqnetworks.com/index.php/SNET_Protocol).
Arduino communicates with the shield using SoftwareSerial library. This means that AirQ ShielD interacts with arduino through an UART interface. Digital pins 10 and 11 of the Uno board are used. However, library can be set up to use different pins.

#Installation
The library can simply installed under the user 

##Requirements
This library requires an AirQ ShielD Arduino Uno shield and it was tested under Arduino Ide 1.5.x.


#How it works


#Usage
The library was designed to minimize the use of both Arduino RAM and ROM memory. This is translated in just one requirement: developer must know how many AirQ Networks devices will be 

Let's suppose we want to interface two AirQ Networks 
```cpp
sNET snet(2);
```