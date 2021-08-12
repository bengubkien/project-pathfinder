#include <Wire.h>

#define SLAVE_ADDR 1 

// Defining the connections for the two DC motors.
int enA = 3;
int in1 = 9;
int in2 = 8;

// Initializing the response obtained from master.
float response = 0;
int speed;
int reverse = 0;

void setup() {

  // Set-up of I2C communications.
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);

  // Set-up of testing through serial monitor.
  Serial.begin(115200);

  // Set all the motor control pins to outputs.
	pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	
	// Initial state of motors is turned off.
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}


void receiveEvent() {
  
  float response = 0;            // Resets the response.
  String aux;

  while (Wire.available()) {    // Recieves the response.
      char b = Wire.read();
      aux += b;
  } 

  response = aux.toFloat();

  if (response > 0) {
    speed = map(int(response*100), 0, 100, 1, 255);
    reverse = 0;
  } 
  else {
    speed = map(int(response*100), -100, 0, 255, 1);
    reverse = 1;
  }
}


void motorAction(int speed) {           
	analogWrite(enA, speed);

	// Turn on motor A.
  if (reverse) {
    digitalWrite(in1, HIGH);
	  digitalWrite(in2, LOW);
  }
  else {
    digitalWrite(in1, LOW);
	  digitalWrite(in2, HIGH);
  }
	
}

void loop() {
  motorAction(speed);
}

