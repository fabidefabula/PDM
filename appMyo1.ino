//Fabiola Fernández Sánchez - PDM 2015/16 

// Esta aplicación realiza el control de un robot autónomo motorizado 
//(DG012-EV Multi Chasis tank,Explore Version) mediante bluetooth. 
//La lógica del robot está compuesta por Arduino Uno, una shield bluetooth de RedBeardLab
//(BLE Shield Version 2.3) y una shield para el control de los servomotores
//(Adafruit Motor/Stepper/Servo Shield for Arduino v2 Kit-2.3) El control del mismo se 
//realiza de forma inalámbrica mediante un brazalete Myo, que emparejado con el Arduino, 
//gracias al programa MyoDuino,permite el reconocimiento gestual del usuario para controlar
//el movimiento del robot. *En el tutorial 1 se explica cómo realizar el control del robot y
//y la programación del movimiento de los motores*

#include <MyoController.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


#define FIST_PIN 4
#define WAVEIN_PIN 5
#define WAVEOUT_PIN 6
#define FINGERSSPREAD_PIN 7
#define DOUBLETAP_PIN 8

MyoController myo = MyoController(); //Creamos un objeto Myo
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //Creamos el objeto que controla
//las funciones de la shield de Adafruit

Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2); //Definimos los motores 2 y 3
//(Conectados a M2 y M3 respectivamente en la placa Arduino) , cada uno controla un laterial del coche
Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);

//Creamos las funciones de movimientos básicos del robot, hacia adelante, hacia atrás y parar.
void moverAdelante(){
  myMotor2->run(FORWARD);
  myMotor3->run(FORWARD);
}
void moverAtras(){
   myMotor2->run(BACKWARD);
   myMotor3->run(BACKWARD);
}
void parar(){
    myMotor2->run(RELEASE);
    myMotor3->run(RELEASE);
}

void setup() {
//Definimos la función de los pines de Arduino
  pinMode(FIST_PIN, OUTPUT);
  pinMode(WAVEIN_PIN, OUTPUT);
  pinMode(WAVEOUT_PIN, OUTPUT);
  pinMode(FINGERSSPREAD_PIN, OUTPUT);
  pinMode(DOUBLETAP_PIN, OUTPUT);
  
  myo.initMyo();
    AFMS.begin();  //Indicamos el inicio del objeto de la shield para los motores, la frecuencia 
    //será por defecto 1.6KHz
  
  ///Definimos la velocidad de los motores de 0 a 255(máxima velocidad)
  myMotor2->setSpeed(100);
  myMotor3->setSpeed(100);
  myMotor2->run(FORWARD);
  myMotor3->run(FORWARD);
  
  myMotor2->run(RELEASE);
  myMotor3->run(RELEASE);
}

void loop()
{
   myo.updatePose();
   //En cada loop actualizamos la posición captada por Myo.
   switch ( myo.getCurrentPose() ) {//Con este switch hacemos que el coche realice un movimiento
    //en función del gesto detectado.
    case rest:
      digitalWrite(FIST_PIN,LOW); 
      digitalWrite(WAVEIN_PIN,LOW);
      digitalWrite(WAVEOUT_PIN,LOW);
      digitalWrite(FINGERSSPREAD_PIN,LOW);
      digitalWrite(DOUBLETAP_PIN,LOW);
      break;
    case fist:
      digitalWrite(FIST_PIN,HIGH);
      parar();
      break;
    case waveIn:
      digitalWrite(WAVEIN_PIN,HIGH);
   moverAdelante();
      break;
    case waveOut:
      digitalWrite(WAVEOUT_PIN,HIGH);
  moverAtras();
      break;
    case fingersSpread:
      digitalWrite(FINGERSSPREAD_PIN,HIGH);
    moverAtras();
      break;
    case doubleTap:
      digitalWrite(DOUBLETAP_PIN,HIGH);
      moverAdelante();
      break;
   } 
   delay(100);
}
