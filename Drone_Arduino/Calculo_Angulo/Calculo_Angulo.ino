
void initHardware();
float Medir(int puerto);
float Angle(float Value);




int Eje_z = A1;
int Sleep = 6;
int GS = 7;


float Value_z = 0.0;
float Angle_t = 0.0;


#define P 165;


void setup() {
  initHardware();

  Serial.println("..........WELCOME..........");
}

void loop() {

  Value_z = Medir(Eje_z);
  Serial.print("Valor Z: ");
  Serial.println(Value_z);

  Angle_t = Angle(Value_z);
  Serial.print("Angulo: ");
  Serial.println(Angle_t);

  delay(2000);
  Serial.println();
}








void initHardware(){
  Serial.begin(9600);

  pinMode(Eje_z, INPUT);
  pinMode(Sleep, OUTPUT);         //Sleep
  pinMode(GS, OUTPUT);            //Sense Select

  digitalWrite(Sleep, HIGH);
  digitalWrite(GS, LOW);

}


float Medir(int puerto){
  float Value = 0.0;

  Value = analogRead(puerto);
  Serial.print("Medida z: ");
  Serial.println(Value);


  Value = Value * 488;
  Value = Value / 1000;

  return Value;
 }


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
