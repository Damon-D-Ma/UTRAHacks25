#include <Servo.h>

Servo servo; 

#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 3

int curr_colour = -1; 
int prev_colour = -1;


unsigned long t0;
unsigned long t1;

// MOTOR 1 IS RIGHT SIDE
// MOTOR 2 IS LEFT SIDE

#define SERVO_IN 5   // PWM enabled
#define DRIVER_ENA 6   // PWM enabled
#define DRIVER_IN1 7
#define DRIVER_IN2 8
#define DRIVER_IN3 9   // PWM enabled
#define DRIVER_IN4 10  // PWM enabled
#define DRIVER_ENB 11  // PWM enabled

// Pin Layout
const int color_S0  = 11;
const int color_S1  = 12;
const int color_OE  = 13;
const int color_OUT = A0;
const int color_S2  = A1;
const int color_S3  = A2;

// Empirically found values: 
const int red_low = 61;
const int red_high = 83;
const int green_low = 115;
const int green_high = 227;
const int blue_low = 83;
const int blue_high = 240;
const int white_low = 83;
const int white_high = 240;

int redColor;
int greenColor;
int blueColor;
int whitePW;

int brightness = 150;


void setup() {
    Serial.begin(9600);

    // Set S0 - S3 as outputs
    pinMode(color_S0, OUTPUT);
    pinMode(color_S1, OUTPUT);
    pinMode(color_S2, OUTPUT);
    pinMode(color_S3, OUTPUT);
    pinMode(color_OE, OUTPUT);
    // Set Sensor output as input
    pinMode(color_OUT, INPUT);
    // Set Pulse Width scaling to 20%
    digitalWrite(color_S0, HIGH);
    digitalWrite(color_S1, LOW);
    digitalWrite(color_OE, LOW);

    servo.attach(SERVO_IN);
    servo.write(25);
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
  //drive backwards a little bit, assuming that the
  // colour sensor is in the middle under the car
  // and the clamp is in the front
  analogWrite(DRIVER_ENA, 255);
  analogWrite(DRIVER_ENB, 255);
  motorControl(LOW, HIGH, LOW, HIGH);
  delay(1000); //TODO: fine tune this to make sure it backs up an appropriate amount

  //open clamp to drop
  servo.write(25);
}

void start(){
  setupFlag(); //clamp holds on to flag
  findCircle(); //find the outermost ring
  findCentre(); //traverse to centre
  alignPoint(); //get to the middle of the final ring/cicle
  dropFlag(); // back up a little and release clamp
}

void findCircle(){
  int s_right = 255;
  int s_left = 1;
  while (curr_colour != BLACK){
    analogWrite(DRIVER_ENA, s_right);
    analogWrite(DRIVER_ENB, s_left);

    //move for a bit
    motorControl(HIGH, LOW, HIGH, LOW);

    // slightly increase the left wheel speed so the
    // car continues to drive in a circle
    if (s_left < 255){
      s_left++; //this could be a problem if we reach max speed and still didn't find anything.
    }

    //check colour to see if we reached a ring
    curr_colour = getColour();
    delay(20); // TODO: FINE TUNE THIS TO GET BEST BALANCE BETWEEN PRECISION AND SPEEED
  }
  //curr_colour is no longer black we found first ring. Done
  // reset wheel speeds
  analogWrite(DRIVER_ENA, 255);
  analogWrite(DRIVER_ENB, 255);
  motorControl(LOW, LOW, LOW, LOW);

}

void findCentre(){
  int rings_found = 1; // Assuming findCircle terminated, we are on the first ring
  prev_colour = BLACK;

  while (rings_found < 5){
    int poll_colour =  getColour();
    // move forward until we see either a new colour, or turn if we see the previous colour 
    while (poll_colour != prev_colour || poll_colour != curr_colour){
        analogWrite(DRIVER_ENA, 255);
        analogWrite(DRIVER_ENB, 255);
        motorControl(HIGH, LOW, HIGH, LOW);
        poll_colour = getColour();
        if (poll_colour == prev_colour){
            //stop
            motorControl(LOW, LOW, LOW, LOW);
            delay(100);
            //pivot by making the right wheel go forward, the left goes backwards
            motorControl(HIGH, LOW, LOW, HIGH);
            delay(500); //TODO: tune the timing for the right amount of turn
            //resume normal operation
            motorControl(HIGH, LOW, HIGH, LOW);
        }
        delay(20);
    }
    prev_colour = curr_colour;
    curr_colour = poll_colour;
    rings_found++;
  }

  // reset wheel speeds
  analogWrite(DRIVER_ENA, 255);
  analogWrite(DRIVER_ENB, 255);
  motorControl(LOW, LOW, LOW, LOW);
}


