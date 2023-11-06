#include <Arduino.h>
#include <Stepper.h>

#define Rayon_Robot 150. //mm
#define Rayon_roue 30. //mm au hasard pour le moment

extern STEPPER stepper1, stepper2;

void initDriver();
void bothStepper(int swpulse, int dir, int mvt);
void rotation_robot_absolue(double degree);
void rotation_robot_relative(double degree);
void Ligne_droite(double distance);