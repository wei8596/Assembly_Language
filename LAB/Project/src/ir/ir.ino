int i = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//設置Baud rate
  pinMode(12, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(12) == LOW) {
    Serial.println(i);
    ++i;
  }
}
