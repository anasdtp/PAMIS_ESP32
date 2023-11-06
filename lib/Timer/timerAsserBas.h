#include <Arduino.h>

#define TE_25MS 25          //Temps d'echantionnage -> 25 x 100US = 2.5ms

extern volatile uint32_t mscount, mscount2, mscountOdo;
//----------------------------------------------------------------------Timer
void init_Timer();
void onTime();//prototype de la fonction s'exécutent à chaque interruptions
void TempsEchantionnage(int TIME);
uint32_t timeMs();