#include <Arduino.h>

// ---- définition des variables ----

// Pin de la carte.
const int trigPin = 8; // Trigger (émission)
const int echoPin = 9; // Echo (réception)
const int buzzerPin = 10;
const int led1Pin = 12; // led recul
const int led2Pin = 11; // led phare
const int sunPin = A0;
const int bouton = 13;
const int IN1 = 7; // Pins moteur.
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;

// --- Variables globales. ----

// Variable pour capteur distance.
long dureeEcho; // durée de l'echo
int distance;   // distance

// Variable pour led phare.
float intensite = 0;
float lum = 0;

// Variables pour Buzzer.
int etatBuzzer = 0;
int dureeEteint = 200;
int dureeAllume = 200;
int dureeBuzzer = dureeEteint;
unsigned long debutB = 0;

// Variables pour moteurs.
int etatM = 0; // sens 1 ou 2 et eteint (0).
int etatBouton = 0;

// ---- définition des fonctions ----

int getDistance() // retourne la distance de l'obstacle détecté par le capteur.
{
  int d;
  // Émission d'un signal de durée 10 microsecondes
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Écoute de l'écho
  dureeEcho = pulseIn(echoPin, HIGH);
  // Calcul de la distance
  d = dureeEcho * 0.034 / 2; // distance = 1/2 vitesse du son fois la durée
  return d;
}

void testSun() // allume la led "phare" plus ou moins fortement selon la lumière présente. (compris entre 1030 (noir dehors) et 850)
{
  lum = int(analogRead(A0));
  intensite = map(lum,925,1023,0,255);//1,417 * lum - 1204,17;
  if (lum < 925)
  {
    analogWrite(led2Pin, 0);
  }
  else
  {
    analogWrite(led2Pin, intensite);
  }
}

void testDuree(int d) // Change la duree d'allumage du buzzer en fonction de la distance.
{
  if (d > 0 and d <= 5)
  {
    dureeEteint = 50;
  }
  if (d > 5 and d <= 10)
  {
    dureeEteint = 200;
  }
  if (d > 10 and d <= 20)
  {
    dureeEteint = 600;
  }
  if (d > 20 and d <= 30)
  {
    dureeEteint = 1000;
  }
}

void testBuzzer(int d) // change l'etat du buzzer en fonction de la duree eteinte ou allumee.
{
  if (millis() - debutB > dureeBuzzer)
  {
    if (etatBuzzer == 0 and d <= 30)
    {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(led1Pin, HIGH);
      etatBuzzer = 1;
      dureeBuzzer = dureeAllume; // duree avant prochain changement d'etat
    }
    else if (etatBuzzer == 1)
    {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(led1Pin, LOW);
      etatBuzzer = 0;
      dureeBuzzer = dureeEteint;
    }
    debutB = millis();
  }
}

void testMoteur(int d) // change l'etat du buzzer en fonction de la duree eteinte ou allumee.
{
  if (d < 15)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  else
  {
    if (etatM ==1)
    {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    else
    {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
  }
}

void testBouton() // change l'etat du buzzer en fonction de la duree eteinte ou allumee.
{
  if (digitalRead(bouton) == LOW)
  {
    etatBouton = 1;
    switch (etatM)
    {
    case 0:
      etatM = 1;
      break;
    case 1:
      etatM = 0;
      break;
    }
  }
  else
  {
    etatBouton = 0;
  }
}

void afficher(int a, String(b))// Affiche etats des variables afin de debuguer le code.
{
  Serial.print(b);
  Serial.print(": ");
  Serial.print(a);
  Serial.print(" | ");
}

void debug()// Affiche etats des variables afin de debuguer le code.
{
  afficher(dureeBuzzer,"dureeBuzzer");
  afficher(etatBuzzer,"etatBuzzer");
  afficher(lum,"lum");
  afficher(intensite,"intensite");
  afficher(etatM,"etatM");
  afficher(etatBouton,"etatBouton");
  afficher(distance,"distance");
  Serial.print("\n");
}

void setup()
{
  pinMode(trigPin, OUTPUT); // Configuration du port du Trigger comme une SORTIE
  pinMode(echoPin, INPUT);  // Configuration du port de l'Echo comme une ENTREE
  pinMode(bouton, INPUT_PULLUP);  
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600); // Démarrage de la communication série
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// ---- MAIN ----

void loop() // appellé en boucle par le processeur.
{
  distance = getDistance(); // Recupere la distance mesure par le capteur.
  testDuree(distance);  // Test si la distance implique de changer la frequence d'activation du buzzer.
  testBuzzer(distance); // Test si il est temps de changer l'etat du buzzer (on/off). Buzz avec une fréquence qui dépend de la periode allume.
  testSun();            // Test si la lumière extérieur implique de changer l'intensité lumineuse du phare.
  testMoteur(distance); // Test si la distance implique de changer le sens de rotation sens du moteur.
  testBouton();         // Test si une pression sur le bouton implique de changer sens rotation du moteur.
  debug();              // Affiche etats des variables afin de debuguer le code.
}
