// ---- définition des variables ----

// Pin de la carte.
const int trigPin = 11;  // Trigger (émission)
const int echoPin = 12;  // Echo (réception)
const int buzzerPin = 10;
const int led1Pin = 8;
const int led2Pin = 7;
const int sunPin = A0;

// Variables globales.
int periodeD = 2000;
int periodeL = 100;
unsigned long debut = 0;
int etatLed = 0;
long duree;    // durée de l'echo
int distance;  // distance

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
  duree = pulseIn(echoPin, HIGH);
  // Calcul de la distance
  distance1 = duree * 0.034 / 2;  //distance = 1/2 vitesse du son fois la durée
  // Affichage de la distance dans le Moniteur Série
  return distance1;
}

void turnLed(int state, int intensite) // allume la led plus ou moins fort.
{
  if ((millis()-debut)>periodeL)
  {
    switch (etatLed)
    {
      case 0:
        digitalWrite(led1Pin, LOW);
        etatLed = 1;
      case 1:
        digitalWrite(led1Pin, HIGH);
        etatLed = 0;
    }
    switch (intensite)
    {
      case 0:
        //rien
      case 1:
        periodeL = 100;
      case 2:
        periodeL = 10;
      case 3:
        periodeL = 1;
  }
  debut = millis();
  }
}

void testDistance() // print la distance tout les T secondes.
{
  if ((millis() - debut) > periodeD) {
    distance = getDistance();
    //Serial.print(distance);
    //Serial.print("\n");
    debut = millis();
  }
}

void testSun() // allume la led "phare" plus ou moins fortement selon la lumière présente.
{
  // supérieur à  1000 = noire
  int lum = int(analogRead(A0));
  Serial.print(analogRead(A0));
  Serial.print("\n");
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
  //testBuzzer();  // Buzz avec une fréquence qui dépend de la distance.
  testSun();      // Allume le phare avec une intensité qui dépend de la distance.
}
