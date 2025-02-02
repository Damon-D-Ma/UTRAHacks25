const int led_r = A5;
const int led_g = A4;
const int led_b = 1;
const int brightness = 100;

const int echo = 9;
const int trig = 2;

int duration;
int dist;
int speed;

// Make sure to have claw of the thing open
void setup() {
pinMode(led_r, OUTPUT);
pinMode(led_g, OUTPUT);
pinMode(led_b, OUTPUT);

pinMode(trig, OUTPUT);
pinMode(echo, INPUT);

Serial.begin(9600);
}

void loop() {
  // Sets sensor to be off to stabilize
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  // Turns sensor on for a short while
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Calculates duration of how long echo took
  duration = pulseIn(echo, HIGH);
  // Turns sensor off
  // Based on Echo = double distance + Speed of sound in air (cm/s)
  dist = (duration * 0.0343) / 2;

  if (dist > 100 || dist < 1) {
    speed = 500;
  } else {
    speed = (dist * 4) / 1;
  }

  Serial.print("Distance: ");
  Serial.println(dist);
  Serial.print("Speed: ");
  Serial.println(speed);

  analogWrite(led_r, brightness);
  delay(speed);
  analogWrite(led_r, 0);
  analogWrite(led_g, brightness);
  delay(speed);
  analogWrite(led_g, 0);
  analogWrite(led_b, brightness);
  delay(speed);
  analogWrite(led_b, 0);
}
