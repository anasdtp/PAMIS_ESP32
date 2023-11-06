#include "stepper_robot.h"


volatile double x_robot = 0, y_robot = 0, theta_robot = 0;//degree en dizieme de degree, distance en mm
        /*(STEPpin, DIRpin, MOpin)*/
STEPPER stepper1(17, 16, 5);
STEPPER stepper2(19, 18, 23);

void initDriver(){
  stepper1.MO(HIGH); stepper2.MO(HIGH);
  delay (100);
}

void bothStepper(int swpulse, int dir, int mvt){
  if (stepper1.m0==0) {stepper1.MO(LOW); stepper2.MO(LOW);} else {stepper2.MO(HIGH); stepper2.MO(HIGH);}

  if(mvt == MOUVEMENT_LIGNE_DROITE){
    if (dir==0) {stepper1.DIR(HIGH); stepper2.DIR(LOW );} 
    else        {stepper1.DIR(LOW ); stepper2.DIR(HIGH);}
  }
  else if(mvt == MOUVEMENT_ROTATION){
    if (dir==0) {stepper1.DIR(LOW ); stepper2.DIR(LOW );} 
    else        {stepper1.DIR(HIGH); stepper2.DIR(HIGH);}
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
  //distance_totale = degree * PI/ 180 * Rayon_robot;
  //Rotation d'une roue = distance_totale/Rayon_roue * 360;
  //Step = degreeParStep * Rotation d'une roue ;
  double degreeParStep = DEGREPARSTEP; int dir = 0;
  //      m0 == 0 && m1 == 0 && m2 == 0
  if     (stepper1.m0 == 1){degreeParStep /= 2.0;}//On verifie que pour un stepper car normalement c'est le meme pour les deux
  if (degree > 1800.0) {
        degree -= 3600.0; // Effectuer une rotation dans le sens opposé
  }else if(degree < -1800.0){
    degree += 3600.0;
  }
  
  if(degree<0){dir = 1;}
  double distance_totale = abs(degree) * PI/ 1800 * Rayon_Robot;
  double Step = (distance_totale*180.0/(Rayon_roue*PI))/degreeParStep;

  bothStepper((int)Step, dir, MOUVEMENT_ROTATION);

  theta_robot += degree;
  if (theta_robot > 1800.0) {
        theta_robot -= 3600.0; 
  }else if(theta_robot < -1800.0){
    theta_robot += 3600.0;
  }
}

// Fonction de rotation relative
void rotation_robot_relative(double degree) {//en dizieme de degree
  if(degree == theta_robot){return;}

    double degreeAparcourir = degree - theta_robot;
    // Appelez la fonction de rotation absolue avec le nouvel angle
    rotation_robot_absolue(degreeAparcourir);
    theta_robot = degree;
}

void Ligne_droite(double distance) {//En mm
    // Calculate the number of steps required to move the given distance
    // Assuming DEGREEPARSTEP is the number of degrees moved per step
    double degreeParStep = DEGREPARSTEP;
    if     (stepper1.m0 == 1){degreeParStep /= 2.0;}//On verifie que pour un stepper car normalement c'est le meme pour les deux
    double steps =  (distance*180.0/(Rayon_roue*PI))/degreeParStep;
    
    // Determine the direction (forward or backward movement)
    int dir = (distance >= 0) ? 0 : 1;

    // Call the function to move both stepper motors
    bothStepper((int)abs(steps), dir, MOUVEMENT_LIGNE_DROITE);
    x_robot = x_robot + distance * cos(theta_robot * PI / 1800.0);
    y_robot = y_robot + distance * sin(theta_robot * PI / 1800.0);
}

void XYT(double px, double py, double ptheta){
  static double dist = 0, ang1 = 0, ang2 = 0;


  // Son hypothénuse correspond à la distance à parcourir
  dist = sqrt((px - x_robot)*(px - x_robot)+(py - y_robot)*(py - y_robot));

  // la 1ere rotation correspond à l'angle du triangle, moins l'angle de la position de départ
  // C'est-à-dire la tangente du côté opposé sur l'angle adjacent
  // La fonction atan2 fait le calcul de la tangente en radians, entre Pi et -Pi
  // On rajoute des coefficients pour passer en degrés
  // On ajoute 7200 dixièmes de degrés pour être sûrs que le résultat soit positif
  // ang1 = (short)((atan2((double)(py - y_robot), (double)(px - x_robot)) * 1800 / PI) - theta_robot + 7200) % 3600;

  if((((py-y_robot)!=0)&&((px-x_robot)!=0))||((px-x_robot)!=0)){
    ang1 = (long)((atan2((double)(py - y_robot), (double)(px - x_robot)) * 1800 / M_PI) - theta_robot + 7200) % 3600;
  }   
  // On passe le résultat entre -1800 et 1800
  if(ang1 > 1800) {ang1 = (ang1 - 3600);}

  // La 2è rotation correspond à l'angle de destination, moins l'angle à la fin de la ligne droite,
  // donc le même qu'à la fin de la 1ère rotation, donc l'angle de départ plus la première rotation
  // On ajoute 3600 pour être sûr d'avoir un résultat positif
  ang2 = (long)(ptheta - ang1 - theta_robot + 3600) % 3600;
                    
  // On passe le résultat entre -1800 et 1800
  if(ang2 > 1800) ang2 = (ang2 - 3600);
  // Serial.printf("X_Y_Theta : Dist : %f, ang1 : %hd, ang2 : %hd       ;       ", dist, ang1, ang2); 

  //ROTATION_X_Y_THETA_1 :
  rotation_robot_absolue(ang1);
  Serial.printf("rotation_robot_absolue(ang1 : %lf);       ", ang1); 
  delay(200);
  // LIGNE_DROITE_X_Y_THETA :
  Ligne_droite(dist);
  Serial.printf("Ligne_droite(dist : %lf);       ", dist); 
  delay(200);
  // ROTATION_X_Y_THETA_2 : 
  rotation_robot_absolue(ang2);
  Serial.printf("rotation_robot_absolue(ang2 : %lf);\n", ang2); 

}

void recalage(int16_t distance, short mode, int16_t valRecalage)
{
  // si mode == 1, recalage sur x, sinon recalage sur y
  int16_t distance_parcourue = 0;
  double step = (distance < 0 ? -10.0 : 10.0); 
  //Je le mets au cas où mais distance doit toujours etre positif parce que les interrupteurs sont devant. Apres on pourrait changer d'avis donc voila
  while (distance_parcourue < abs(distance))
  {
    Ligne_droite(step); // 1 cm
    distance_parcourue += 10;
    if (1)//Capteur interrupteur, Si 1 alors on a bien touché le rebord, recalage fait
    {
      distance_parcourue = abs(distance);//On sort de la boucle
      if(mode == 1){//sur x
        x_robot = valRecalage;
                        // On met l'angle à jour en fonction de la position sur le terrain
                        // Si on est dans la partie haute ( > la moitié), on est dans un sens,
                        // Si on est dans la partie basse, on met à jour dans l'autre sens
                        // On prend aussi en compte le sens dans lequel on a fait la ligne droite
        if (valRecalage > 1000){
          if (distance >= 0){theta_robot = 0;}
          else{theta_robot = 1800;}
        }
        else{
          if (distance >= 0){theta_robot = 1800;}
          else{theta_robot = 0;}
        }
      }
      else{//sur y
        y_robot = valRecalage;

        if (valRecalage > 1500){
          if (distance >= 0){theta_robot = 900;}
          else{theta_robot = -900;}
        }
        else{
          if (distance >= 0){theta_robot = -900;}
          else{theta_robot = 900;}
        }
      }
    }else{
      delay(5);
    }
  }
}