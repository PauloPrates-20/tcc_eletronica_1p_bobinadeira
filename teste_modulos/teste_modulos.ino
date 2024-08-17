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

char quadrado = 255;

/* 
  | Instanciamento do display
  | -------------------------------------
  | endereço 0x27 no modelo físico
  | endereço 0x38 para teste no proteus
*/
LiquidCrystal_I2C display(0x27, COLUNAS_DISPLAY, LINHAS_DISPLAY);

// Variáveis de controle
int espiras = 0;
int camadas = 0;
bool indutor = false;

String refEspiras = "";
String refComprimento = "";
String refDiametro = "";
int paramEspiras;
int paramComprimento;
float paramDiametro;
String parametro;
String valorFormatado;

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
  display.init();
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
        telaAtual = telaAvisoCalibragem();
        break;
      }
      break;
    case 20:
      switch (tecla) {
        case '1':
          telaAtual = telaIndutor();
          break;
        case '2':
          if (indutor) {
            espiras = 0;
            camadas = 0;
            telaAtual = telaProgresso(espiras, camadas);

            while (espiras < paramEspiras) {
              espiras++;

              if (espiras % 30 - 1 == 0) {
                camadas++;
              }

              atualizarAndamento(espiras, camadas);
              delay(150);
            }

            delay(2000);
            telaAtual = telaInicial();
          }
          break;
        case '0':
          telaAtual = telaInicial();
          break;
      }
      break;
    case 21:
      switch (tecla) {
        case 'A':
          parametro = "Espiras";
          telaAtual = telaParametro(parametro, refEspiras);
          break;
        case 'B':
          parametro = "Comprimento";
          telaAtual = telaParametro(parametro, refComprimento);
          break;
        case 'C':
          parametro = "Diametro";
          telaAtual = telaParametro(parametro, refDiametro);
          break;
        case '*':
          if (refEspiras != "" && refComprimento != "" && refDiametro != "") telaAtual = telaConfirmarParametros();
          break;
        case '#':
          telaAtual = telaBobinar();
          break;
      }
      break;
    case 22:
      switch (tecla) {
        case '*':
          if (parametro == "Espiras") {
            paramEspiras = valorFormatado.toInt();
          } else if (parametro == "Comprimento") {
            paramComprimento = valorFormatado.toInt();
          } else {
            paramDiametro = valorFormatado.toFloat();
          }
          telaAtual = telaIndutor();
          break;
        case '#':
          if (parametro == "Espiras") {
            refEspiras.remove(refEspiras.length() - 1);
            telaAtual = telaParametro(parametro, refEspiras);
          } else if (parametro == "Comprimento") {
            refComprimento.remove(refComprimento.length() - 1);
            telaAtual = telaParametro(parametro, refComprimento);
          } else {
            refDiametro.remove(refDiametro.length() - 1);
            telaAtual = telaParametro(parametro, refDiametro);
          }
          break;
        default:
          if (isDigit(tecla)) {
            if (parametro == "Espiras") {
              if (tecla == '0' && refEspiras.length() == 0) {
                break;
              }
              refEspiras += tecla;
              telaAtual = telaParametro(parametro, refEspiras);
            } else if (parametro == "Comprimento") {
              if (tecla == 0 && refComprimento.length() == 0) {
                break;
              }
              refComprimento += tecla;
              telaAtual = telaParametro(parametro, refComprimento);
            } else {
              if (tecla == '0' && refDiametro.length() == 0) {
                break;
              }
              refDiametro += tecla;
              telaAtual = telaParametro(parametro, refDiametro);
            }
          }
          break;
        }
        break;
      case 23:
        switch (tecla) {
          case '*':
            indutor = true;
            espiras = 0;
            camadas = 0;
            telaAtual = telaProgresso(espiras, camadas);

            while (espiras < paramEspiras) {
              espiras++;

              if (espiras % 30 - 1 == 0) {
                camadas++;
              }

              atualizarAndamento(espiras, camadas);
              delay(150);
            }
            delay(2000);
            telaAtual = telaInicial();
            break;
          case '#':
            telaAtual = telaIndutor();
            break;
        }
        break;
      case 30:
        switch (tecla) {
          case '*':
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
  }
}
