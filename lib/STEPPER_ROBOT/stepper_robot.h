#include <Arduino.h>
#include <Stepper.h>

#define Rayon_Robot 150. //mm
#define Rayon_roue 25. //mm au hasard pour le moment

extern STEPPER stepper1, stepper2;
extern volatile double x_robot, y_robot, theta_robot;

void initDriver();
void bothStepper(int swpulse, int dir, int mvt);
void rotation_robot_absolue(double degree);
void rotation_robot_relative(double degree);
void Ligne_droite(double distance);
void XYT(double px, double py, double ptheta);