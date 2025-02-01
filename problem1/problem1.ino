#include <Servo.h>

Servo servo; 

#define BLACK = 0;
#define RED = 1;
#define GREEN = 2;
#define BLUE = 3;

int curr_colour = -1; 

unsigned long t0;
unsigned long t1;

// MOTOR 1 IS RIGHT SIDE
// MOTOR 2 IS LEFT SIDE

#define SERVO_IN = 5;   // PWM enabled
#define DRIVER_ENA = 6;   // PWM enabled
#define DRIVER_IN1 = 7;
#define DRIVER_IN2 = 8;
#define DRIVER_IN3 = 9;   // PWM enabled
#define DRIVER_IN4 = 10;  // PWM enabled
#define DRIVER_ENB = 11;  // PWM enabled






//placeholder values until we figure out wiring
// #define s0 = 1000;
// #define s1 = 10000;
// #define s2 = 100000;
// #define s3 = 10000000;
// #define out = 100000000;
// #define SERVO = 1000000000;

// #define R_LED
// #define G_LED
// #define B_LED

int brightness 150;


void setup() {
    Serial.begin(9600);

    // pinMode(s0, OUTPUT);
    // pinMode(s1, OUTPUT);
    // pinMode(s2, OUTPUT);
    // pinMode(s3, OUTPUT);
    // pinMode(out, INPUT);

    // pinMode(R_LED, OUTPUT);
    // pinMode(G_LED, OUTPUT);
    // pinMode(B_LED, OUTPUT);

    servo1.attach(SERVO_IN);
    servo.write(0);

    //digitalWrite(s0, HIGH);
    //digitalWrite(s1, HIGH);
    start();
}

void setupFlag() {
  //open clamp
  servo.write(25);
  
  //you have 5 seconds to hold the flag in front of the clamp
  delay(5000);

  //close clamp, depending on size, we may need 
  // to make the pos smaller so we don't crush it
  // or larger so it's not too loose
  servo.write(100);
}

void dropFlag(){
  //TODO: do backup procedure here, not sure what the steps are yet

  //buffer to make sure that the car is fully stopped
  delay(1000);
  //open clamp to drop
  servo1.write(25);
}

void start(){
  setupFlag(); //clamp holds on to flag
  findCircle(); //find the outermost ring
  findCentre(); //traverse to centre
  dropFlag(); // back up a little and release clamp
}



void findCircle(){
  int s_right = 255;
  int s_left = 1;
  while (curr_colour != BLACK){
    analogWrite(DRIVER_ENA, s_right);
    analogWrite(DRIVER_ENB s_left);

    //move for a bit
    motorControl(HIGH, LOW, HIGH, LOW);

    // slightly increase the left wheel speed so the
    // car continues to drive in a circle
    if (s_left < 255){
      s_left++;
    }

    //check colour to see if we reached a ring
    curr_colour = pollColour();
    delay(20); // TODO: FINE TUNE THIS TO GET BEST BALANCE BETWEEN PRECISION AND SPEEED
  }
  //curr_colour is no longer black we found first ring. Done
  // reset wheel speeds
  analogWrite(DRIVER_ENA, 255);
  analogWrite(DRIVER_ENB 255);
}

void findCentre(){
  int rings_found = 1; // Assuming findCircle terminated, we are on the first ring





}


// Function to control motors properly
void motorControl(int state1, int state2, int state3, int state4) {
  digitalWrite(DRIVER_IN1, state1);
  digitalWrite(DRIVER_IN2, state2);
  digitalWrite(DRIVER_IN3, state3);
  digitalWrite(DRIVER_IN4, state4);
}

void loop(){
  //nothing to do here, NOP
}




int pollColour(){
  //placeholder function for the colour sensor
  return -1;
}