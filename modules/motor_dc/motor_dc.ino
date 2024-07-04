#define PIN_MOTOR_DC 6
#define PIN_CLK 20

int rpm = 350;
volatile int pulsosEncoder = 0;
volatile bool ultimoEstadoClk = LOW;

unsigned long ultimoDebounce = 0;
const unsigned long DEBOUNCE = 0;

void lerEncoder() {
  unsigned long tempoAtual = millis();
  bool estadoClk = digitalRead(PIN_CLK);

  if ((tempoAtual - ultimoDebounce) > DEBOUNCE && estadoClk != ultimoEstadoClk) {
    if (estadoClk == HIGH) {
    pulsosEncoder++;
    }

    ultimoDebounce = tempoAtual;
  }

  ultimoEstadoClk = estadoClk;
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

  attachInterrupt(digitalPinToInterrupt(PIN_CLK), lerEncoder, CHANGE);
  while (millis() - tempo < 5000) { }
  detachInterrupt(digitalPinToInterrupt(PIN_CLK));

  desligarMotorDc();

  rpm = ((pulsosEncoder / 20.0 / 5.0) * 60.0);

  Serial.print("RPM medido: ");
  Serial.println(rpm);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MOTOR_DC, OUTPUT);
  digitalWrite(PIN_MOTOR_DC, LOW);

  pinMode(PIN_CLK, INPUT_PULLUP);

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
