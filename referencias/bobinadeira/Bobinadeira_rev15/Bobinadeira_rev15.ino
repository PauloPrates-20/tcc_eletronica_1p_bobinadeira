
#include <LiquidCrystal_I2C.h>


#include <EEPROM.h>
#include <Keypad.h>
#include <Keypad_I2C.h>

#include <Wire.h>
//#include <LiquidCrystal_PCF8574.h>
//#include <LiquidCrystal_I2C.h>
#include <Stepper.h>                                        // Biblioteca para uso do step motor

/*
   Nome do Projeto:  Bobinadeira
   Nome do Aquivo:   bobinadeira_Botao13.ino
   link: 			 http://labdegaragem.com/forum/topics/bobinadeira-autom-tica-controlada-com-arduino
   Dependencias:     funcoes13.ino, funcoes14.ino,tutorial.ino
   MCU:              ATmega
   Board:            Arduino Uno/Mega/Mini
   Compilador        N/A
   IDE:              Arduino IDE 1.8.15
   Hardware:         Arduino UNO/MEGA/Mini
   Escrito por:      Rui Viana
   Modificado por:   Juarez Matias 
   Corrigido por:    Gilson do canal curse agora 
   Finalizado por:   André Leão
   Data:             08/12/2109 
   Uso:              Didático
   Desenhos          CONTADOR2_SIMU.PDF
   Copyright @       N/A
   
   Este programa é software livre;
   e é distribuído na esperança que possa ser útil, mas SEM QUALQUER GARANTIA;
   mesmo sem a garantia implícita de COMERCIALIZAÇÃO ou ADEQUAÇÃO A UM DETERMINADO FIM.
 
*/
Stepper stepper(200, 12, 13);                               // Ports do step e steps por volt
unsigned long previousMillis = 0;
int buzzerState = LOW; 
#define pinBuzzer         5                              
#define encoder           2                                 // Entrada do encoder do motor principal
#define motor_carretel    11                                // Port motor do carretel
String  Texto          = "";
int     numEsp = 0;                                             // Numero de espiras previstas
float   bitFio = 0;                                             // Bitola do fio em mm
float   carrLeng = 0;                                           // Comprimento do carretel
int     numEspAtual    =  0;                                // Numero de espirar atuais
bool    incrementou = false;                                // Controle de incremento de espiras
int     espFase        =  1;                                // N de espiras por etapa
int     espCam         =  0;                                // Calcula o numero de espira por camada
int     camf           =  1;                                // Numero de camada feita
float   dln            =  0;                                // Fator de correção de largura
float   dciclos        =  0;                                // Fator de correção passo
bool    isolador    = false;                                // Contrele de isolador de camada
bool    flag_direct;
int     menu           =  0;                                // Telas do sistema
int     conta          =  0;                                // Variavel encoder
int     conta_old      =  0;                                // Estado ant encoder
int     cstep          =  0;                                // Contador de passos
int     ciclos         =  0;                                // Calcula o numero de ciclos da guia linear para casa passo
int     contaStep      =  0;
bool    direct         =  1;                                // Sentido da guia
float   incrementa     =  1;                                // Offset de incremento
bool    pause       = false;                                // Contrele de pause
bool    tap_central  = false;                               // Transfomador com tap central
int     camada = 0;                                         

