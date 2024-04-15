#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define KEYPAD_ROWS 4 // quantidade de linhas do teclado matricial
#define KEYPAD_COLUMNS 4 // quantidade de colunas do teclado matricial
#define DISPLAY_COLUMNS 20 // quantidade de colunas do display
#define DISPLAY_ROWS 4 // quantidade de linhas do display
#define DISPLAY_ADDRESS 0x27 // endereço i2c do display
#define STEPPER_PWM 6 // pino de pulso do motor de passo
#define STEPPER_ENABLE 3 // pino de enable do motor de passo
#define STEPPER_DIR 9 // pino de direção do motor de passo
#define DC_PWMA 10 // primeiro pino de pulso do motor dc
#define DC_PWMB 11 // segundo pino de pulso do motor dc

/* controle de telas
id 0 = inicio
id 1 = bobinar
id 2 = memória
id 3 = recalibrar
*/
int telaAtual = 0; // iniciliza a interface na tela inicial

// definição do teclado matricial
byte keypad_row_pins[KEYPAD_ROWS] = {2, 3, 4, 5}; // pinos associados às linhas do teclado
byte keypad_column_pins[KEYPAD_COLUMNS] = {7, 8, 12, 13}; // pinos associados às colunas do teclado
// mapeamento das teclas 
char keys[KEYPAD_ROWS][KEYPAD_COLUMNS] = {'1', '2', '3', 'A',
                                             '4', '5', '6', 'B',
                                             '7', '8', '9', 'C',
                                             '*', '0', '#', 'D'};
// instanciamento do teclado
Keypad teclado = Keypad(makeKeymap(keys), keypad_row_pins, keypad_column_pins, KEYPAD_ROWS, KEYPAD_COLUMNS);

// instanciamento do display
LiquidCrystal_I2C display(DISPLAY_ADDRESS, DISPLAY_COLUMNS, DISPLAY_ROWS);

void setup() {
    Serial.begin(9600); // inicialização da comunicação Serial

    // tela inicial da interface
    display.init();
    display.backlight();
    telaInicial();
}

void loop() {
    char commandKey = teclado.getKey(); // lê os comandos do usuário pelo teclado matricial
    switch(commandKey) {
        case '1':
            telaAtual = telaBobinar();
            break;
        case '2':
            telaAtual = telaMemoria();
            break;
        case '3':
            telaAtual = telaRecalibrar();
            delay(3000);
            telaAtual = telaInicial();
            break;
        default:
            selecaoInvalida();
            delay(3000);
            telaAtual = telaInicial();
    }
}

// desenho da tela inicial do display
int telaInicial() {
    display.setCursor(0,0);
    display.print("Bobinadeira V1.0");
    display.setCursor(0,1);
    display.print("1 - Bobinar");
    display.setCursor(0,2);
    display.print("2 - Memória");
    display.setCursor(0,3);
    display.print("3 - Recalibrar");

    return 0;
}

int telaBobinar() {
    display.setCursor(0,1);
    display.print("Teste Bobinar");
    display.setCursor(0,2);
    display.print("Pressione D para voltar");

    return 1;
}

int telaMemoria() {
    display.setCursor(0,1);
    display.print("Teste Memória");
    display.setCursor(0,2);
    display.print("Pressione D para voltar");

    return 2;
}

int telaRecalibrar() {
    display.setCursor(0,1);
    display.print("Recalibrando");
    display.setCursor(0,2);
    display.print("Aguarde");

    return 3;
}

// tela de erro do teclado
void selecaoInvalida() {
    display.setCursor(0,1);
    display.print("Comando Inválido!");
    display.setCursor(0,2);
    display.print("Tente Novamente!");
}