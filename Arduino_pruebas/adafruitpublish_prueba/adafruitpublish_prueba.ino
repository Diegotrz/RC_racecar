// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#define HCR 0
int valpot;
int STHCR =0;
/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("acelerac");

void setup() {
  
  // start the serial connection
  Serial.begin(115200);


  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    
    delay(500);
  }


  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  //io.run(); //is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
   
 
 io.run();

  // save count to the 'counter' feed on Adafruit IO
 STHCR = digitalRead(HCR);  
 // if (STHCR == 01)
  
  Serial.print("sending -> ");
  Serial.println(STHCR);
 counter->save(STHCR);

 delay(3000);
 

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.


}
