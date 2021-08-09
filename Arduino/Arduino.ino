#include <Wire.h>

#define SLAVE_ADDR 1 

// Defining the connections for the two DC motors.
int enA = 3;
int in1 = 9;
int in2 = 8;

// Initializing the response obtained from master.
String response = "";


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
  
  response = "";                // Resets the response.

  while (Wire.available()) {    // Recieves the response.
      char b = Wire.read();
      response += b;
  } 
 
  Serial.println("Received command.");
}


void directionControl() {           
	// Set motors to maximum speed.
	// For PWM maximum possible values are 0 to 255.
	analogWrite(enA, 255);

	// Turn on motor A.
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	delay(2000);
	
	// Now change motor directions.
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	delay(2000);
	
	// Turn off motors.
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

void speedControl() {
	// Turn on motor A.
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	
	// Accelerate from zero to maximum speed.
	for (int i = 25; i < 256; i++) {
		analogWrite(enA, i);
		delay(50);
	}
	
	// Decelerate from maximum speed to zero.
	for (int i = 255; i >= 25; --i) {
		analogWrite(enA, i);
		delay(50);
	}
	
	// Now turn off motor.
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

void loop() {

  delay(50);

  if(response == "speed") {                           // Depending on the command given by master, speed or control directions are activated.
    Serial.println("Speed control activated.");    
    speedControl();
    response = "";
  } 
  else if (response == "direction") {
    Serial.println("Direction control activated.");
    directionControl();
    response = "";
  } 
}

