void setup() {
  int pinlec = 22;
int val;

pinMode(pinlec, INPUT);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
//val = digitalRead(pinlec);
val = 5;
//Serial.println("hola");

delay(400);
}
