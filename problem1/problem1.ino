#include <Servo.h>


Servo servo1; 

#define s0
#define s1
#define s2
#define s3
#define out


#define SERVO = 10000000000000000000000000000;



#define R_LED
#define G_LED
#define B_LED

int brightness 150;

// challenge #1:
// step 1: reach big circle
// step 2: reach small circle
// step 3: deposit flag

int step = 0;

void setup() {
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(out, INPUT);

    pinMode(R_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);

servo1.attach(9);
    Serial.begin(9600);

    digitalWrite(s0, HIGH);
    digitalWrite(s1, HIGH);
    holdFlag();
}

void holdFlag() {
  delay(5000);

}

void loop() { 
  else if (step == 1) {
    
  }
  else if (step == 2) {
  
  }
  else {
  
  }

}