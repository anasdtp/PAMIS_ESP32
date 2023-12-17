#include <Arduino.h>
#include <Capteur.h>

#define SIZE_ACTION 4 + 1 + 2       +1 //Depart + Jack + jeu   +1 fin de match

//Phase d'initialisation, de recalage : 
#define EN_BAS_A_DROITE 0 //L'origine donc x = x y = y theta = theta //Zone Bleu
#define EN_BAS_A_GAUCHE 1 //donc y = 3000 - y et theta = -theta //Zone JAUNE
//On ne peut pas partir d'en HAUT (pour le moment...); EN BAS c'est coté mat

#define RECALAGE_DEPART EN_BAS_A_GAUCHE //Donc en zone JAUNE


//Phase de jeu : 
//Pour la phase du match choisir une couleur, couleur de base BLEU, mettre les coordonnées comme si on etait en BLEU meme si on est JAUNE
#define BLEU 0
#define JAUNE 1

#define COULEUR_STRAT JAUNE //Bleu Ou JAUNE, de base il faut faire la strat comme si on etait en bleu et sa s'inverse tout seul
// IMPORTANT!  FAIRE LA STRAT de base EN BLEU

typedef struct Etape
{
  /* data */
  int ACTION;
  int variable;
  double x,y,theta;// XYT

  double angle; //rotation

  double distance; uint8_t mode; int16_t val_recalage; //Ligne droite ou recalage, Si mode<0 recalage sur y sinon sur x. Si mode = 0 alors ligne droite

}Etape;

extern Etape strategie_hackaton[SIZE_ACTION];
extern bool next_action;
//Cpateur ultrason : -----------------------------------------------------------------------------
extern char type_Evitement; //Mouvement ETAT_GAME_MVT_DANGER evitement sinon non 
extern unsigned short target_x,target_y,target_theta;//Pour aller là où on devait aller apres que le robot soit passé

extern volatile bool JACK;

//Construction liste strategie : ------------------------------------------------------

/* [TYPE] [ARG1] [ARG2] [ARG3] [ARG4]*/

/*  J : Jack ;;; Wait for jack
    X : XYT [x];[y];[t];
    L : Ligne droite [d];;;
    R : Rotation [a];;;
    O : Recalage [d];[x ou y?]si x mettre 1, si y -1;[Val recalage] valeur que prendra l'encodeur à la fin;

    A : Action [Type] [ARG1] [ARG2]
*/

extern int16_t tab_action_brut[SIZE_ACTION][5];


void initStrategie();
void StrategieLoop();