// Function to control motors properly
void motorControl(int state1, int state2, int state3, int state4) {
  digitalWrite(DRIVER_IN1, state1);
  digitalWrite(DRIVER_IN2, state2);
  digitalWrite(DRIVER_IN3, state3);
  digitalWrite(DRIVER_IN4, state4);
}


/**
1. Go forward till we detect colour from previous ring and record the time it took to do this 
2. turn left, then do one of the following:
  scenario 1: if the time to the end of the cycle is higher than the previous, turn left and 
  find out how much time it takes to drive to the edge of the circle. Do this until it stops 
  increase. Stop once the time is smaller. Turn back to the previous position

  scenario 2: if the time is less, do the same as scenario 2: but in the right direction.

3. for half of the recorded time, drive forward and stop, the sensor is now in the middle 
and we can start flag dropping procedure
*/
void alignPoint(){
  //now in the final ring/circle
  unsigned long longest_time = 0;
  unsigned long curr_time = 0;
  analogWrite(DRIVER_ENA, 255);
  analogWrite(DRIVER_ENB, 255);
  bool move_left = true;
  //get benchmark time of our current direction
  longest_time = getLength();
  delay(100);


  //check if running along the distance slightly left of us is longer (more time = longer)
  pivot(true);
  curr_time = getLength();

  move_left = (curr_time > longest_time);

  curr_time = longest_time;

  while (curr_time >= longest_time){
    pivot(move_left);
    curr_time = getLength();
    if (curr_time > longest_time){
      longest_time = curr_time;
    }
  }

  //pivot one back since we did not reset to get to the angle with the longest length

  pivot(!move_left);

  //now travel half the distance to get to the centre point now that we're in the correct angle
  
  motorControl(HIGH, LOW, HIGH, LOW);
  delay(longest_time/2);
  motorControl(LOW, LOW, LOW, LOW);

}



unsigned long getLength(){
  unsigned long t_end;
  unsigned long t_start = millis(); 
  int poll_colour = curr_colour;

  motorControl(HIGH, LOW, HIGH, LOW);
    while (poll_colour == curr_colour){
    poll_colour = getColour();
    delay(20); //TODO: finetune this value for precision vs runtime
  }
  motorControl(LOW, LOW, LOW, LOW);
  t1 = millis();
  unsigned long time = t1 - t0;

  //back up to original point
  delay(100);

  motorControl(LOW, HIGH, LOW, HIGH);
  delay(time);
  motorControl(LOW, LOW, LOW, LOW);

  return time;
}


void pivot (bool go_left){
  int pivot_length = 50;
  if (go_left){
    motorControl(LOW, HIGH, HIGH, LOW);
  }else{
    motorControl(HIGH, LOW, LOW, HIGH);
  }
  delay(50);
  motorControl(LOW, LOW, LOW, LOW);
}

void loop(){
  //nothing to do here, NOP
}

int getColour(){
  int prev = pollColour();
  int curr = pollColour();

  while (prev != curr){
    prev = curr; 
    curr = pollColour();
  }
  return curr;
}


int pollColour(){

  // get color Pw gets the Pulse Frequency of the sensor's image for that color
  // This function then inverts it and puts it onto a linear map to normalize.
  // Visible colour is the max vaue amoung the three.
  redColor = map(getRedPW(), red_low, red_high, 255, 0);
	delay(50);
  greenColor = map(getGreenPW(), green_low, green_high, 255, 0);
	delay(50);
  blueColor = map(getBluePW(), blue_low, blue_high, 255, 0);
	delay(50);
  whitePW = getWhitePW();

  if (whitePW > 85) {
    return BLACK;
  } else if (max3(redColor, blueColor, greenColor) == redColor) {
    return RED;
  } else if (max3(redColor, blueColor, greenColor) == greenColor) {
    return GREEN;
  } else if (max3(redColor, blueColor, blueColor) == blueColor) {
    return BLUE;
  }


}






// Being Lazy
int max3(int n1, int n2, int n3) {
  return max(n1, max(n2, n3));
}

// Function to read Red Pulse Widths
int getRedPW() {
	// Set sensor to read Red only
	digitalWrite(color_S2, LOW);
	digitalWrite(color_S3, LOW);
  delay(10);
	int PW = pulseIn(color_OUT, LOW);
	return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
	// Set sensor to read Blue only
	digitalWrite(color_S2, LOW);
	digitalWrite(color_S3, HIGH);
  delay(10);
	int PW = pulseIn(color_OUT, LOW);
	return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
	// Set sensor to read Green only
	digitalWrite(color_S2, HIGH);
	digitalWrite(color_S3, HIGH);
  delay(10);
	int PW = pulseIn(color_OUT, LOW);
	return PW;
}

int getWhitePW() {
	// Set sensor to read Overall colour
	digitalWrite(color_S2, HIGH);
	digitalWrite(color_S3, LOW);
  delay(10);
	int PW = pulseIn(color_OUT, LOW);
	return PW;
}

