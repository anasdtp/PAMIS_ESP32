#include <Arduino.h>
#include <Ultrasonic.h>

//Capteur ultrason 
#define ETAT_GAME_MVT_DANGER 0x01
#define ETAT_GAME_PAS_DANGER 0x02

extern volatile bool DANGER;

void init_Capteurs();
uint8_t etatMoustaches();
bool UltrasonStatus();