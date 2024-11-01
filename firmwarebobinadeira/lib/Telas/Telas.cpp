#include "Arduino.h"
#include "Telas.h"
#include "codigoTelas.h"
#include <LiquidCrystal_I2C.h>
#include <Rele.h>

LiquidCrystal_I2C display(ENDERECO_DISPLAY, COLUNAS_DISPLAY, LINHAS_DISPLAY);

int centralizarDisplay(String texto) {
  int posicao = (COLUNAS_DISPLAY - texto.length()) / 2;

  if (posicao < 0) {
    return 0;
  } else {
    return posicao;
  }
}

// Função para atualizar o progresso
void atualizarEspira(int espiras) {
  display.setCursor(8, 2);
  display.print(espiras);
}

void atualizarCamada(int camadas) {
  display.setCursor(8, 3);
  display.print(camadas);
}

// Função para formatar valores
void formatarValor(String parametro, String valor) {
  valorFormatado = valor;
  if (valor.length() < 1) {
    valorFormatado = "0";
  }

  if (parametro == "Diametro") {
    if (valor.length() < 1) {
      valorFormatado = "0";
    } else if (valor.length() < 2) {
      valorFormatado = "0.0" + valor;
    } else if (valor.length() < 3) {
      valorFormatado = "0." + valor;
    } else {
      int tamanhoOriginal = valor.length();
      valorFormatado = valor.substring(0, tamanhoOriginal - 2) + "." + valor.substring(tamanhoOriginal - 2);
    }
  }
}

/* Telas */
// Tela Incial
int telaInicial() {
  display.clear();

  String titulo = "Bobinadeira V1.0";

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 1);
  display.print("1)Bobinar");
  display.setCursor(0, 2);
  display.print("2)Recalibrar");
  display.setCursor(0, 3);
  display.print("0)Offset");

  seguro();

  return INICIAL;
}

/* Telas de execução */
// Tela de inicialização do processo
int telaBobinar() {
  display.clear();

  String titulo = "Bobinar";

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 1);
  display.print("1)Novo Indutor");
  display.setCursor(0, 2);
  display.print("2)Repetir Indutor");
  display.setCursor(0, 3);
  display.print("#)Voltar");

  seguro();

  return BOBINAR;
}

// Tela de dimensionamento do indutor
int telaIndutor() {
  display.clear();

  String titulo = "Ajustar parametros";
  String linha1 = "A)Esp B)Comp C)Diam";
  String linha2 = "*)Seguir #)Voltar";

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 3);
  display.print(linha2);

  seguro();

  return INDUTOR;
}

int telaParametro(String parametro, String valor) {
  display.clear();

  String espacador = "";

  if (parametro != "Espiras") {
    espacador = "mm";
  }

  formatarValor(parametro, valor);

  String titulo = "Definir " + parametro;
  String linha1 = parametro + ": " + String(valorFormatado) + QUADRADO + espacador;
  String linha2 = "*)Aceitar #)Apagar";


  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 3);
  display.print(linha2);

  seguro();

  return PARAMETROS;
}

// Tela de confirmação dos parâmetros
int telaConfirmarParametros() {
  display.clear();

  String linha1 = "Espiras: " + String(espiras);
  String linha2 = "Comprimento: " + String(comprimento) + " mm";
  String linha3 = "Diametro: " +  String(diametro) + " mm";
  String linha4 = "*)Confirmar #)Voltar";

  display.setCursor(0, 0);
  display.print(linha1);
  display.setCursor(0, 1);
  display.print(linha2);
  display.setCursor(0, 2);
  display.print(linha3);
  display.setCursor(centralizarDisplay(linha4), 3);
  display.print(linha4);

  cuidado();

  return CONFIRMAR_PARAMETROS;
}

// Tela de monitoramento do processo
int telaProgresso(int espiras, int camadas) {
  display.clear();

  String titulo = "Andamento";
  String linha1 = "Espira: " + String(espiras);
  String linha2 = "Camada: " + String(camadas);

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 2);
  display.print(linha1);
  display.setCursor(0, 3);
  display.print(linha2);

  perigo();

  return PROGRESSO;
}

// Tela de ajuste do offset
int telaOffset(String parametro, String valor) {
  display.clear();

  String espacador = "";

  if (parametro != "Espiras") {
    espacador = "mm";
  }

  formatarValor(parametro, valor);

  String titulo = "Definir " + parametro;
  String linha1 = parametro + ": " + String(valorFormatado) + QUADRADO + espacador;
  String linha2 = "*)Aceitar #)Apagar";


  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 3);
  display.print(linha2);

  seguro();

  return OFFSET;
}

/* Telas de calibragem */
// Tela de aviso de segurança de calibragem
int telaAvisoCalibragem() {
  display.clear();

  String titulo = "Iniciando calibragem";
  String linha1 = "Retire o filamento";
  String linha2 = "*)Seguir #)Voltar";

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 3);
  display.print(linha2);

  cuidado();

  return AVISO_CALIBRAGEM;
}

// Tela de calibragem do RPM
int telaCalibrarRPM(int tempo) {
  display.clear();

  String linha1 = "Calibrando RPM";
  String linha2 = "Aguarde " + String(tempo) + "s";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  perigo();

  return CALIBRAGEM_RPM;
}

// Tela de exibição do RPM
int telaConfirmarRPM(int rpm) {
  display.clear();

  String linha1 = "RPM Medido:";
  String linha2 = String(rpm) + " RPM";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);
  
  return CONFIRMAR_RPM;
}

// Tela de calibragem do motor de passo
int telaCalibrarPasso() {
  display.clear();

  String linha1 = "Alinhando eixo";
  String linha2 = "Aguarde...";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  perigo();

  return CALIBRAGEM_PASSO;
}

// Tela de conclusão da calibragem
int telaConcluirCalibragem() {
  display.clear();

  String linha1 = "Eixo alinhado";
  String linha2 = "Calibragem concluida";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  cuidado();

  return CONCLUIR_CALIBRAGEM;
}

// Tela de erro
int telaErroCalibragem() {
  display.clear();

  String titulo = "Aviso";
  String linha1 = "Falha ao medir RPM";
  String linha2 = "Tentando novamente";

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(centralizarDisplay(linha1), 2);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 3);
  display.print(linha2);

  perigo();

  return ERRO_CALIBRAGEM;
}

// Tela de erro porta aberta
int telaErroPorta() {
  display.clear();

  String titulo = "Aviso";
  String linha1 = "Porta aberta";

  display.setCursor(centralizarDisplay(titulo), 1);
  display.print(titulo);
  display.setCursor(centralizarDisplay(linha1), 2);
  display.print(linha1);

  perigo();

  return ERRO_PORTA;
}