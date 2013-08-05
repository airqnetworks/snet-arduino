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

#include <sNET.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <Flash.h>
#include <SPI.h>
#include <SD.h>
#include <TinyWebServer.h>

float setTemp = 18.0; // This variabile sets the desired ambient temperature
char blank[] = "                "; // Clears a LCD line
char line1[16], line2[16];
uint8_t upBTN = 6;
uint8_t downBTN = 7;

LiquidCrystal lcd(5, 4, 3, 6, 1, 0);

AIRQ305 *board;
AIRQ100 *sensor;

// Enter a MAC address and IP address for your thermostat below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 2, 177 };

boolean index_handler(TinyWebServer& web_server);
boolean inc_handler(TinyWebServer& web_server);
boolean dec_handler(TinyWebServer& web_server);

TinyWebServer::PathHandler handlers[] = {
  {"/", TinyWebServer::GET, &index_handler },
  {"/inc", TinyWebServer::GET, &incdec_handler },
  {"/dec", TinyWebServer::GET, &incdec_handler },

  {NULL},
};

TinyWebServer web = TinyWebServer(handlers, NULL);


/* This is all the boilerplate code used to render home
   page of the web thermostat */
boolean index_handler(TinyWebServer& web_server) {
  web_server.send_error_code(200);
  web_server.send_content_type("text/html");
  web_server.end_headers();
  web_server << F("<html><body><h1>Ambient temperature: ");
  web_server << F("<font style='color:red'>");
  web_server << float2char(sensor->getTEMP());
  web_server << F("</font><br/>Set temperature: ");
  web_server << F("<font style='color:red'>");
  web_server << float2char(setTemp);
  web_server << F("</font>  <a href='inc' style='font-size:140%'>+</a>/<a href='dec' style='font-size:140%'>-</a><br/>RELAY1 status: <font style='color:red'>");
  web_server << (board->getRELAY1() ? "<font style='color:green'>ON</font>" : "<font style='color:blue'>OFF</font>");
  web_server << F("</font></body></html>\n");
  return true;
}

/* /inc and /dec pages are used to increment and decrement
   setTemp variabile */
boolean incdec_handler(TinyWebServer& web_server) {
  web_server << F("HTTP/1.1 303 See Other\n");
  web_server << F("Location: /");
  web_server.end_headers();

  if(strcmp(web_server.get_path(), "/inc") == 0)
    setTemp += 0.1;
  else
    setTemp -= 0.1;

  return true;
}


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
  // Set pins to interface upBTN and downBTN
  pinMode(upBTN, INPUT);
  pinMode(downBTN, INPUT);
  digitalWrite(upBTN, HIGH); /* Enables pull-up resistors on both pins */
  digitalWrite(downBTN, HIGH);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  web.begin();
  sNET.begin(2);
  
  lcd.begin(16, 2);
  sprintf(line2, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  lcdUpdate("IP Address", line2);
                        delay(3000);

  /* We wait until both AIRQ 305 and AIRQ 100 are detected */  
  lcdUpdate("Waiting R1...", "Waiting sensor...");
  
  while((board = (AIRQ305*)sNET.getDeviceForDeviceID(5,0,1,3)) == 0);
  
  lcdUpdate("Connected!", "Waiting sensor...");
  
  while((sensor = (AIRQ100*)sNET.getDeviceForDeviceID(101,2,1,0)) == 0);
  
  lcdUpdate("Connected!", "Connected!");

}

void loop()
{
  delay(150);  //A little bit of debouncing
  web.process();

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


