

void setup(){
  pinMode(A3, INPUT);
  Serial.begin(9600);
}

void loop(){
  int t = analogRead(A3);
  Serial.println(t);
  delay(100);
}
