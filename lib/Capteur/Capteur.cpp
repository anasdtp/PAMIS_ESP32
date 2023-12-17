#include "Capteur.h"
#include <timerAsserBas.h>
//Tous les capteurs sur les PAMIS : Les moustaches, le capteur ultrason

//Interrupteur, fdc, appellé moustaches, il y en a deux
const int PinMoustache1 = 2, PinMoustache2 = 15;

Ultrasonic ultrasonic(26);

volatile bool DANGER = false;

uint8_t etatMoustaches(){
    static bool both, one, MSTH1, MSTH2;
    MSTH1 = digitalRead(PinMoustache1); MSTH2 = digitalRead(PinMoustache2);
    // Serial.printf("MSTH1 : %d MSTH2 : %d\n", MSTH1, MSTH2);
    one = (MSTH1 || MSTH2 ? true : false);
    both= (MSTH1 && MSTH2 ? true : false);
    
    return (both ? 2 : one);
}


void Ultrason_task(void *parameter){
    static int detection = 0,n=0, distance = 0;
    uint32_t start_time = 0;
    
    while(1){
        start_time = timeMs();

        distance = ultrasonic.MeasureInMillimeters(); // two measurements should keep an interval
        Serial.print(distance);//0~4000mm
        Serial.println(" mm");

        if(distance>20 && distance<150){ detection+=1;}

        n+=1;
        if (n>10){ n=0; detection=0; DANGER = false;}
        if (detection>2){DANGER = true;}
        else{DANGER = false;}

        while((timeMs()-start_time)<150);
    }
}

bool UltrasonStatus(){
  return DANGER;
}

void init_Ultrason_task(){
    // Créez une tâche pour incrémenter uscount sur le deuxième cœur (CORE_1)
  xTaskCreatePinnedToCore(Ultrason_task, "Ultrason_task", 2048, NULL, 1, NULL, 1);

} 

void init_Capteurs(){
    pinMode(PinMoustache1, INPUT_PULLDOWN);//Pins qui doivent etre libre lors du boot
    pinMode(PinMoustache2, INPUT_PULLDOWN);

    init_Ultrason_task();
}
