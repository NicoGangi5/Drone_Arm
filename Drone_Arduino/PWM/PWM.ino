/* Programacion de un ESC con Arduino
 *
 * La velocidad del motor puede cambiarse enviando
 * un entero entre 1000 (vel. minima) y 2000 (vel. max.)
 * por Serial.
 *
 */
#include<Servo.h>

int Cambio_Escala(int Value);

Servo ESC; //Crear un objeto de clase servo

int vel = 1000; //amplitud del pulso


void setup()
{
  //Asignar un pin al ESC
  ESC.attach(13);                  //Cambiar por pin 9, pin 13 solo para probar
  pinMode(13, OUTPUT);

  //Activar el ESC
  ESC.writeMicroseconds(1000); //1000 = 1ms
  //Cambia el 1000 anterior por 2000 si
  //tu ESC se activa con un pulso de 2ms
  delay(5000); //Esperar 5 segundos para hacer la activacion

  //Iniciar puerto serial
  Serial.begin(9600);
  Serial.setTimeout(10);


}


void loop()
{
  if(Serial.available() >= 1)
  {
    Serial.println("Introduzca la velodidad(Entre 1000 y 2000)");
    vel = Serial.parseInt(); //Leer un entero por serial

    vel = Cambio_Escala(vel);

    if(vel != 0)
    {
      ESC.writeMicroseconds(vel); //Generar un pulso con el numero recibido
    }
  }
}




int Cambio_Escala(int Value){
  Value = Value - 1000;
  Value = (Value * 10) / 1000;

  return Value;
}
