void tela0(){
  
  if (menu == 0){                                       // Menu 0 - ajuste da guia 
    som();                                         
    lcd.clear();                                        // LCD clear
    lcd.setCursor(0, 0);                                // Cursor LCD em 0,0
    lcd.print("=ELETRONICA  MATIAS=");                  // Print
    lcd.setCursor(0, 1);                                // Cursor LCD em 0,0
    lcd.print("Ajuste a guia de fio");                  // Print
    lcd.setCursor(0, 2);                                // Cursor LCD em 0,1
    lcd.print("para o ponto inicial");                   // Print
    lcd.setCursor(0, 3);                                // Cursor LCD em 0,2
    lcd.print("ENT.: para continuar");                        // Print
    lcd.setCursor(0, 2);                                // Cursor LCD em 0,2
  
  while (menu == 0) {                                   // Enquanto for menu 0
  //  inicio = contaStep;
    key=kpd.getKey();
    switch (kpd.getState()){
      
    case PRESSED: 
      if (key)
      {
     oldKey = key;      
      }
    case HOLD:  
      if (key == 'D')                         // Se muda de menu
      {
        som();
        menu = 1;                             // Muda para o menu 1  seleciona funcao
        delay(10);                                     
      }
      else if (oldKey == '2')                 // Se avança step em 5 steps
      {
        stepper.step(120);                               
        //delay(1);                                       
      }
      else if (oldKey == '6')                // Se avança step em 50 steps
      {
        stepper.step(1);                               
       // delay(5);                                              
      }
      else if (oldKey == '4')                // Se avança step em -5 steps
      {
        stepper.step(-2);                               
        delay(5);                                                              
      }
      else if (oldKey == '8')                 // Se avança step em -50 steps
      {
        stepper.step(-120);                              
        delay(1);                                                                     
      }
      else if (key == '0') {
            EEPROM.put(10, 0);    //numEspAtual
         //   EEPROM.put(20, 0);    //numEsp
            EEPROM.put(30, 0);    //bitFio
            EEPROM.put(40, 0);    //carrLeng
            EEPROM.put(50, 0);    //camf          
            som();
            delay(100);  
         
      }
      else if (key == 'A') {       
            returnHome();          
            som();                       
      }
     }              
    }
   }
}
