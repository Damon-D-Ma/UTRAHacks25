#include <Servo.h>

const int servo_IN = 5;   // PWN enabled
int driver_ENA = 6;   // PWM enabled
int driver_IN1 = 7;
int driver_IN2 = 8;
int driver_IN3 = 9;   // PWM enabled
int driver_IN4 = 10;  // PWM enabled
int driver_ENB = 11;  // PWM enabled
Servo servo;

void setup() {
  Serial.begin(9600);

  // Motor driver setup
  pinMode(driver_ENA, OUTPUT);
  pinMode(driver_ENB, OUTPUT);
  pinMode(driver_IN1, OUTPUT);
  pinMode(driver_IN2, OUTPUT);
  pinMode(driver_IN3, OUTPUT);
  pinMode(driver_IN4, OUTPUT);

  // Enable motor driver at full speed
  analogWrite(driver_ENA, 255);
  analogWrite(driver_ENB, 255);

  // Servo setup
  servo.attach(servo_IN);
  servo.write(0);


  delay(2000);

  motorControl(HIGH, LOW, LOW, LOW);
  delay(3000);
  motorControl(LOW, LOW, LOW, LOW);


  servo.write(180);
}

void loop() {
  // // Motor 1 forward
  // motorControl(HIGH, LOW, LOW, LOW);
  // delay(3000);

  // // Motor 1 backward
  // motorControl(LOW, HIGH, LOW, LOW);
  // delay(3000);

  // // Motor 2 forward
  // motorControl(LOW, LOW, HIGH, LOW);
  // delay(3000);

  // // Motor 2 backward
  // motorControl(LOW, LOW, LOW, HIGH);
  // delay(3000);

  //Testing servo
  Serial.println("Servo 180");
  for (int i = 0; i <= 180; i++) {
    servo.write(i);
    delay(15);
  }
  Serial.println("Servo 0");
  for (int i = 180; i >= 0; i -= 1) {
    servo.write(i);
    delay(15);
  }
}

// Function to control motors properly
void motorControl(int state1, int state2, int state3, int state4) {
  digitalWrite(driver_IN1, state1);
  digitalWrite(driver_IN2, state2);
  digitalWrite(driver_IN3, state3);
  digitalWrite(driver_IN4, state4);
}
