void tela4(){
  
  //---------------- INICIO DA CONFIGURAÇÃO DO CARRETEL -------------------
   if (menu == 4) {                                      // Menu 4 - Comprimento do carretel
    lcd.clear();                                        // LCD clear
    lcd.setCursor(0, 0);                                // Cursor LCD em 0,0
    lcd.print("Largura do carretel");                   // Print
    lcd.setCursor(0, 1);                                // Cursor LCD em 0,1
    lcd.print("   em milimetro:");                      // Print                                
    lcd.setCursor(10, 3);                                // Cursor LCD em 0,1
    lcd.print("ENT: Cont.");
    lcd.setCursor(0, 3);                                // Cursor LCD em 0,1
    lcd.print("A: Apaga");
    lcd.setCursor(8, 2);                              // Cursor LCD em 8,2                             
    while (menu == 4){                                  // Enquanto menu 4
      lcd.blink();
      key=kpd.waitForKey();                             // Largura do carretel
      switch (key){
        case '0': som(); if (Texto.length() < 2) { Texto += "0"; lcd.print(key); } break;
        case '1': som(); if (Texto.length() < 2) { Texto += "1"; lcd.print(key); } break;
        case '2': som(); if (Texto.length() < 2) { Texto += "2"; lcd.print(key); } break;
        case '3': som(); if (Texto.length() < 2) { Texto += "3"; lcd.print(key); } break;
        case '4': som(); if (Texto.length() < 2) { Texto += "4"; lcd.print(key); } break;
        case '5': som(); if (Texto.length() < 2) { Texto += "5"; lcd.print(key); } break;
        case '6': som(); if (Texto.length() < 2) { Texto += "6"; lcd.print(key); } break;
        case '7': som(); if (Texto.length() < 2) { Texto += "7"; lcd.print(key); } break;
        case '8': som(); if (Texto.length() < 2) { Texto += "8"; lcd.print(key); } break;
        case '9': som(); if (Texto.length() < 2) { Texto += "9"; lcd.print(key); } break;
        
        case 'A':
          som();
          Texto  = "";
          carrLeng = 0;
          lcd.setCursor(0, 2);
          lcd.print("                    "); 
          lcd.setCursor(8, 2);
        break;                                          //A: limpa o texto e zera o número de espiras
      case 'D':                                         // Mudar para menu 5 Seleciona funcao
        som();
        carrLeng = Texto.toFloat();                     // Converte Texto para Float
        Texto = "";                                     // Limpa Texto
        menu = 5;                                       // Muda para o menu 5 Seleciona funcao
        delay(10);
      break;
      }
    }               
  }
}
