//---------------------------- IMH -------------------------
void selecao(){                                         // Seleciona parametros e menus  
  
  tela0();
  tela1();
  tela2();
  tela3();
  tela4(); 
  tela5();
  tela6();
  tela7();
  tela8();
  tela9();
  tela10();
  tela11();
 
}
//--------------------------------------------------

void LCDContagem()                                      // Print contagem de espiras e camadas
{
  camada = bitFio * numEsp / carrLeng;
   lcd.setCursor(10, 2);
   if (numEspAtual < 10) lcd.print("0");
   if (numEspAtual < 100) lcd.print("0");
   if (numEspAtual < 1000) lcd.print("0");
   
  lcd.print(numEspAtual);
  lcd.setCursor(14, 2);
  lcd.print("/");
  if (numEsp < 10) lcd.print("0");
  if (numEsp < 100) lcd.print("0");
  if (numEsp < 1000) lcd.print("0");
  lcd.print(numEsp);
  lcd.setCursor(10, 3); 
  if (camf < 10) lcd.print("0");
  if (camf < 100) lcd.print("0");
  if (camf < 1000) lcd.print("0");                                // Cursor LCD em 12,1
  lcd.print(camf); 
  lcd.setCursor(14, 3);
  lcd.print("/");
  if (camada < 10) lcd.print("0");
  if (camada < 100) lcd.print("0");
  if (camada < 1000) lcd.print("0");
  lcd.print(camada);                                     
  
}

//---------------------------- FIM DE PROCESSO  --------------------------------

void endProcess()
{ 
  
  EEPROM.put(10, 0);                                     //numEsp
  EEPROM.put(30, 0);                                      //bitFio
  EEPROM.put(40, 0);                                      //carrLeng
  EEPROM.put(50, 0);                                      //camf
 
  digitalWrite(motor_carretel, LOW );                     // Desliga motor do crretel
  lcd.clear();                                            // LCD clear
  lcd.print(" Processo concluido");                       // Print
  lcd.setCursor(0, 1);                                    // Cursor LCD em 0,1
  lcd.print("com:");                                      // Print
  lcd.setCursor(5, 1);                                    // Cursor LCD em 0,3
  lcd.print(numEsp);                                      // Print
  lcd.setCursor(10, 1);                                   // Cursor LCD em 8,3
  lcd.print("Espiras e");                                 // Print
  lcd.setCursor(1, 2);                                    // Cursor LCD em 8,3
  lcd.print(camada);
  lcd.print("  camadas: ENTER ");                                   // Print
  lcd.setCursor(1, 3);                                     // Cursor LCD em 8,3
  lcd.print("para  nova Bobina");                                   // Print
  delay(50);                                              // Delay
    while (menu == 18){                                   // Enquanto menu 9
    fim();
    key=kpd.getKey();
    if (key == 'D'){
      menu = 0;       
    }
  }
}
