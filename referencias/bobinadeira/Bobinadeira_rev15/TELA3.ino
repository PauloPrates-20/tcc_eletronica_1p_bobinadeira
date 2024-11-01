void tela3(){
   
   //------------ INICIO DA CONFIGURAÇÃO DA BITOLA DO FIO ---------
  if (menu == 3) {                                      // Menu 3 - Bitola do fio                                                    
    lcd.clear();                                        // LCD clear
    lcd.setCursor(1, 0);                                // Cursor LCD em 0,0
    lcd.print("Digite a bitola do");                    // Print
    lcd.setCursor(2, 1);                                // Cursor LCD em 0,1
    lcd.print("fio em milimetro"); 
    lcd.setCursor(10, 3);                                // Cursor LCD em 0,1
    lcd.print("ENT: Cont.");
    lcd.setCursor(0, 3);                                // Cursor LCD em 0,1
    lcd.print("A: Apaga");
    lcd.setCursor(8, 2);
    while (menu == 3) {                                 // Enquanto menu 3
    lcd.blink();
    key=kpd.waitForKey();                               // Bitola do fio
    switch (key)
    {
        case '0': som(); if (Texto.length() < 4) { Texto += "0"; lcd.print(key); } break;
        case '1': som(); if (Texto.length() < 4) { Texto += "1"; lcd.print(key); } break;
        case '2': som(); if (Texto.length() < 4) { Texto += "2"; lcd.print(key); } break;
        case '3': som(); if (Texto.length() < 4) { Texto += "3"; lcd.print(key); } break;
        case '4': som(); if (Texto.length() < 4) { Texto += "4"; lcd.print(key); } break;
        case '5': som(); if (Texto.length() < 4) { Texto += "5"; lcd.print(key); } break;
        case '6': som(); if (Texto.length() < 4) { Texto += "6"; lcd.print(key); } break;
        case '7': som(); if (Texto.length() < 4) { Texto += "7"; lcd.print(key); } break;
        case '8': som(); if (Texto.length() < 4) { Texto += "8"; lcd.print(key); } break;
        case '9': som();if (Texto.length() < 4) { Texto += "9"; lcd.print(key); } break;
        case '.': som();if (Texto.length() < 4) { Texto += "."; lcd.print(key); } break;
        case 'A':
          som();
          Texto  = "";
          bitFio = 0;
          lcd.setCursor(0, 2);
          lcd.print("                    "); 
          lcd.setCursor(8, 2);
        break;                                          //A: limpa o texto e zera o número de espiras
      case 'D':                                         // Muda para menu 4 Comprimento do carretel
        som();
        bitFio = Texto.toFloat();                       // Converte o texto para Float;
        Texto = "";                                     // Limpa o Texto
        menu = 4;                                       // Muda para menu 4 Comprimento do carretel
        delay(10);
      break;
     }    
    }
  }
}
