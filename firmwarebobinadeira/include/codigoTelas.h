#ifndef codigoTelas_h
#define codigoTelas_h

// Controle de telas do display
enum Codigos {
  ERRO_PORTA = -2,
  ERRO_CALIBRAGEM,
  INICIAL = 1,
  BOBINAR = 20,
  INDUTOR,
  PARAMETROS,
  CONFIRMAR_PARAMETROS,
  PROGRESSO,
  OFFSET,
  AVISO_CALIBRAGEM = 30,
  CALIBRAGEM_RPM,
  CONFIRMAR_RPM,
  CALIBRAGEM_PASSO,
  CONCLUIR_CALIBRAGEM
};

#endif