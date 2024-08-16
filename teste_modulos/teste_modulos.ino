/* Bibliotecas */
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

/* Definições */
// Display
#define ENDERECO_DISPLAY 0x38
#define COLUNAS_DISPLAY 20
#define LINHAS_DISPLAY 4

// Keypad
#define LINHAS_TECLADO 4
#define COLUNAS_TECLADO 4

/* Variáveis */
// Display
/* 
  | Controle de telas
  | -----------------------------
  | id -1 = erro de comando
  | id 1 = Tela de Seleção
  | id 2x = Telas de execução
  | id 3x = Telas de Calibragem
*/
int telaAtual = 0;

int espiras = 0;
int camadas = 0;

/* 
  | Instanciamento do display
  | -------------------------------------
  | endereço 0x27 no modelo físico
  | endereço 0x38 para teste no proteus
*/
LiquidCrystal_I2C display(0x27, COLUNAS_DISPLAY, LINHAS_DISPLAY);

// Keypad
byte pinLinhasTeclado[LINHAS_TECLADO] = {2, 3, 4, 5};
byte pinColunasTeclado[COLUNAS_TECLADO] = {6, 7, 8, 9};

char teclas[LINHAS_TECLADO][COLUNAS_TECLADO] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  '*', '0', '#', 'D'
};

// Instanciamento do keypad
Keypad teclado = Keypad(makeKeymap(teclas), pinLinhasTeclado, pinColunasTeclado, LINHAS_TECLADO, COLUNAS_TECLADO);

void setup() {
  // Inicialização do display
  display.begin();
  display.backlight();

  telaAtual = telaInicial();

  Serial.begin(9600);
}

void loop() {
  char tecla = teclado.getKey();
  if (tecla) {
    Serial.println(tecla);
    Serial.println(telaAtual);
  }

  switch (telaAtual) {
    case 1:
      switch (tecla) {
      case '1':
        telaAtual = telaBobinar();
        break;
      case '2':
        int tempo = 5;
        while (tempo > 0) {
          telaAtual = calibrarRPM(tempo);
          delay(1000);
          tempo--;
        }
        telaAtual = confirmarRPM(268);
        delay(2000);
        telaAtual = calibrarPasso();
        delay(2000);
        telaAtual = concluirCalibragem();
        delay(2000);
        telaAtual = telaInicial();
        break;
      }
      break;
    case 20:
      switch (tecla) {
        case '1':
          telaAtual = telaIndutor();
          break;
        case '2':
          telaAtual = telaProgresso(espiras, camadas);

          while (espiras < 120) {
            espiras++;

            if (espiras % 30 == 0) {
              camadas++;
            }

            atualizarAndamento(espiras, camadas);
            delay(150);
          }

          espiras = 0;
          camadas = 0;
          delay(2000);
          telaAtual = telaInicial();
          break;
        case '0':
          telaAtual = telaInicial();
          break;
      }
      break;
    case 21:
      switch (tecla) {
        case 'A':
          break;
        case 'B':
          break;
        case 'C':
          break;
        case '*':
          break;
        case '#':
          telaAtual = telaBobinar();
          break;
      }
      break;
  }
}
