#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
void setup() {
Serial.begin(9600);
if(!accel.begin()){
  Serial.println("El sensor no logro iniciar");
  while(1);

}

}

void loop() {
   Sensors_event_t event;
   accel.getEvent (&event);
   Serial.print("X="); Serial.print(event.acceleration.x); Serial.print("  ");
   Serial.print("Y="); Serial.print(event.acceleration.y); Serial.print("  ");
   Serial.print("Z="); Serial.print(event.acceleration.z); Serial.print("  ");
   delay(700);
}
