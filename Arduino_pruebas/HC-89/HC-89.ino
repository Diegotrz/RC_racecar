 int pinlec = 22;
int val;

void setup() {
 

pinMode(pinlec, INPUT);
Serial.begin(9600);

}

void loop() {

val = digitalRead(pinlec);

Serial.println(val);

delay(400);
}
