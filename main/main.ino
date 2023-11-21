const int trigPin = 11; // Trigger (émission)
const int echoPin = 12; // Echo (réception)
int buzzerPin = 9;
#define led 9
#define sun A0

const int periodeD = 2000;
int periodeL = 100;
unsigned long debut = 0;
int etatLed = 0;


// Variables utiles
long duree;    // durée de l'echo
int distance;  // distance

// ---- définition des fonctions ----

int getDistance(void)
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
  distance1 = duree * 0.034 / 2; //distance = 1/2 vitesse du son fois la durée 
  // Affichage de la distance dans le Moniteur Série
  return distance1;
}

void turnLed(int state)
{
  switch (state)
  {
  case 0:
    digitalWrite(led,LOW);
  case 1:
    digitalWrite(led,HIGH);
  }
}



void testBuzz(int duree)
{
  tone(buzzerPin, duree);
  noTone(buzzerPin);
}




void testDistance()
{
  if ( (millis()-debut) > periodeD )
  {
    distance = getDistance();
    //Serial.print(distance);
    //Serial.print("\n");
    debut = millis();
  }
}

void testLed()
{
  if (0 < distance and distance < 10)
  Serial.print("0 < distance < 10\n");
  {
    periodeL = 10;
    switch (etatLed)
    {
    case 0:
      turnLed(1);
    case 1:
      turnLed(0);
    }
  }
  if (10 < distance and distance < 20)
  {
    periodeL = 100;
    switch (etatLed)
    {
    case 0:
      turnLed(1);
    case 1:
      turnLed(0);
    }
  }
  if (20 < distance and distance < 30)
  {
    periodeL = 1000;
    switch (etatLed)
    {
    case 0:
      turnLed(1);
    case 1:
      turnLed(0);
    }
  }
  if (40 < distance)
  {
    turnLed(0);
  }
}

void testSun()
{
  int lumos = analogRead(sun)
  if (0 <lumos and lumos < 10)
  {
    periodeL = 1
  }
}


void setup()
{
  pinMode(trigPin, OUTPUT); // Configuration du port du Trigger comme une SORTIE
  pinMode(echoPin, INPUT);  // Configuration du port de l'Echo comme une ENTREE
  pinMode(led, OUTPUT);
  Serial.begin(9600);        // Démarrage de la communication série
  pinMode(buzzerPin, OUTPUT);
}

void loop()
{
  testDistance(); // vérifie si il est temps de regarder la distance.
  testBuzzer();  // Buzz avec une fréquence qui dépend de la distance.
  testSun();    // Test l'intensité lumineuse et allume la led en fonction. 
  testLed();   // regarde si j'allume la led carpas de lumière.
}






