void tela2(){
  
  //----------- INICIO DA CONFIGURAÇÃO DO NÚMERO DE ESPIRAS -----------
   if (menu == 2){                                      // Menu 2 - Numero de espiras       
    som();
    lcd.clear();
    lcd.setCursor(3, 0);                                // Cursor LCD em 0,0
    lcd.print("Digite o numero");                       // Numero de espiras
    lcd.setCursor(4, 1);                                // Cursor LCD em 0,0
    lcd.print("de espiras:");
    lcd.setCursor(10, 3);                                // Cursor LCD em 0,1
    lcd.print("ENT: Cont.");
    lcd.setCursor(0, 3);                                // Cursor LCD em 0,1
    lcd.print("A: Apaga");
    lcd.setCursor(8, 2);
    lcd.blink();
    while (menu == 2){                                  // Enquanto menu  2
    key=kpd.waitForKey();                               // Num. de espiras
      switch (key){     
        case '0': som(); if (Texto.length() < 4) { Texto += "0"; lcd.print(key); } break;
        case '1': som(); if (Texto.length() < 4) { Texto += "1"; lcd.print(key); } break;
        case '2': som(); if (Texto.length() < 4) { Texto += "2"; lcd.print(key); } break;
        case '3': som(); if (Texto.length() < 4) { Texto += "3"; lcd.print(key); } break;
        case '4': som(); if (Texto.length() < 4) { Texto += "4"; lcd.print(key); } break;
        case '5': som(); if (Texto.length() < 4) { Texto += "5"; lcd.print(key); } break;
        case '6': som(); if (Texto.length() < 4) { Texto += "6"; lcd.print(key); } break;
        case '7': som(); if (Texto.length() < 4) { Texto += "7"; lcd.print(key); } break;
        case '8': som(); if (Texto.length() < 4) { Texto += "8"; lcd.print(key); } break;
        case '9': som(); if (Texto.length() < 4) { Texto += "9"; lcd.print(key); } break;
        
        case 'A':                                        //A: limpa o texto e zera o número de espiras
          Texto  = "";
          numEsp = 0;
          lcd.setCursor(8, 2);
          lcd.print("    "); 
          lcd.setCursor(8, 2);
        break;                                           
        case 'D':        
          numEsp = Texto.toInt();                        // Converte o Texto para Inteiro
          Texto = "";                                    // Limpa o Texto
          som();
          menu = 3;                                      // Muda para o menu 3 Bitola do fio
          delay(10);
        break;
      }
    }
  }
}
