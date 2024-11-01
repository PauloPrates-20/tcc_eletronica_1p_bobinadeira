void tela5(){
  
  // --------- INICIO DA CONFIGURAÇÃO DE ISOLÃO DAS CAMADAS -----------

  if (menu == 5){                                       // Menu 6 - define se tem isolação entre camadas                                                       
    
    lcd.noBlink();                                                   // BT1; BT2 menu 8; BT3;  BT4 inicio = 6;  BT5
    lcd.clear();                                        // LCD clear
    lcd.setCursor(0, 0);                                // Cursor LCD em 0,0
    lcd.print("ISOLACAO POR CAMADA");                       // Print
    lcd.setCursor(0, 1);                                // Cursor LCD em 5,1
    lcd.print("A = Isolar       ( )");                           // Print
    lcd.print(" ");                                     // Print
    lcd.setCursor(0, 2); 
    lcd.print("B = Tap Central  ( )");
    lcd.setCursor(18, 1);
    if(isolador)lcd.print("X");                        // Print
        else 
        lcd.print(" ");
    lcd.setCursor(18, 2);
    if(tap_central)lcd.print("X");
        else
    lcd.print(" ");      
    lcd.setCursor(0, 3);
    lcd.print("D = para continuar");                       // Print

    while (menu == 5){                                  // Enquanto menu 6
    key=kpd.waitForKey(); 
      if (key == 'D')                                   // Muda de menu
      {   lcd.clear();       
        som();
        menu = 6;                                       // Muda para o menu 7 Fator de correcao de passo
      }
      else if (key == 'B')                              // Definir isolacao NAO
      { 
        som();
        tap_central = !tap_central; 
        lcd.setCursor(18, 2);
        if(tap_central)lcd.print("X");
        else
        lcd.print(" ");                                 // Print
       
      }
      else if (key == 'A')                              // Definir isolacao SIM
      {
        som();
        isolador = !isolador;                                 // Define audio ligado
        lcd.setCursor(18, 1);                            // Cursor LCD em 0,1
        if(isolador)lcd.print("X");                        // Print
        else 
        lcd.print(" "); 
      }
    }
  }
}
