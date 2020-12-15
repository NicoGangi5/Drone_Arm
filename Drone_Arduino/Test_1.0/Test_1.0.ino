
// ANTES QUE NADA TENGO QUE PROBAR EL MOTOR CON EL PWM PARA VER LOS VALORES QUE LO MANTIENEN

//------------------------------VARIABLES Y FUNCIONES------------------------------
void initHardware();
float Medir(int puerto);
float Angle(float Value);
void Act_PWM(int Speed);
void Speed_Up();
void Speed_Down();
void start();



int Eje_z = A1;
int Sleep = 6;
int GS = 7;
int motor = 13;
int led = 12;

int Vel_start = 100;            //Ver a que velocidad enciende el motor
float Value_z = 0.0;
float Angle_t = 0.0;

float Data_User = 0.0;          //Variable usada para hardcodear un valor de angulo
int PWM_speed = 0;              //Va a ser usada como "bandera" de la velocidad actual del motor

//------------------------------SETUP------------------------------
void setup() {
  initHardware();

  Serial.println("..........WELCOME..........");
}


//------------------------------LOOP------------------------------
void loop() {

  Value_z = Medir(Eje_z);
  Serial.print("Valor Z: ");Serial.println(Value_z);

  Angle_t = Angle(Value_z);
  Serial.print("Angulo: ");Serial.println(Angle_t);

  Serial.println();


  Data_User = 45.0;

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

  pinMode(Eje_z, INPUT);
  pinMode(Sleep, OUTPUT);         //Sleep
  pinMode(GS, OUTPUT);            //Sense Select

  digitalWrite(Sleep, HIGH);
  digitalWrite(GS, LOW);

  pinMode(motor, OUTPUT);
}

//------------------------------FUNCIÓN initHardware------------------------------
void start(){
  for (int i = 0; i < Vel_start; i++) {
    analogWrite(motor, i);
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
  }

  ang = (aux * 100) /60;

  return ang;
 }

//------------------------------FUNCIÓN Speed_Up------------------------------
 void Speed_Up(){
   if (PWM_speed > 255) {               //Velocidad que asegure que no se va a la bosta
     PWM_speed = 255;
   }
   else{
     PWM_speed++;
   }
   delay(200);
   analogWrite(motor,PWM_speed);
 }


 //------------------------------FUNCIÓN Speed_Up------------------------------
  void Speed_Down(){
    if (PWM_speed < 1) {               //Velocidad que asegure que no se va a la bosta
      PWM_speed = 1;
    }
    else{
      PWM_speed--;
    }
    delay(200);
    analogWrite(motor,PWM_speed);
  }
