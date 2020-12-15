


void initHardware();
long Cambio(long Value);


int Sleep = 6;
int GS = 7;

long Eje_x = 0;
long Eje_z = 0;

long Eje_x_total = 0;
long Eje_z_total = 0;

float Eje_x_prom = 0;
float Eje_z_prom = 0;

long Example = 0;


void setup(){
	initHardware();

	Example = analogRead(A0);
	Serial.print("Ejemplo de prueba: ");
  Example = Cambio(Example);
	Serial.println(Example);

}


void loop(){

	for(int i=0; i < 10000; i++){

		Eje_x = analogRead(A0);
		Eje_x_total = Eje_x_total + Eje_x;


		Eje_z = analogRead(A1);
		Eje_z_total = Eje_z_total + Eje_z;

	}

	Serial.print("Eje X TOTAL: ");
	Serial.println(Eje_x_total);

	Serial.print("Eje Z TOTAL: ");
	Serial.println(Eje_z_total);




	Eje_x_prom = Eje_x_total / 10000;
	Eje_z_prom = Eje_z_total / 10000;

	Serial.print("Eje X: ");
	Serial.println(Eje_x_prom);

	Serial.print("Eje Z: ");
	Serial.println(Eje_z_prom);



	Serial.println();
	delay(1000);


	Eje_x = 0;
	Eje_z = 0;

	Eje_x_total = 0;
	Eje_z_total = 0;

	Eje_x_prom = 0;
	Eje_z_prom = 0;
	
}








void initHardware(){
	Serial.begin(9600);
	/*
	pinMode(A0, INPUT);					//Eje X
	pinMode(A1, INPUT);					//Eje Z
	*/
	pinMode(Sleep, OUTPUT);					//Sleep
	pinMode(GS, OUTPUT);					//Sense Select

	digitalWrite(Sleep, HIGH);
	digitalWrite(GS, LOW);
}

long Cambio(long Value){
  float Changed = 0.0;

  Changed = Value * (5000.0 / 1023);
  return Changed;
}
