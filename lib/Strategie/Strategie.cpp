#include "Strategie.h"
#include <Capteur.h>
#include <timerAsserBas.h>
#include <ident_club_tech.h>
#include <stepper_robot.h>
#include <espBLE.h>
//Jack = depart Match

Etape strategie_hackaton[SIZE_ACTION];//Faire la fonction qui interprete tab_action_brut. void initStrategie();
bool next_action = true;
//Cpateur ultrason : -----------------------------------------------------------------------------
char type_Evitement = ETAT_GAME_PAS_DANGER; //Mouvement ETAT_GAME_MVT_DANGER evitement sinon non 
unsigned short target_x,target_y,target_theta;//Pour aller là où on devait aller apres que le robot soit passé

int16_t tab_action_brut[SIZE_ACTION][5]={
    //Phase d'initialisation (pendant les 3 min) :
    {'O', 1000,1,MOITIEE_ROBOT,0},//On avance doucement jusqu'a se plaquer contre le mur sur l'axe des x
    {'L', -150,0,0,0},//On recule
    {'X', 150,1480,1800,0},//Preparation garage debut match
    {'O', 1000,1,MOITIEE_ROBOT,0},//On se plaque contre le mur

    {'J', 0,0,0,0},//Attente du jack
    //Debut du match :
    {'X', 250,450,-900,0},
    {'O', 1000,-1,MOITIEE_ROBOT,0},//On avance doucement jusqu'a touché soit le mur soit une plante

    {'0', 0,0,0,0}//Fin de match
};
//Fin Construction liste strategie : --------------------------------------------------


void initStrategie(){
    
}

void StrategieLoop(){
    static bool WaitForJack = false;
    static int ETAPE_lecture=0,ETAPE_occupation=0,ETAPE_max_occupation=0, etat_evitement = 3;

    if(WaitForJack){//On attend alors le debut du match
        if(JACK){
            Serial.println("Game start");
            JACK = false;
            WaitForJack = false;
            next_action = true;
        }
        else{
            return;
        }
    }

    ETAPE_occupation=SIZE_ACTION-ETAPE_lecture;
    if(ETAPE_occupation<0){ETAPE_occupation=ETAPE_occupation+SIZE_ACTION;}
    if(ETAPE_max_occupation<ETAPE_occupation){ETAPE_max_occupation=ETAPE_occupation;}//Ajouter des conditions : attendre que l'action est été faite

    

    
/*
    switch (etat_evitement)
    {
    case 0:{
        if(UltrasonStatus()){//Alors s'arreter)   //&& (type_Evitement != ETAT_GAME_PAS_DANGER) //Enlever car bug des fois
            Serial.println("Capteur_ultrason detection");
            // stop_receive = 1; //Arret brutal
            etat_evitement = 1;
            mscount_capteur = 0;
            return;
        }
    }
        break;
    case 1:{
        if(mscount_capteur>1000){//Attendre une seconde
            etat_evitement = 2;
        }
        return;
    }
        break;
    case 2:{
        if(UltrasonStatus()){//Alors s'arreter
            // stop_receive = 1; //Arret brutal
            etat_evitement = 1;
            mscount_capteur = 0;
            return;
        }else{//On reprend
                etat_evitement = 0;
            // `#START MESSAGE_X_Y_Theta_RECEIVED` 
                    // stop_receive = 0;

                    XYT(target_x, target_y, target_theta); //On va là où on devait aller
                    return;    
        }
    }
        break;
    case 3 :
    //Ne rien faire, Lidar desactivé pendant la phase d'initialisation et s'active dés quon tire le Jack
    break;
    default:
        break;
    }//*/

    
    if(!next_action){return;}
    next_action = false;
    Serial.print(ETAPE_lecture);  Serial.print(" - Action : ");  Serial.print(strategie_hackaton[ETAPE_lecture].ACTION); 
   //if(canAvailable || BtAvailable){
    // if(canAvailable){canReadExtRtr();}//On le me ici pour ne pas surcharger l'interruption CAN.onRecveive
    // canAvailable = false; BtAvailable = false;
    ////Serial.println("CAN received");
    // if(!ETAPE_occupation){return;}
    switch (strategie_hackaton[ETAPE_lecture].ACTION)
    {
            case WAIT_FOR_JACK ://Debut du Match dés qu'on tire le Jack
                WaitForJack = true;
                etat_evitement = 0;//On active le lidar durant le match
            break;

            case ID_PAMIS_LIGNE_DROITE:
            {
              Ligne_droite(strategie_hackaton[ETAPE_lecture].distance,1./2.);
              next_action = true;
            }
            break;
            
            case ID_PAMIS_ROTATION://Absolue
            {
              rotation_robot_absolue(strategie_hackaton[ETAPE_lecture].angle);
              next_action = true;
            }
            break;
            case ID_PAMIS_XYT:
            {
              double x = strategie_hackaton[ETAPE_lecture].x;
              double y = strategie_hackaton[ETAPE_lecture].y;
              double theta = strategie_hackaton[ETAPE_lecture].theta;

              XYT(x, y, theta);
              next_action = true;
            }
            break;
            case ID_PAMIS_RECALAGE:
            {
              int16_t distance = strategie_hackaton[ETAPE_lecture].distance;
              uint8_t mode = strategie_hackaton[ETAPE_lecture].mode;// si mode == 1, recalage sur x, sinon recalage sur y
              int16_t valRecalage = strategie_hackaton[ETAPE_lecture].val_recalage;//Valeur pris au final

              recalage(distance, mode, valRecalage);
              next_action = true;
            }
              
            // case ODOMETRIE_SMALL_POSITION:
            // {
            //     Odo_x = (strategie_hackaton[ETAPE_lecture].dt[1] << 8) | strategie_hackaton[ETAPE_lecture].dt[0];
            //     Odo_y = (strategie_hackaton[ETAPE_lecture].dt[3] << 8) | strategie_hackaton[ETAPE_lecture].dt[2];
            //     Odo_theta = (strategie_hackaton[ETAPE_lecture].dt[5] << 8) | strategie_hackaton[ETAPE_lecture].dt[4];
            // }
            // break;
           
            
            default :
            break;
    }
    ETAPE_lecture ++;
    // ETAPE_lecture %= SIZE_ACTION;
    // ETAPE_lecture=(ETAPE_lecture+1)%SIZE_ACTION;
    Serial.println(ETAPE_lecture);
}   
