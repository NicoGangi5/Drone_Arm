#include<Servo.h>


/* ANTES QUE NADA TENGO QUE PROBAR EL MOTOR CON EL PWM PARA VER LOS VALORES QUE LO MANTIENEN
*
* La velocidad del motor puede cambiarse enviando
* un entero entre 1000 (vel. minima) y 2000 (vel. max.)
* por Serial.
*
*///------------------------------VARIABLES Y FUNCIONES------------------------------
void initHardware();
float Medir(int puerto);
int Escala_PWM (int Cambiemos);

Servo ESC; //Crear un objeto de clase servo

int motor = 9;
int led = 13;
int pote = A0;

float Value_pote = 0.0;

//------------------------------SETUP------------------------------
void setup() {
  initHardware();

  Serial.println("..........WELCOME..........");
}


//------------------------------LOOP------------------------------
void loop() {

      Value_pote = Medir(pote);
      Serial.print("Valor del pote: ");
      Serial.println(Value_pote);

      Value_pote = Escala_PWM (Value_pote);
      Serial.print("Valor del PWM del pote: ");
      Serial.println(Value_pote);

      ESC.writeMicroseconds(Value_pote); //Generar un pulso con el numero recibido

      delay(1000);
}







//------------------------------FUNCIÓN initHardware------------------------------
void initHardware(){
  //Asignar un pin al ESC
  ESC.attach(motor);

  //Activar el ESC
  ESC.writeMicroseconds(1000); //1000 = 1ms
  //Cambia el 1000 anterior por 2000 si
  //tu ESC se activa con un pulso de 2ms
  delay(5000); //Esperar 5 segundos para hacer la activacion

  //Iniciar puerto serial
  Serial.begin(9600);
  Serial.setTimeout(10);

  pinMode(led, OUTPUT);
  pinMode(pote, INPUT);
}


float Medir(int puerto){
  float Value = 0.0;

  Value = analogRead(puerto);
  // Serial.print("Medida z: ");
  // Serial.println(Value);


  Value = Value * 488;
  Value = Value / 1000;

  return Value;
 }



 int Escala_PWM (int Cambiemos){
   int Value = 0;

   Value = Cambiemos * 10;
   Value = Value / 5;
   Value = Value + 1000;

   return Value;
 }
