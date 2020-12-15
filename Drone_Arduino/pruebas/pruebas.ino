int a = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("..........WELCOME..........");
}

void loop() {

    
    if (Serial.available() > 0) {
      a = Serial.read(); // read the incoming byte:
      Serial.print(" I received:");
      Serial.println(a);
    }
}
