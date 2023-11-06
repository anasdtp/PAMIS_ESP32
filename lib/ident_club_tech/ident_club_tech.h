/*Fichiers regroupant la totalité des IDs que ce soit pour le bus CAN ou pour le BLE, c'est le meme protocole*/

//IDs pour les PAMIS :
#define ID_DEPART_PAMIS 0x100
#define ID_PAMIS_XYT 0x101
#define ID_PAMIS_LIGNE_DROITE 0x102
#define ID_PAMIS_ROTATION 0x103
/*
Le recalage sera fait grace aux deux moustaches/interrupteurs devant les PAMIS
Pour le y, le PAMIS sera poser le precisement possible à la main sur une ligne
Pour le x, Le Pamis s'avancera jusqu'a que les deux interupteurs touchent le mur
L'angle initiale sera donc de -90°
*/
#define ID_PAMIS_RECALAGE 0x104 