//PINS

// light sensor
const int color_S0  = 11;
const int color_S1  = 12;
const int color_OE  = 13;
const int color_OUT = A0;
const int color_S2  = A1;
const int color_S3  = A2;

// light empirical values
const int red_low = 61;
const int red_high = 83;
const int green_low = 115;
const int green_high = 227;
const int blue_low = 83;
const int blue_high = 240;
const int white_low = 83;
const int white_high = 240;

// motors right then left
const int DRIVER_ENA = 5;   // PWM enabled
const int DRIVER_IN1 = 6;
const int DRIVER_IN2 = 7;
const int DRIVER_IN3 = 8;  // PWM enabled
const int DRIVER_IN4 = 4;  // PWM enabled
const int DRIVER_ENB = 10;  // PWM enabled

// leds
const int R_LED = A5;
const int G_LED = A4;
const int B_LED = A3;

// sonar
const int trig = 2;
const int echo = 9;

const int bright = 150;
const int turnDur = 100;
bool skip = false;
int dist;

void setup() {
  // light sensor
	pinMode(color_S0, OUTPUT);
	pinMode(color_S1, OUTPUT);
	pinMode(color_S2, OUTPUT);
	pinMode(color_S3, OUTPUT);
  pinMode(color_OE, OUTPUT);
	pinMode(color_OUT, INPUT);
	digitalWrite(color_S0, HIGH);
	digitalWrite(color_S1, LOW);
  digitalWrite(color_OE, LOW);

  // LEDs
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);

  // sonar
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // motor
  pinMode(DRIVER_IN1, OUTPUT);
  pinMode(DRIVER_IN2, OUTPUT);
  pinMode(DRIVER_IN3, OUTPUT);
  pinMode(DRIVER_IN4, OUTPUT);
  pinMode(DRIVER_ENA, OUTPUT);
  pinMode(DRIVER_ENB, OUTPUT);
  analogWrite(DRIVER_ENA, 128);
  analogWrite(DRIVER_ENB, 128);

  Serial.begin(9600);
}

void loop() { 
  if (!skip) {
    getDistance();
    if (dist > 10) {
      stop();
      int color = voteColor();
      
      if (color == 1) {
        turnColor(bright,0,0);
        turn(2); // uturn
      }
      else if (color == 2) {
        turnColor(0,0,bright);
        turn(1); // anti 90
      }
      else if (color == 3) {
        turnColor(0,bright,0);
        turn(3); // clockwise 90
      }
      else {
        turnColor(0,0,0);
      }
    }

    else {
      motorControl(HIGH, LOW, HIGH, LOW);
      delay(10);
    }
  }
}

void getDistance() {
  int duration = pulseIn(echo, HIGH);
  dist = (duration * 0.0343) / 2;
}

void stop() {
  skip = true;
  analogWrite(R_LED, bright);
  analogWrite(G_LED, bright);
  analogWrite(B_LED, bright);
  motorControl(LOW, LOW, LOW, LOW);
}

void turn(int angle) {
  skip = true;
  if (angle == 3) {
    motorControl(LOW, HIGH, HIGH, LOW);
  }
  else {
    motorControl(HIGH, LOW, LOW, HIGH);
  }
  if (angle == 2) {
    delay(2 * turnDur);
  }
  else {
    delay(turnDur);
  }
  motorControl(LOW, LOW, LOW, LOW);
  skip = false;
}

bool wall() {
  if (dist > 10) {
    return true;
  }
  return false;
}

void turnColor(int r, int g, int b) {
  analogWrite(R_LED, r);
  analogWrite(G_LED, g);
  analogWrite(B_LED, b);
}

int getColor() {
  // get color Pw gets the Pulse Frequency of the sensor's image for that color
  // This function then inverts it and puts it onto a linear map to normalize.
  // Visible colour is the max vaue amoung the three.
  int redColor = map(getRedPW(), red_low, red_high, 255, 0);
	delay(50);
  int greenColor = map(getGreenPW(), green_low, green_high, 255, 0);
	delay(50);
  int blueColor = map(getBluePW(), blue_low, blue_high, 255, 0);
	delay(50);
  int whitePW = getWhitePW();

  if (whitePW > 85) {
    return 0;
  } else if (max3(redColor, blueColor, greenColor) == redColor) {
    return 1;
  } else if (max3(redColor, blueColor, greenColor) == greenColor) {
    return 2;
  } else if (max3(redColor, blueColor, blueColor) == blueColor) {
    return 3;
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

void motorControl(int state1, int state2, int state3, int state4) {
  digitalWrite(DRIVER_IN1, state1);
  digitalWrite(DRIVER_IN2, state2);
  digitalWrite(DRIVER_IN3, state3);
  digitalWrite(DRIVER_IN4, state4);
}

int voteColor() {
  int redColor = 0;
  int greenColor = 0;
  int blueColor = 0;
  int blackColor = 0;
  int color;
  while (redColor < 5 && greenColor < 5 && blueColor < 5 && blackColor < 5) {
    color = getColor();
    if (color == 1) {
      redColor++;
    }
    else if (color == 2) {
      greenColor++;
    }
    else if (color == 3) {
      blueColor++;
    }
    else {
      blackColor++;
    }
  }
  return color;
}