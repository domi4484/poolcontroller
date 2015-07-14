/***********************************************************
            Read analog input on request
            
Description:  Programma per leggere la tensione per il 
              progetto piscina
Author:       Damiano Lombardi
Date:         11.02.2012

Copiright (c) Damiano Lombardi
***********************************************************/

const int inA0 = A0;
const int inA1 = A1;

const int _reqA1 = 49;  // ASCII code per '1'

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available())
  {
    String data = Serial.read();
    
    if (data == _reqA1)
    {
      int sensorValue = analogRead(inA1); // Leggo il valore di A0
      Serial.println(sensorValue);  // Invio il valore tramite RS232
      delay(10);  // tempo per l'ADC per riprendersis
    }
  }
}
