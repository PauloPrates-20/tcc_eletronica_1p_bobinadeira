void tela6(){
   
   // ------------- INICIO DO MENU PARA CONFERIR E CORRIGIR ----------
if (menu == 6){
  //      digitalWrite(motor_carretel, LOW);
        som();
        lcd.noBlink();   
        lcd.clear();                                       
        lcd.setCursor(0, 0);
        lcd.print("Espiras: ");
        lcd.setCursor(12, 0);
        lcd.print(numEsp); 
        lcd.setCursor(0, 1);
        lcd.print("Fio:              mm");
        lcd.setCursor(13, 1);
        lcd.print(bitFio);
        lcd.setCursor(0, 2);
        lcd.print("Carretel:         mm");
        lcd.setCursor(12, 2);
        lcd.print(carrLeng);
        lcd.setCursor(0, 3); 
        lcd.print("A-corrige ENT-inicia");   
  while (menu == 6){               
  key=kpd.waitForKey();                           
     switch (key)                      
      {
        case 'D':
         menu = 11;                                       
        break;                                      
       case 'A':                 
         som();
         menu = 7;                                                   
       break;
      case 'B':
         som();
         EEPROM.put(20, numEsp); 
         EEPROM.put(30, bitFio);
         EEPROM.put(40, carrLeng);   
      break;       
   } 
  }
 }
}
