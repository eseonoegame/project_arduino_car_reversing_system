// ---- définition des variables ----

// Pin de la carte.
const int trigPin = 11;  // Trigger (émission)
const int echoPin = 12;  // Echo (réception)
const int buzzerPin = 10;
const int led1Pin = 8; // led recul
const int led2Pin = 13; // led phare
const int sunPin = A0;

// --- Variables globales. ----
int periodeD = 2000;
unsigned long debutD = 0;

// Variable pour led phare.
int periodeL = 100;
unsigned long debutL = 0;
int etatLed = 0;

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
  int distance1;
  // Émission d'un signal de durée 10 microsecondes
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Écoute de l'écho
  dureeEcho = pulseIn(echoPin, HIGH);
  // Calcul de la distance
  distance1 = dureeEcho * 0.034 / 2;  //distance = 1/2 vitesse du son fois la durée
  // Affichage de la distance dans le Moniteur Série
  return distance1;
}

void turnLed(int state, int intensite) // allume la led plus ou moins fort.
{
  if ((millis()-debutL)>periodeL)
  {
    switch (etatLed)
    {
      case 0:
        digitalWrite(led2Pin, LOW);
        etatLed = 1;
        break;
      case 1:
        digitalWrite(led2Pin, HIGH);
        etatLed = 0;
        break;
    }
    switch (intensite)
    {
      default: 
        periodeL = 1000;
        break;
      case 1:
        periodeL = 100;
        break;
      case 2:
        periodeL = 10;
        break;
      case 3:
        periodeL = 1;
        break;
  }
  debutL = millis();
  }
}

void testDistance() // print la distance tout les T secondes.
{
  if ((millis() - debutD) > periodeD) {
    distance = getDistance();
    Serial.print(distance);
    Serial.print("\n");
    debutD = millis();
  }
}

void testSun() // allume la led "phare" plus ou moins fortement selon la lumière présente.
{
  // supérieur à  1000 = noire
  int lum = int(analogRead(A0));
  //Serial.print(analogRead(A0));
  //Serial.print("\n");
  if (lum > 1000) 
  {
    turnLed(1,3);  // on allume la led énormément.
  }
  else if (800 < lum and lum < 1000)
  {
    turnLed(1,2); // on allume la led beaucoup.
  }
  else if (500 < lum and lum < 800)
  {
    turnLed(1,1); // on éteint la led un peu.
  }
  else if (500<lum)
  {
    turnLed(0,0); // on éteint la led.
  }
}

void testDuree()  // Change la duree d'allumage du buzzer en fonction de la distance.
{ 
  distance=getDistance();
  if (distance>5 and distance<=10 )
  {
    dureeEteint=500;
    //Serial.print("dureeEteint=200\n");
  }
  if (distance>10 and distance<=15) 
  {
    dureeEteint=2000;
    //Serial.print("dureeEteint=500\n");
  }
   if (distance>15 and distance<=20) 
  {
    dureeEteint=4000;
    //Serial.print("dureeEteint=800\n");
  }
}

void testBuzzer() // change l'etat du buzzer en fonction de la duree eteinte ou allumee.
{
  if (millis()-debutB>dureeBuzzer)
  {
    if (etatBuzzer==0 and getDistance()<=20)
    {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(led1Pin, HIGH);
      etatBuzzer=1;
      dureeBuzzer=dureeAllume; // duree avant prochain changement d'etat
      //Serial.print("Buzzer on\n");
    }
    else if (etatBuzzer==1)
    {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(led1Pin, LOW);
      etatBuzzer=0;
      dureeBuzzer=dureeEteint;
      //Serial.print("Buzzer off\n");
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
  testSun();      // Allume le phare avec une intensité qui dépend de la distance.
  testDuree();   // Test si la distance implique de changer la durée du bip.
  testBuzzer(); // Test si il est temps de changer l'etat du buzzer. Buzz avec une fréquence qui dépend de la distance.
  debug();
}
