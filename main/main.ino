/*
Carte arduino UNO.
*/

// ---- importation des librairies ----

#include "Ultrasonic.h"

// ---- declaration des variables ---- 

Ultrasonic ultrasonic(9, 8); // Trig et Echo


// ---- declaration des fonctions ----

void setup() 
{
  Serial.begin(9600); // initialize serial communication at 9600 bits per second.
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

unsigned int getDistance()
{
  unsigned int dist = ultrasonic.Ranging(CM);
  return dist;
}


// ---- MAIN ----

void loop() 
{     
  // put your main code here, to run repeatedly:
 
  Serial.print(getDistance());
  Serial.println(" cm");
  delay(1000);

}
 