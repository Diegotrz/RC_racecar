#include <SoftwareSerial.h>
SoftwareSerial miBT(22,23);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Listo");
miBT.begin(38400);
}

void loop() {
if (miBT.available())
  Serial.write(miBT.read());

if (Serial.available())
  miBT.write(Serial.read());

}
