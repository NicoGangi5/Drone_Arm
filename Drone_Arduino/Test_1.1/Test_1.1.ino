#include<Servo.h>


// ANTES QUE NADA TENGO QUE PROBAR EL MOTOR CON EL PWM PARA VER LOS VALORES QUE LO MANTIENEN

//------------------------------VARIABLES Y FUNCIONES------------------------------
void initHardware();
float Medir(int puerto);
float Angle(float Value);
void Act_PWM(int Speed);
void Speed_Up();
void Speed_Down();
void start();


Servo ESC; //Crear un objeto de clase servo


int Eje_z = A0;
int Sleep = 6;
int GS = 7;
int motor = 9;
int led = 13;

int Vel_start = 1270;            //Ver a que velocidad enciende el motor
float Value_z = 0.0;
float Angle_t = 0.0;

float Data_User = 60.0;          //Variable usada para hardcodear un valor de angulo
int PWM_speed = 0;              //Va a ser usada como "bandera" de la velocidad actual del motor

//------------------------------SETUP------------------------------
void setup() {
  initHardware();
  start();

  Serial.println("..........WELCOME..........");
}


//------------------------------LOOP------------------------------
void loop() {

  Value_z = Medir(Eje_z);
  Serial.print("Valor Z: ");Serial.println(Value_z);

  Angle_t = Angle(Value_z);
  Serial.print("Angulo: ");Serial.println(Angle_t);

  Serial.println();


  if (Data_User > Angle_t){
    // Tengo que subir
    Speed_Up();
    Serial.println("Subiendo velocidad");
  }
  else if(Data_User < Angle_t){
    // Tengo que bajar
    Speed_Down();
    Serial.println("Bajando velocidad");
  }
  else if(Data_User == Angle_t){
    Serial.println("Estabilizado");
  }


  Serial.print("El PWM esta en: ");Serial.println(PWM_speed);

  Serial.println();
  //delay(1000);
}







//------------------------------FUNCIÓN initHardware------------------------------
void initHardware(){
  Serial.begin(9600);
  Serial.setTimeout(10);

  pinMode(Eje_z, INPUT);
  pinMode(Sleep, OUTPUT);         //Sleep
  pinMode(GS, OUTPUT);            //Sense Select

  digitalWrite(Sleep, HIGH);
  digitalWrite(GS, LOW);

  pinMode(motor, OUTPUT);

  //Asignar un pin al ESC
  ESC.attach(motor);

  //Activar el ESC
  ESC.writeMicroseconds(1000); //1000 = 1ms
  //Cambia el 1000 anterior por 2000 si
  //tu ESC se activa con un pulso de 2ms
  delay(5000); //Esperar 5 segundos para hacer la activacion

  pinMode(led, OUTPUT);
}

//------------------------------FUNCIÓN initHardware------------------------------
void start(){
  for (int i = 0; i < Vel_start; i++) {
    ESC.writeMicroseconds(PWM_speed);
  }
  digitalWrite(led, LOW);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
}

//------------------------------FUNCIÓN Medir------------------------------
float Medir(int puerto){
  float Value = 0.0;

  Value = analogRead(puerto);
  Serial.print("Medida z: ");
  Serial.println(Value);


  Value = Value * 488;
  Value = Value / 1000;

  return Value;
 }

//------------------------------FUNCIÓN Angle------------------------------
float Angle(float Value){
  float aux = 0.0;
  float ang = 0.0;

  aux = Value - 165;
  if (aux > 85){
    aux = 85;
  }
  if(aux < 0){
    aux = 0;
  // }

  ang = (aux * 100) /60;

  return ang;
}

//------------------------------FUNCIÓN Speed_Up------------------------------
void Speed_Up(){
  if (PWM_speed > 1550) {               //Velocidad que asegure que no se va a la bosta
   PWM_speed = 1550;
  }
  else{
   PWM_speed++;
  }
  delay(2);
  ESC.writeMicroseconds(PWM_speed);
}


//------------------------------FUNCIÓN Speed_Up------------------------------
void Speed_Down(){
  if (PWM_speed < 1270) {               //Velocidad que asegure que no se va a la bosta
    PWM_speed = 1270;
  }
  else{
    PWM_speed--;
  }
  delay(2);
  ESC.writeMicroseconds(PWM_speed);
}
