void tela7(){
   
   //------- MENU DE CORREÇÃO ------
  if (menu == 7){
        //digitalWrite(motor_carretel, LOW);    
        lcd.noBlink();   
        lcd.clear();                                        
        lcd.setCursor(0, 0);
        lcd.print("1 - corrige espiras"); 
        lcd.setCursor(0, 1);
        lcd.print("2 - corrige bitola"); 
        lcd.setCursor(0, 2);
        lcd.print("3 - corrige carretel"); 
        lcd.setCursor(0, 3);
        lcd.print("4 - corrigir todos");    
  while (menu == 7){             
  key=kpd.getKey();                           
     switch (key)                      
      {
        case 'D':
         som();
         menu = 7;                                       
        break;                                      
       case '1':                 
         som();
         menu = 8;                                                   
       break;
       case '2':                 
         som();
         menu = 9;                                                   
       break;
       case '3':                 
         som();
         menu = 10;                                                   
       break;
       case '4':                 
         som();
         menu = 2;                                                   
       break;
        
   } 
  }
 }
}
