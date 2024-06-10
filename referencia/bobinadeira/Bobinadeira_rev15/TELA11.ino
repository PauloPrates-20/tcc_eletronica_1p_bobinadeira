void tela11(){
  
  if (menu == 11) {                                       // Menu 8 - Inicia processo / calculos
     som();                                                
    espCam = (carrLeng  / bitFio) / (1 + dln / 100); // numero de espiras por camada
  //  digitalWrite(motor_carretel, LOW);
    lcd.clear();                                        // LCD clear
    lcd.setCursor(1, 0);                                // Cursor LCD em 0,0
    lcd.print("ESCOLHA O SENTIDO");  
    lcd.setCursor(4, 1);                                // Cursor LCD em 0,0
    lcd.print("PARA  INICIAR");                         // Print
    lcd.setCursor(4, 2);                                // Cursor LCD em 0,0
    lcd.print("O enrolamento");
    lcd.setCursor(0, 3);                                // Cursor LCD em 0,2
    lcd.print(" ESQ <-- OU --> DIR");                   // Print
    
    while (menu == 11)                                   // Enquanto menu 8
    { key=kpd.waitForKey();                             // SENTIDO
      if (key == '#')                                   // Define sentido CW
      {
        digitalWrite(motor_carretel, HIGH ); 
        som();
        direct = 0;                                     // Sentido CW
        flag_direct = 0; ///////////////////////////
        menu = 18;                                       // Inicia processo
      }
      else if (key == '*')                              // Define sentido CCW
      {
        digitalWrite(motor_carretel, HIGH );
        som();
        direct = 1;                                     // Sentido CCW
        flag_direct = 1; ///////////////////////////
        menu = 18;                                       // Inicia processo
      }
    }
     contaStep = 0;               
    lcd.clear();                                        // LCD clear
    lcd.setCursor(3, 1);                                // Cursor LCD em 3,1
    lcd.print(" ");                                     // Print
    lcd.setCursor(0, 0);                                // Cursor LCD em 2,0
    lcd.print("PROC.  EM ANDAMENTO");
    lcd.setCursor(0, 1);
    lcd.print("STATUS  =");
    lcd.setCursor(10, 1);
    lcd.print("Enrolando");
    lcd.setCursor(0, 2);
    lcd.print("ESPIRAS =");
    lcd.setCursor(0, 3);
    lcd.print("CAMADAS =");
    delay(100);      
     
     ciclos = espFase * bitFio * 360 / (1 * 0.703125 * 1 * 3.14159 * 5) * 1.8 * (1 + dciclos / 100) / 0.585;       // 0.640; altere esses valores para um ajuste fino da guia do fio  
    
     EEPROM.get(10, numEspAtual);                        //comando para ler eeprom                                        
     EEPROM.get(50, camf);                           // Zera contador de camadas feitas    
     LCDContagem();                                      // Mostra contagem de espiras e camadas
     contaStep = 0;
  }
}
