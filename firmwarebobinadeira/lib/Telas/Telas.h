#ifndef Telas.h
#define Telas.h
#define COLUNAS_DISPLAY 20
#define LINHAS_DISPLAY 4
#define ENDERECO_DISPLAY 0x27

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <Rele.h>
#include "codigoTelas.h"

// Instância do display
extern LiquidCrystal_I2C display;

// Controle de tela
extern int telaAtual = 1;

// Caractere quadrado
const char QUADRADO = 255;

// Referência dos parâmetros do indutor
extern String refEspiras = "";      // Espiras do indutor na IHM
extern String refComprimento = "";  // Comprimento do indutor na IHM
extern String refDiametro = "";     // Diametro do filamento na IHM
extern String refOffset = "";       // Offset do motor de passo na IHM
extern String parametro;            // Parametro a exibir no display
extern String valorFormatado;       // Valor do parametro formatado para exibição

// Parâmetros do indutor
extern int espiras;         // Quantidade de espiras do indutor
extern int comprimento;     // Comprimento interno do indutor em mm
extern float diametro;      // Bitola do fio em mm
extern int offsetPasso = 20;     // Offset do motor de passo em mm

// Funções auxiliares
int centralizarDisplay(String texto);
void atualizarEspira(int espiras);
void atualizarCamada(int camadas);
void formatarValor(String parametro, String valor);

// Telas
int telaInicial();
int telaBobinar();
int telaIndutor();
int telaParametro(String parametro, String valor);
int telaConfirmarParametros();
int telaProgresso(int espiras, int camadas);
int telaOffset(String parametro, String valor);
int telaAvisoCalibragem();
int telaCalibrarRPM(int tempo);
int telaConfirmarRPM(int rpm);
int telaCalibrarPasso();
int telaConcluirCalibragem();
int telaErroCalibragem();
int telaErroPorta();

#endif