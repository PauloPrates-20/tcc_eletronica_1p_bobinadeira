#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define KEYPAD_ROWS 4         // quantidade de linhas do teclado matricial
#define KEYPAD_COLUMNS 4      // quantidade de colunas do teclado matricial
#define DISPLAY_COLUMNS 20    // quantidade de colunas do display
#define DISPLAY_ROWS 4        // quantidade de linhas do display
#define DISPLAY_ADDRESS 0x27  // endereço i2c do display
#define STEPPER_PWM 6         // pino de pulso do motor de passo
#define STEPPER_ENABLE 3      // pino de enable do motor de passo
#define STEPPER_DIR 9         // pino de direção do motor de passo
#define DC_PWMA 10            // primeiro pino de pulso do motor dc
#define DC_PWMB 11            // segundo pino de pulso do motor dc

/* controle de telas
id -1 = erro
id 0 = inicio
id 1 = bobinar
id 2 = recalibrar
*/
int telaAtual = 0;  // iniciliza a interface na tela inicial

// definição do teclado matricial
byte keypad_row_pins[KEYPAD_ROWS] = { 2, 3, 4, 5 };          // pinos associados às linhas do teclado
byte keypad_column_pins[KEYPAD_COLUMNS] = { 7, 8, 12, 13 };  // pinos associados às colunas do teclado
// mapeamento das teclas
char keys[KEYPAD_ROWS][KEYPAD_COLUMNS] = { '1', '2', '3', 'A',
                                           '4', '5', '6', 'B',
                                           '7', '8', '9', 'C',
                                           '*', '0', '#', 'D' };
// instanciamento do teclado
Keypad teclado = Keypad(makeKeymap(keys), keypad_row_pins, keypad_column_pins, KEYPAD_ROWS, KEYPAD_COLUMNS);

// instanciamento do display
// endereço 0x27 no modelo físico
// endereço 0x38 para teste no proteus
LiquidCrystal_I2C display(0x38, DISPLAY_COLUMNS, DISPLAY_ROWS);

void setup() {
  Serial.begin(9600);  // inicialização da comunicação Serial

  // tela inicial da interface
  display.begin();
  display.backlight();
  telaInicial();
}

void loop() {
  char commandKey = teclado.getKey();  // lê os comandos do usuário pelo teclado matricial
  if (commandKey) {
    switch (commandKey) {
      case '1':
        telaAtual = telaBobinar();
        break;
      case '2':
        for(int time = 3000; time >= 0; time -= 1000) {
          String tempo = String(time / 1000) + "s";
          telaAtual = telaRecalibrar(tempo);
          delay(1000);
        }
        telaAtual = telaInicial();
        break;
        case '0':
          telaAtual = telaInicial();
          break;
        default:
            selecaoInvalida();
            delay(3000);
            telaAtual = telaInicial();
            break;
    }
  }
}

// Funções de tela
// Tela Incial
int telaInicial() {
  display.clear();
  String titulo = "Bobinadeira V1.0";
  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 1);
  display.print("1 - Bobinar");
  display.setCursor(0, 2);
  display.print("2 - Recalibrar");

  return 0;
}

// Tela Inicial de Bobinar
int telaBobinar() {
  display.clear();
  String titulo = "Bobinar";
  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 1);
  display.print("1 - Novo Indutor");
  display.setCursor(0, 2);
  display.print("2 - Carregar Indutor");
  display.setCursor(0, 3);
  display.print("0 - Voltar");

  return 1;
}

// Tela de calibragem
int telaRecalibrar(String tempo) {
  display.clear();
  String linha1 = "Recalibrando";
  String linha2 = "Aguarde " + tempo;
  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  return 2;
}

// Tela de erro
int selecaoInvalida() {
  display.clear();
  String linha1 = "Comando Invalido!";
  String linha2 = "Tente Novamente!";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  return -1;
}

// Função para centralizar o texto
int centralizarDisplay(String texto) {
  int posicao = (DISPLAY_COLUMNS - texto.length()) / 2;

  if (posicao < 0) {
    return 0;
  } else {
    return posicao;
  }
}
