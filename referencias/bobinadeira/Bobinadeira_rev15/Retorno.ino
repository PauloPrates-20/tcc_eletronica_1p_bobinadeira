//declara essa nova variável
//bool flag_direct;

////////////////////////////////////////////////////////////////////////////

void returnHome()
{
  if (flag_direct == 0)
  {
    while ( contaStep > 0)
    {
      contaStep--;
      stepper.step(-1);                                        // Retorna para o inicio
    }
  }
    if (flag_direct == 1)
  {
    while ( contaStep < 0)
    {
      contaStep++;
      stepper.step(1);                                        // Retorna para o inicio
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////
/*

//adiciona as linhas flag_direct no menu 8

    while (menu == 8)                                   // Enquanto menu 8
    { key = kpd.waitForKey();                           // SENTIDO
      if (key == '#')                                   // Define sentido CW
      {
        som();
        direct = 0;                                     // Sentido CW
    flag_direct = 0; ///////////////////////////
        menu = 9;                                       // Inicia processo
      }
      else if (key == '*')                              // Define sentido CCW
      {
        som();
        direct = 1;                                     // Sentido CCW
    flag_direct = 1; ///////////////////////////
        menu = 9;                                       // Inicia processo
      }
    }*/
