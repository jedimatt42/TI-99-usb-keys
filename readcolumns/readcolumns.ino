
volatile int triggers = 0;

void onChange() {
  cli();
  triggers++;
  Serial.println(triggers);
  sei();
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(0, INPUT_PULLUP);
  attachInterrupt(0, onChange, CHANGE);
  Serial.println("setup.");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
}
