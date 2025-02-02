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

void setup() {
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
	// Setup Serial Monitor
	Serial.begin(9600);

}

void loop() {
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
    Serial.println("Color: BLACK");
  } else if (max3(redColor, blueColor, greenColor) == redColor) {
    Serial.println("Color: RED");
  } else if (max3(redColor, blueColor, greenColor) == greenColor) {
    Serial.println("Color: GREEN");
  } else if (max3(redColor, blueColor, blueColor) == blueColor) {
    Serial.println("Color: BLUE");
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


