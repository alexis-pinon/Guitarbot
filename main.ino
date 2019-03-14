// checker serial.ParseInt()
// comment réaliser une machine à état (logique séquentielle).
//solution 1 : réécrire en équation logique
// 2 : utiliser des switch case


#define LED_PIN 13
#define ENC_A 19
#define ENC_B 38
#define PWM 8
#define BI1 36
#define BI2 37

#define DT 100
#define KP 0.001
#define KI 0.0005

volatile int pos = 0; // interdit l'optimisation sur l'accès mémoire
int vitesse = 80;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN,OUTPUT);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(PWM, OUTPUT);
  pinMode(BI1,OUTPUT);
  pinMode(BI2,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A),count, RISING);
  Serial.begin(9600);
}


void loop() {
  static long lastSampleTime; //conserve l'ancienne valeur de la variable à chaque appel de la fonction
  while(millis()-lastSampleTime < DT){
  }
  lastSampleTime=millis();
  float vit = 0;
  vit = motorSpeed();
  float consigne = 300;
  float erreur = consigne-vit;

  float commande = P_I(erreur);
  moteur(commande);

  Serial.println(vit);
}

void moteur(int valeur){
  if(valeur>0){
    digitalWrite(BI1,0);
    digitalWrite(BI2,1);
  } else {
    digitalWrite(BI2,0);
    digitalWrite(BI1,1);
  }
  analogWrite(PWM, constrain(abs(valeur),0,191)); // constrain sécurise la valeur
}

void count(){
  if(digitalRead(ENC_B)){
    pos++;
  } else {
    pos--;
  }
}

float motorSpeed(){
  static int pos_prec;
  int pos_k;
  noInterrupts(); // evite de lire une donnée en train d'être modifiée
  pos_k = pos;
  interrupts();
  float vit = (float)(pos_k-pos_prec)*1000/DT;
  pos_prec=pos_k;
  return vit;
}
 float P_I(float erreur){
  static float x;
  float commande = 0;
  commande = KP*erreur + KI * x;
  x = x + erreur * DT;
  x = constrain(x,-191.0/KI,191.0/KI); // anti windup structure
  return commande;
 }
