/*Fichiers regroupant la totalité des IDs que ce soit pour le bus CAN ou pour le BLE, c'est le meme protocole*/

//IDs pour les PAMIS :
#define ID_DEPART_PAMIS 0x1F7
#define ID_PAMIS_XYT 0x020
#define ID_PAMIS_LIGNE_DROITE 0x024
#define ID_PAMIS_ROTATION 0x023

#define ID_PAMIS_ODO 0x028
/*
Le recalage sera fait grace aux deux moustaches/interrupteurs devant les PAMIS
Pour le y, le PAMIS sera poser le precisement possible à la main sur une ligne
Pour le x, Le Pamis s'avancera jusqu'a que les deux interupteurs touchent le mur
L'angle initiale sera donc de -90°
*/
#define ID_PAMIS_RECALAGE 0x104 

#define ESP32_RESTART 0x34

#define WAIT_FOR_JACK 0x105