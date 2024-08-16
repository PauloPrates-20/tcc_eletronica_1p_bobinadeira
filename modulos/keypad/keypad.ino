#include <Keypad.h>

#define LINHAS_TECLADO 4
#define COLUNAS_TECLADO 4

byte pinLinhasTeclado[LINHAS_TECLADO] = {2, 3, 4, 5};
byte pinColunasTeclado[COLUNAS_TECLADO] = {6, 7, 8, 9};

char teclas[LINHAS_TECLADO][COLUNAS_TECLADO] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  '*', '0', '#', 'D'
};

Keypad teclado = Keypad(makeKeymap(teclas), pinLinhasTeclado, pinColunasTeclado, LINHAS_TECLADO, COLUNAS_TECLADO);


// Void loop e setup apenas para teste individual. Excluir na integração.
// void setup() {

// }

// void loop() {

// }