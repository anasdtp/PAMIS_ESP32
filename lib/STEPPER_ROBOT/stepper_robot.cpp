#include "stepper_robot.h"


double x_robot = 0, y_robot = 0, theta_robot = 0;//degree en dizieme de degree, distance en mm

STEPPER stepper1(17, 16, 5);
STEPPER stepper2(19, 18, 23);

void initDriver(){
  stepper1.MO(HIGH); stepper2.MO(HIGH);
  delay (500);
}

void bothStepper(int swpulse, int dir, int mvt){
  if (stepper1.m0==0) {stepper1.MO(LOW); stepper2.MO(LOW);} else {stepper2.MO(HIGH); stepper2.MO(HIGH);}

  if(mvt == MOUVEMENT_ROTATION){
    if (dir==0) {stepper1.DIR(HIGH); stepper2.DIR(LOW);} else {stepper1.DIR(LOW); stepper2.DIR(HIGH);}
  }
  else if(mvt == MOUVEMENT_LIGNE_DROITE){
    if (dir==0) {stepper1.DIR(LOW); stepper2.DIR(LOW);} else {stepper1.DIR(HIGH); stepper2.DIR(HIGH);}
  }
    
  // STEP GENERATOR
  for ( int i = 0; i < swpulse; i++) {
      stepper1.STEP(HIGH); stepper2.STEP(HIGH);
      delay(1);
      stepper1.STEP(LOW); stepper2.STEP(LOW); 
      delay(1);
  }
  return;
}

void rotation_robot_absolue(double degree){//en dizieme de degree 360° -> 3600 dizieme de degree
  //distance_totale = degree * PI * Rayon_robot/ 180;
  //distance_parcourue_par_une_roue = DEGREEPARSTEP * R_roue;
  //Step = distance_totale/distance_parcourue_par_une_roue;
  float degreeParStep = DEGREPARSTEP; int dir = 0;
  //      m0 == 0 && m1 == 0 && m2 == 0
  if     (stepper1.m0 == 1){degreeParStep /= 2.0;}//On verifie que pour un stepper car normalement c'est le meme pour les deux
  if (degree > 1800.0) {
        degree -= 3600.0; // Effectuer une rotation dans le sens opposé
  }

  if(degree<0){dir = 1; degree *= -1;}

  double Step = degree * (PI*Rayon_Robot/(degreeParStep*Rayon_roue)) / 180;

  bothStepper((int)Step, dir, MOUVEMENT_ROTATION);
}

// Fonction de rotation relative
void rotation_robot_relative(double degree) {//en dizieme de degree
  if(degree == theta_robot){return;}

    double degreeAparcourir = degree - theta_robot;
    // Appelez la fonction de rotation absolue avec le nouvel angle
    rotation_robot_absolue(degreeAparcourir);
    theta_robot = degree;
}

void Ligne_droite(double distance) {
    // Calculate the number of steps required to move the given distance
    // Assuming DEGREEPARSTEP is the number of degrees moved per step
    double degreeParStep = DEGREPARSTEP;
    if     (stepper1.m0 == 1){degreeParStep /= 2.0;}//On verifie que pour un stepper car normalement c'est le meme pour les deux
    double steps = distance * (PI * Rayon_Robot / (degreeParStep * Rayon_roue)) / 180;
    
    // Determine the direction (forward or backward movement)
    int dir = (distance >= 0) ? 0 : 1;

    // Call the function to move both stepper motors
    bothStepper((int)abs(steps), dir, MOUVEMENT_LIGNE_DROITE);
}
