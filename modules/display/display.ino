#include <LiquidCrystal_I2C.h>

#define ENDERECO_DISPLAY 0x27
#define COLUNAS_DISPLAY 20
#define LINHAS_DISPLAY 4

/* 
Controle de telas

id -1 = erro
id 0 = Tela Inicial
id 1 = Tela de bobinar
id 2 = Calibração
*/
int telaAtual = 0;

/* 
Instanciamento do display

endereço 0x27 no modelo físico
endereço 0x38 para teste no proteus
*/
LiquidCrystal_I2C display(ENDERECO_DISPLAY, COLUNAS_DISPLAY, LINHAS_DISPLAY);

// Função para centralizar o texto
int centralizarDisplay(String texto) {
  int posicao = (COLUNAS_DISPLAY - texto.length()) / 2;

  if (posicao < 0) {
    return 0;
  } else {
    return posicao;
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

// void setup e void loop apenas para teste individual. Excluir na integração.
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
