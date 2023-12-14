// importer librairie Arduino.h
#include <Arduino.h>

// ---- définition des variables ----

// Pin de la carte.
const int trigPin = 8; // Trigger (émission)
const int echoPin = 9; // Echo (réception)
const int buzzerPin = 10;
const int led1Pin = 12; // led recul
const int led2Pin = 11; // led phare
const int sunPin = A0;

// --- Variables globales. ----
int periodeD = 2000;
unsigned long debutD = 0;

// Variable pour led phare.
int periodeL = 100;
unsigned long debutL = 0;
int etatLed = 0;
float intensite = 0;
int lum = 0;

// Variable pour capteur distance.
long dureeEcho; // durée de l'echo
int distance;   // distance

// Variables pour Buzzer.
int etatBuzzer = 0;
int dureeEteint = 200;
int dureeAllume = 200;
int dureeBuzzer = dureeEteint;
unsigned long debutB = 0;

// Variables pour moteurs.
int dureeM = 2000;
int debutM = 0;
int mode = 1;
int etatM = 0;

const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;

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
  intensite = ((17 / 12) * (lum))-(7225/6);
  if (lum < 500)
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

void testMoteur2(int d) // change l'etat du buzzer en fonction de la duree eteinte ou allumee.
{
  if (((millis() - debutM) > dureeM) and (mode == 1))
  {
    switch (etatM)
    {
    case 0:
      etatM = 1;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    case 1:
      etatM = 0;
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    }
    debutM = millis();
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
    etatM = 0;
  }
  else
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    etatM = 1;
  }
}

void testBouton() // change l'etat du buzzer en fonction de la duree eteinte ou allumee.
{
  // if (bouton presse)
  {
    switch (mode)
    {
    case 0:
      mode = 1;
    case 1:
      mode = 0;
    }
  }
}

void debug()
{
  Serial.print("dureeBuzzer: ");
  Serial.print(dureeBuzzer);
  Serial.print(" | distance: ");
  Serial.print(distance);
  Serial.print(" | etatBuzzer:");
  Serial.print(etatBuzzer);
  Serial.print(" | lum:");
  Serial.print(lum);
  Serial.print(" | intensite:");
  Serial.print(intensite);
  Serial.print(" | etatM:");
  Serial.print(etatM);
  Serial.print(" | dureeEteint:");
  Serial.print(dureeEteint);
  Serial.print("\n");
}

void setup()
{
  pinMode(trigPin, OUTPUT); // Configuration du port du Trigger comme une SORTIE
  pinMode(echoPin, INPUT);  // Configuration du port de l'Echo comme une ENTREE
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
  distance = getDistance();
  testDuree(distance);  // Test si la distance implique de changer la durée du bip.
  testBuzzer(distance); // Test si il est temps de changer l'etat du buzzer. Buzz avec une fréquence qui dépend de la durée d'allumage.
  testMoteur(distance); // Change sens du moteur en fonction du temps.
  // testBouton(); // Vérifie dans quel mode de fonctionnement est le système.
  testSun(); // Test si la lumière extérieur implique de changer l'intensité lumineuse du phare.
  debug();   // montre valeurs des variables de distances, d'etat des lampes et buzzer, d'intensite lumineuse.
}
