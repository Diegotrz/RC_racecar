#define pot 4
int valpot;
int mapot =0;
void setup() {

  pinMode(pot,INPUT);
  Serial.begin(9600);
}

void loop() {
valpot = analogRead(pot);  
mapot = map(valpot,0,4095,0,255);
Serial.println(mapot);
}
