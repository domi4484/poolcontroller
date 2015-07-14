/***********************************************************
            Send a dente di sega on request
            
Description:  Programma di aiuto alla programmazione sull'
              host per il progetto piscina
Author:       Damiano Lombardi
Date:         14.02.2012

Copiright (c) Damiano Lombardi
***********************************************************/

void setup()
{
  Serial.begin(9600);
}


int valore = 0;

void loop()
{
  
  if (Serial.available())
  {
    String data = Serial.read();
    if (data == 49)
    {
      Serial.println(valore);
      valore++;
      
      if (valore == 1023)
      {
        valore = 0;
      }
    }
  }
}
