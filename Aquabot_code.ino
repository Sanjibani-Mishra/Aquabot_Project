#include <NewPing.h>
#include <Servo.h>

// Motor pins
const int motor1Pin1 = 2; // Motor 1 forward
const int motor1Pin2 = 3; // Motor 1 backward
const int motor2Pin1 = 4; // Motor 2 forward
const int motor2Pin2 = 5; // Motor 2 backward

// Ultrasonic sensor pins
#define TRIGGER_PIN 6
#define ECHO_PIN 7
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Servo pin
Servo debrisServo;
const int servoPin = 8;

// Movement speeds
const int motorSpeed = 255;

// Function to control motors
void setMotor(int motor, int speed) {
  bool forward = speed > 0;
  speed = abs(speed);

  if (motor == 1) {
    analogWrite(motor1Pin1, forward ? speed : 0);
    analogWrite(motor1Pin2, forward ? 0 : speed);
  } else if (motor == 2) {
    analogWrite(motor2Pin1, forward ? speed : 0);
    analogWrite(motor2Pin2, forward ? 0 : speed);
  }
}

// Function to move forward
void moveForward() {
  setMotor(1, motorSpeed);
  setMotor(2, motorSpeed);
}

// Function to stop
void stopMotors() {
  setMotor(1, 0);
  setMotor(2, 0);
}

// Function to turn
void turnRight() {
  setMotor(1, motorSpeed);
  setMotor(2, -motorSpeed);
}

// Setup function
void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  debrisServo.attach(servoPin);
  debrisServo.write(90); // Initial position

  Serial.begin(9600);
}

// Loop function
void loop() {
  // Get distance from ultrasonic sensor
  unsigned int distance = sonar.ping_cm();

  // If an obstacle is detected
  if (distance > 0 && distance < 30) {
    stopMotors();
    delay(500);
    turnRight();
    delay(500);
    stopMotors();
    delay(500);
  } else {
    moveForward();
  }

  // Control the debris servo
  if (millis() % 5000 < 2500) {
    debrisServo.write(45); // Lower position
  } else {
    debrisServo.write(90); // Initial position
  }
}
