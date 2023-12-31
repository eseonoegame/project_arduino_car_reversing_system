// ---- définition des variables ----

// Pin de la carte.
const int trigPin = 11;  // Trigger (émission)
const int echoPin = 12;  // Echo (réception)
const int buzzerPin = 10;
const int led1Pin = 8; // led recul
const int led2Pin = 5; // led phare
const int sunPin = A0;

// --- Variables globales. ----
int periodeD = 2000;
unsigned long debutD = 0;

// Variable pour led phare.
int periodeL = 100;
unsigned long debutL = 0;
int etatLed = 0;
int intensite = 0;
int lum = 0;

// Variable pour capteur distance.
long dureeEcho;    // durée de l'echo
int distance;  // distance

// Variables pour Buzzer.
int etatBuzzer = 0;
int dureeEteint = 200;
int dureeAllume = 200;
int dureeBuzzer = dureeEteint;
unsigned long debutB = 0;

// ---- définition des fonctions ----

int getDistance(void)  // retourne la distance de l'obstacle détecté par le capteur.
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
  d = dureeEcho * 0.034 / 2;  //distance = 1/2 vitesse du son fois la durée
  return d;
}

void testDistance() // print la distance tout les T secondes.
{
  if ((millis() - debutD) > periodeD) {
    distance = getDistance();
    //Serial.print(distance);
    //Serial.print("\n");
    debutD = millis();
  }
}

void testSun() // allume la led "phare" plus ou moins fortement selon la lumière présente. (compris entre 1050 (noire) et 255)
{
  lum = int(analogRead(A0));
  intensite = -(940-lum)*3;
  if (lum < 500)
  {
    analogWrite(led2Pin, 0);
  }
  else
  {
    analogWrite(led2Pin, intensite);
  }
}

void testDuree()  // Change la duree d'allumage du buzzer en fonction de la distance.
{ 
  distance=getDistance();
  if (distance>0 and distance<=5 )
  {
    dureeEteint=50;
  }
  if (distance>5 and distance<=10) 
  {
    dureeEteint=200;
  }
   if (distance>10 and distance<=20) 
  {
    dureeEteint=600;
  }
  if (distance>20 and distance<=30) 
  {
    dureeEteint=1000;
  }
}

void testBuzzer() // change l'etat du buzzer en fonction de la duree eteinte ou allumee.
{
  if (millis()-debutB>dureeBuzzer)
  {
    if (etatBuzzer==0 and getDistance()<=30)
    {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(led1Pin, HIGH);
      etatBuzzer=1;
      dureeBuzzer=dureeAllume; // duree avant prochain changement d'etat
    }
    else if (etatBuzzer==1)
    {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(led1Pin, LOW);
      etatBuzzer=0;
      dureeBuzzer=dureeEteint;
    }
    debutB = millis();
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
  Serial.print("\n");
    
}

void setup() 
{
  pinMode(trigPin, OUTPUT);  // Configuration du port du Trigger comme une SORTIE
  pinMode(echoPin, INPUT);   // Configuration du port de l'Echo comme une ENTREE
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);  // Démarrage de la communication série
}

// ---- MAIN ----

void loop() // appellé en boucle par le processeur.
{
  testSun();      // Test si la lumière extérieur implique de changer l'intensité lumineuse du phare.
  testDuree();   // Test si la distance implique de changer la durée du bip.
  testBuzzer(); // Test si il est temps de changer l'etat du buzzer. Buzz avec une fréquence qui dépend de la durée d'allumage.
  debug();   // montre valeurs des variables de distances, d'etat des lampes et buzzer, d'intensite lumineuse.
}
