#include <Arduino.h>
#include <Stepper.h>

#define Rayon_robot 160./2. //mm
#define Rayon_roue 58./2. //mm 

#define MOITIEE_ROBOT 50 //mm

extern STEPPER stepper1, stepper2;
extern volatile double x_robot, y_robot, theta_robot;

void initDriver();
void bothStepper(int swpulse, int dir, int mvt, int delaiPas);
void rotation_robot_absolue(double degree);
void rotation_robot_relative(double degree);
void Ligne_droite(double distance, double vit_m_s);
void XYT(double px, double py, double ptheta);
void recalage(int16_t distance, short mode, int16_t valRecalage);