// ---------------------- TECLADO 4X4 ----------------------------
const long interval = 2500; 
#define enderecoTeclado 0x20  //0x20 para Maquina 0x38 Simulação
const byte linhas = 4; //four rows
const byte colunas = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[linhas][colunas] = {
                    {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','.'},
                    {'*','0','#','D'}
                  };

// ---------------- Para teclado da Maquina ------------------
byte rowPins[linhas] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[colunas] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad
//-------------- Para Simulação -----------
//byte rowPins[linhas] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
//byte colPins[colunas] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad
Keypad_I2C kpd(makeKeymap(hexaKeys), rowPins, colPins, linhas, colunas, enderecoTeclado, PCF8574 );
//LiquidCrystal_PCF8574 lcd(0x27);     // 0x27 para a maquina, 0x3F para simulação
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
char key = kpd.getKey();
char  tecla;
char oldKey ;

//------ SETUP -----------------------------
void setup(){
  kpd.begin();
  lcd.begin(20, 4);                                         // Inicializa LCD
  Wire.begin();
 
  EEPROM.get(10, numEspAtual);                               //comando para ler eeprom
  EEPROM.get(20, numEsp);                                    //comando para ler eeprom
  EEPROM.get(30, bitFio);                                    //comando para ler eeprom
  EEPROM.get(40, carrLeng);                                  //comando para ler eeprom
  EEPROM.get(50, camf);                                      //comando para ler eeprom

  pinMode(encoder, INPUT);                                  // Encoder como INPUT                            
 // pinMode(encoder, INPUT_PULLUP);                           // Debug
  pinMode(motor_carretel, OUTPUT);                          // Port do motor do carretel como saida
  digitalWrite(motor_carretel, LOW);                        // Desliga motor do carretel
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinBuzzer, HIGH);
  stepper.setSpeed(400); 
  
}
//---------------------------- LOOP -----------------------------
void loop(){
  //comando para gravar numeros grandes na eeprom
  if(analogRead(A1)<800){
   EEPROM.put(10, numEspAtual); 
   EEPROM.put(20, numEsp); 
   EEPROM.put(30, bitFio);
   EEPROM.put(40, carrLeng);   
   EEPROM.put(50, camf); 
    delay(5000);     
  }
  
  selecao();   
  cstep = 0;                                                // Zera contador de passos do step
  
  conta = digitalRead(encoder);                             // Atualiza a contagem de espiras
  
  if (conta == LOW and conta_old == HIGH)                   // Se sensor motor HIGH e conta_old LOW  (Houve ciclo do motor)
  {
    incrementou = true;                                     // Informa incremento de espiras
    numEspAtual++;                                          // Incrementa numero de espiras feitas
  }
  conta_old = conta;                                        // Atualiza contador de pulso
  
  if (incrementou == true)                                  // Houve incremento de espiras
  {
    LCDContagem();                                          // Mostra contagem de espiras e camadas
    incrementou = false;                                    // Desabilita informar incremento de espiras
    if (numEspAtual == numEsp)                              // Numero de espiras realizadas = num de espiras esperadas
    {
    //  EEPROM.put(10, 0); 
    
      endProcess();                                         // Para processo
    }
    if (numEspAtual < numEsp)                               // Numero de espiras realizadas < num de espiras esperadas
    {
      if (direct != LOW)                                    // Se direcao e CW
      {
        while (cstep < ciclos)                              // Enquanto contagem de passos for menor que ciclos
        {
          clockwise();                                      // Gira um step CW
          cstep = cstep + 1;                                // Incrementa contador de passos
        }
      }
      else                                                  // Se direcao e CCW
      {
        while (cstep < ciclos)                              // Enquanto contagem de passos for menor que ciclos
        {
          counterclockwise();                               // Gira um step CCW
          cstep = cstep + 1;                                // Incrementa contador de passos
        }
      }
      if (numEspAtual % espCam == 0)                        // Se modulo de qte de espiras por camada = 0
      {
        camf++;                                             // Incrementa contador de camadas
        LCDContagem();                                      // Mostra contagem de espiras e camadas
        direct = not(direct);                               // Inverte direcao do step
        if (isolador == true)                               // Se precisa de isolador ligado
        { 
         
          digitalWrite(motor_carretel, LOW);                // Desligue o motor do carretel
             lcd.setCursor(10,1);
             lcd.print("  Pausado");
          int t = 0;                                        // Define t como inteiro e igual a zero  ??
          for (t = 0; t < 50;)                              // Faca 50 vezes ??
          {
            if(kpd.getKey() == 'D')
            {
              som();
             lcd.setCursor(10,1);
             lcd.print("Enrolando");
              digitalWrite(motor_carretel, HIGH );
              break;                                        // termine o for
            }
          }
        }
      }
       if(tap_central && (numEspAtual==numEsp/2))
      {
        pause = true;
             lcd.setCursor(10,1);
             lcd.print("  Pausado");
      }
      while (pause == HIGH)
      {
        digitalWrite(motor_carretel, LOW);                    // Desligue o motor do carretel
        if(kpd.getKey() == 'D')
        {
          lcd.setCursor(10,1);
          lcd.print("Enrolando");         
          digitalWrite(motor_carretel, HIGH );
            som();
            pause = LOW;                                       // Desabilita pausar          
        }
      }
    }
  }
                         
  if(kpd.getKey() == 'D')
  { 
      
      som();
      lcd.setCursor(10,1);
      lcd.print("  Pausado");
    //  lcd.print("    ");
      pause = HIGH;                                            // Habilita pausar
   }
}
void fim(){
     unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
    if (buzzerState == LOW) {
      som();
    }
  }
}

void som(){
  digitalWrite(pinBuzzer, HIGH);
  delay(20);
  digitalWrite(pinBuzzer, LOW);
  
}

//------------------------------- CCW --------------------------- // funcoes de acionamento do motor de passo
void counterclockwise ()                                    // Avanca step CCW
{
  //lcd.setCursor(17, 1);
 // lcd.print(">>>");
  stepper.step(1);                                          // Avanca um step CCW
  contaStep = contaStep + 1;
}
//-------------------------------- CW ------------------------------
void clockwise()                                            // Avanca step CW
{
  stepper.step(-1);                                         // Avanca um step CW
  contaStep = contaStep - 1;
 // lcd.setCursor(17, 1);
  //lcd.print("<<<");
}
