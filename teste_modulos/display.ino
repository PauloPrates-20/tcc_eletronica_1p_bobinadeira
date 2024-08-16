/* Funções auxiliares */
// Função para centralizar o texto
int centralizarDisplay(String texto) {
  int posicao = (COLUNAS_DISPLAY - texto.length()) / 2;

  if (posicao < 0) {
    return 0;
  } else {
    return posicao;
  }
}

// Função para atualizar o progresso
void atualizarAndamento(int espiras, int camadas) {
  display.setCursor(8, 2);
  display.print(espiras);
  display.setCursor(8, 3);
  display.print(camadas);
} 

/* Telas */
// Tela Incial
int telaInicial() {
  display.clear();

  String titulo = "Bobinadeira V1.0";

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(0, 2);
  display.print("1)Bobinar");
  display.setCursor(0, 3);
  display.print("2)Recalibrar");

  return 1;
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
  display.print("0)Voltar");

  return 20;
}

// Tela de dimensionamento do indutor
int telaIndutor() {
  display.clear();

  String titulo = "Ajustar parametros";
  String linha1 = "A)Esp B)Comp C)Diam";
  String linha2 = "*)Confirmar #)Voltar";

  display.setCursor(centralizarDisplay(titulo), 0);
  display.print(titulo);
  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 3);
  display.print(linha2);
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

  return 21;
}

/* Telas de calibragem */
// Tela de calibragem do RPM
int calibrarRPM(int tempo) {
  display.clear();

  String linha1 = "Calibrando RPM";
  String linha2 = "Aguarde " + String(tempo) + "s";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  return 31;
}

// Tela de exibição do RPM
int confirmarRPM(int rpm) {
  display.clear();

  String linha1 = "RPM Medido:";
  String linha2 = String(rpm) + " RPM";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);
  
  return 32;
}

// Tela de calibragem do motor de passo
int calibrarPasso() {
  display.clear();

  String linha1 = "Alinhando eixo";
  String linha2 = "Aguarde...";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  return 33;
}

// Tela de conclusão da calibragem
int concluirCalibragem() {
  display.clear();

  String linha1 = "Eixo alinhado";
  String linha2 = "Calibragem concluida";

  display.setCursor(centralizarDisplay(linha1), 1);
  display.print(linha1);
  display.setCursor(centralizarDisplay(linha2), 2);
  display.print(linha2);

  return 30;
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
