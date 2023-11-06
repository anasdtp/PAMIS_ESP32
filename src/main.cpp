#include <Arduino.h>
#include <stepper_robot.h>


void setup(void) {
  Serial.begin(921600);
  initDriver();
  stepper1.m0 = 0;
  stepper2.m0 = 0;
}

void loop() {
  Ligne_droite(10000);
  delay(2000);
 
}
