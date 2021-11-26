// Leds de indicadores
const int led1v = PD_0;
const int led1r = PD_1;
const int led2v = PD_2;
const int led2r = PD_3;
const int led3v = PE_1;
const int led3r = PE_2;
const int led4v = PE_3;
const int led4r = PF_1;

// Push Buttons
const int PB1 = PB_4;
const int PB2 = PA_5;
const int PB3 = PA_6;
const int PB4 = PA_7;

// Variables
int contador = 0;

void setup() {
  Serial.begin(9600);  
  Serial1.begin(9600);     
  pinMode(led1r,OUTPUT);
  pinMode(led1v,OUTPUT);
  pinMode(led2r,OUTPUT);
  pinMode(led2v,OUTPUT);
  pinMode(led3r,OUTPUT);
  pinMode(led3v,OUTPUT);
  pinMode(led4r,OUTPUT);
  pinMode(led4v,OUTPUT);

  pinMode(PB1,INPUT);
  pinMode(PB2,INPUT);
  pinMode(PB3,INPUT);
  pinMode(PB4,INPUT);
}

void loop() {
  contador = 0;

  digitalWrite(led1v,HIGH);
  digitalWrite(led1r,LOW);
  if(digitalRead(PB1) == 1){
    contador = contador+1;
    digitalWrite(led1v,LOW);
    digitalWrite(led1r,HIGH);
  }

  digitalWrite(led2v,HIGH);
  digitalWrite(led2r,LOW);
  if(digitalRead(PB2) == 1){
    contador = contador+1;
    digitalWrite(led2v,LOW);
    digitalWrite(led2r,HIGH);
  }
  
  digitalWrite(led3v,HIGH);
  digitalWrite(led3r,LOW);
  if(digitalRead(PB3) == 1){
    contador = contador+1;
    digitalWrite(led3v,LOW);
    digitalWrite(led3r,HIGH);
  }
  
  digitalWrite(led4v,HIGH);
  digitalWrite(led4r,LOW);
  if(digitalRead(PB4) == 1){
    contador = contador+1;
    digitalWrite(led4v,LOW);
    digitalWrite(led4r,HIGH);
  }

  Serial.println(contador);
  if(contador == 0){Serial1.println("a");}
  if(contador == 1){Serial1.println("b");}
  if(contador == 2){Serial1.println("c");}
  if(contador == 3){Serial1.println("d");}
  if(contador == 4){Serial1.println("e");}
} 
