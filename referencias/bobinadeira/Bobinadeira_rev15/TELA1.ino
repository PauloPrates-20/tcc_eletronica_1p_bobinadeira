void tela1(){
  
if (menu == 1) {                                      // Menu 1 - seleciona funcao                                           
    lcd.clear();                                        // LCD clear
    lcd.setCursor(0, 0);                                // Cursor LCD em 0,0
    lcd.print(" ESCOLHA UMA OPCAO:");                     // Print
    lcd.setCursor(0, 1);                                // Cursor LCD em 0,1
    lcd.print("A.: Para configurar ");                      // Print
    lcd.setCursor(0, 2);                                // Cursor LCD em 0,2
    lcd.print("ENT.: Para iniciar");                         // Print
    lcd.setCursor(0, 3);
    lcd.print("com dados existentes");
    while (menu == 1){                                  // Enquanto menu = 1
    key=kpd.waitForKey();
     switch (key) {
      case 'D':                                         // Se Inicia processo
      som();
       lcd.clear();                                    // LCD clear
        menu = 6;                                      // Muda para o menu 8 Inicia processo / calculos
        delay(10);
      break;
      case 'A':                                         // Se selecionou configuracoes     
        
        lcd.clear();                                    // LCD clear
        lcd.setCursor(4, 0);                            // Cursor LCD em 4,0
        lcd.print("CONFIGURACOES");                     // Print
        lcd.clear();                                    // LCD clear
        menu = 2; 
       break; 
                                           
      }
    }
  }
}
