#include <NewPing.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 400

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(19200);  // ИЗМЕНЕНО С 9600 НА 19200
  Serial.println("HC-SR04 Ultrasonic Sensor Started");
}

void loop() {
  unsigned int distance_cm = sonar.ping_cm();
  
  if (distance_cm > 0) {
    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");
  } else {
    Serial.println("Out of range");
  }
  
  delay(500);
}