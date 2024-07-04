#define PIN_MOTOR_DC 6
#define PIN_ENCODER 20

int rpm = 350;
volatile int pulsosEncoder = 0;

void pulsoEncoder() {
  pulsosEncoder++;
}

void ligarMotorDc() {
  digitalWrite(PIN_MOTOR_DC, HIGH);
}

void desligarMotorDc() {
  digitalWrite(PIN_MOTOR_DC, LOW);
}

void medirRpmDc() {
  pulsosEncoder = 0;

  unsigned long tempo = millis();

  ligarMotorDc();

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), pulsoEncoder, RISING);
  while (millis() - tempo < 5000) { }
  detachInterrupt(digitalPinToInterrupt(PIN_ENCODER));

  desligarMotorDc();

  rpm = ((pulsosEncoder / 20.0 / 5.0) * 60.0);

  Serial.print("RPM medido: ");
  Serial.println(rpm);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MOTOR_DC, OUTPUT);
  digitalWrite(PIN_MOTOR_DC, LOW);

  pinMode(PIN_ENCODER, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    char comando = Serial.read();

    switch (comando) {
      case 'c':
        medirRpmDc();
        break;
    }
  }
}
