void setup() {
  // put your setup code here, to run once:

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int test1 = analogRead(A0);
  int test2 = analogRead(A1);
  int test3 = analogRead(A2);
  Serial.print(test1);
  Serial.print("  ");
  Serial.print(test2);
  Serial.print("  ");
  Serial.print(test3);
  Serial.println();
  //Serial.print("/n");
}
