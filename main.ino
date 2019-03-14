// checker serial.ParseInt()
// comment réaliser une machine à état (logique séquentielle).
//solution 1 : réécrire en équation logique
// 2 : utiliser des switch case


#define LED_PIN 13

//moteur 1
#define ENC_TEN_A 31
#define ENC_TEN_B 18
#define PWM_TEN 12
#define BI1_TEN 34
#define BI2_TEN 35

// moteur 2
#define ENC_GRA_A 19
#define ENC_GRA_B 38
#define PWM_GRA 8
#define BI1_GRA 36
#define BI2_GRA 37

#define DT 100
#define KP 1
#define KI 0.00001

volatile int pos = 0; // interdit l'optimisation sur l'accès mémoire
int vitesse = 80;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN,OUTPUT);
  pinMode(ENC_GRA_A, INPUT_PULLUP);
  pinMode(ENC_GRA_B, INPUT_PULLUP);
  pinMode(PWM_GRA, OUTPUT);
  pinMode(BI1_GRA,OUTPUT);
  pinMode(BI2_GRA,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_GRA_A),count, RISING);
  Serial.begin(9600);

  //Accordage
}


void loop() {
  static long lastSampleTime; //conserve l'ancienne valeur de la variable à chaque appel de la fonction
  while(millis()-lastSampleTime < DT){
  }
  lastSampleTime=millis();
  float vit = 0;
  vit = motorSpeed();
  float consigne = 20;
  float erreur = consigne-pos;
  float commande = P_I(erreur);
  moteur_GRA(commande);

  Serial.println(pos);
}

void moteur_TEN(int valeur){
  if(valeur>0){
    digitalWrite(BI1_TEN,0);
    digitalWrite(BI2_TEN,1);
  } else {
    digitalWrite(BI2_TEN,0);
    digitalWrite(BI1_TEN,1);
  }
  analogWrite(PWM_TEN, constrain(abs(valeur),0,191)); // constrain sécurise la valeur
}

void moteur_GRA(int valeur){
  if(valeur>0){
    digitalWrite(BI1_GRA,0);
    digitalWrite(BI2_GRA,1);
  } else {
    digitalWrite(BI2_GRA,0);
    digitalWrite(BI1_GRA,1);
  }
  analogWrite(PWM_GRA, constrain(abs(valeur),0,191)); // constrain sécurise la valeur
}

void moteur_TEN(int valeur){
  if(valeur>0){
    digitalWrite(BI1_TEN,0);
    digitalWrite(BI2_TEN,1);
  } else {
    digitalWrite(BI2_TEN,0);
    digitalWrite(BI1_TEN,1);
  }
  analogWrite(PWM_TEN, constrain(abs(valeur),0,191)); // constrain sécurise la valeur
}

void count(){
  if(digitalRead(ENC_GRA_B)){
    pos++;
  } else {
    pos--;
  }
}

//float motorSpeed(){
//  static int pos_prec;
//  int pos_k;
//  noInterrupts(); // evite de lire une donnée en train d'être modifiée
//  pos_k = pos;
//  interrupts();
//  float vit = (float)(pos_k-pos_prec)*1000/DT;
//  pos_prec=pos_k;
//  return vit;
//}

 float P_I(float erreur){
  static float x;
  float commande = 0;
  commande = KP*erreur + KI * x;
  x = x + erreur * DT;
  x = constrain(x,-191.0/KI,191.0/KI); // anti windup structure
  return commande;
 }
