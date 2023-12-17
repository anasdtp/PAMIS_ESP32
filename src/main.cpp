#include <Arduino.h>
#include <espQtCAN.h>
#include <stepper_robot.h>
// #include <espBLE.h>
#include <ident_club_tech.h>
#include <timerAsserBas.h>
#include <Capteur.h>
#include <Strategie.h>





void BLEProcessRx();
void EnvoiPosition();
volatile bool JACK = false;
void setup(void) {
  Serial.begin(921600);Serial.println();

  //Init BLE device
  // slaveBTConnect("PAMIS_1");
  // robotIDChoisi = NO_ID;
  // myID = ID_PAMIS_1;
  BTQTsetup();




  
  // init_Capteurs();

  initDriver();
  stepper1.m0 = 0;
  stepper2.m0 = 0;

  if(COULEUR_STRAT == JAUNE){
    x_robot = 0; y_robot = 3000 - 1055; theta_robot = 1800;
  }else{  
    x_robot = 0; y_robot = 1055; theta_robot = 1800; //Position initiale posée à la main à vue d'oeil
  }
  

  EnvoiPosition();
  //test 1:
  // recalage(1000, 1, MOITIEE_ROBOT);
  // EnvoiPosition();
  // Ligne_droite(-150,1./2.);
  // EnvoiPosition();
  // XYT(150, 1520, 1800); //Preparation garage debut match
  // EnvoiPosition();

  // delay(1000);//A suppr
  // recalage(1000, 1, MOITIEE_ROBOT);//On se plaque contre le mur
  // EnvoiPosition();

  // rotation_robot_absolue(900);
  // delay(500);
  // rotation_robot_absolue(900);
  // delay(500);
  // rotation_robot_absolue(900);
  // delay(500);
  // rotation_robot_absolue(900);
  // delay(500);

  //test 3:
  // Ligne_droite(100,1./4.);

  init_Timer();
  Serial.println("End setup");

  // JACK = true;//Pour le moment on part direct pour les tests
}

void loop() {
  BTQtLoop();

  BLEProcessRx();
  
  StrategieLoop();

  EnvoiPosition();


  // BLEProcessRx();
  // refreshBLE();
  TempsEchantionnage(TE_5MS);
}


void EnvoiPosition(){
  if(mscountOdo >= 50)//toutes les 50ms
    {
        mscountOdo = 0;
        // Serial.printf("x_robot : %lf ; y_robot : %lf ; theta_robot : %lf\n", x_robot, y_robot, theta_robot);
        CANMessage txMsg;
        uint8_t data[6] = {(((uint16_t)(x_robot))&0xFF), ((((uint16_t)(x_robot))>>8)&0xFF),
                         (((uint16_t)(y_robot))&0xFF), ((((uint16_t)(y_robot))>>8)&0xFF),
                         (((uint16_t)(theta_robot))&0xFF), ((((uint16_t)(theta_robot))>>8)&0xFF),
         };
        remplirStruct(txMsg, ID_PAMIS_ODO, 6, data[0],data[1],data[2], data[3],data[4],data[5] ,0,0);
        envoieBT(txMsg);
        // CANenvoiMsg3x2Bytes(ODOMETRIE_SMALL_POSITION, Odo_x, Odo_y, ((int16_t)Odo_theta) % 3600);
        //CANenvoiMsg3x2Bytes(ODOMETRIE_SMALL_POSITION, Odo_x, Odo_y, ((int16)Odo_theta) % 3600);
        //CANenvoiMsg3x2Bytes(DEBUG_ASSERV, QuadDec_D_GetCounter(), QuadDec_G_GetCounter(), consigne_pos);

    }
}

void BLEProcessRx(){//Ce qu'il reste à faire
    static signed char FIFO_lecture=0,FIFO_occupation=0,FIFO_max_occupation=0;

    FIFO_occupation=FIFO_ecriture-FIFO_lecture;

    if(FIFO_occupation<0){FIFO_occupation=FIFO_occupation+SIZE_FIFO;}
    if(FIFO_max_occupation<FIFO_occupation){FIFO_max_occupation=FIFO_occupation;}
    if(!FIFO_occupation){return;}


    //if(rxMsgBLE[FIFO_lecture].robotID == myID || rxMsgBLE[FIFO_lecture].robotID == NO_ID){//NO_ID = Tous le monde est concernés
      switch (rxMsgBLE[FIFO_lecture].id)
        {
            case ESP32_RESTART:
                Serial.println("ESP32_RESTART");
                esp_restart();
                
              break;
            case ID_DEPART_PAMIS:
            {
              //Lets go
              JACK = true;
            }
            break;

            case ID_PAMIS_LIGNE_DROITE:
            {
              int16_t distance = (rxMsgBLE[FIFO_lecture].data[1] << 8) | rxMsgBLE[FIFO_lecture].data[0];
              uint8_t mode = rxMsgBLE[FIFO_lecture].data[2];// si mode == 1, recalage sur x, sinon recalage sur y
              int16_t valRecalage = (rxMsgBLE[FIFO_lecture].data[4] << 8) | rxMsgBLE[FIFO_lecture].data[3];//Valeur pris au final
                if(mode){
                  recalage(distance, mode, valRecalage);
                }else{
                  Ligne_droite(distance, 0.6);
                }
              
            }
            break;
            case ID_PAMIS_ROTATION://Absolue
            {
              int16_t angle = (rxMsgBLE[FIFO_lecture].data[1] << 8) | rxMsgBLE[FIFO_lecture].data[0]; //dixieme de degree

              rotation_robot_absolue(angle);
            }
            break;
            case ID_PAMIS_XYT:
            {
              int16_t x = (rxMsgBLE[FIFO_lecture].data[1] << 8) | rxMsgBLE[FIFO_lecture].data[0];
              int16_t y = (rxMsgBLE[FIFO_lecture].data[3] << 8) | rxMsgBLE[FIFO_lecture].data[2];
              int16_t theta = (rxMsgBLE[FIFO_lecture].data[5] << 8) | rxMsgBLE[FIFO_lecture].data[4];

              XYT(x, y, theta);
            }
            break;
            case ID_PAMIS_RECALAGE:
            {
              int16_t distance = (rxMsgBLE[FIFO_lecture].data[1] << 8) | rxMsgBLE[FIFO_lecture].data[0];
              uint8_t mode = rxMsgBLE[FIFO_lecture].data[2];// si mode == 1, recalage sur x, sinon recalage sur y
              int16_t valRecalage = (rxMsgBLE[FIFO_lecture].data[4] << 8) | rxMsgBLE[FIFO_lecture].data[3];//Valeur pris au final

              recalage(distance, mode, valRecalage);
            }
            break;
            case ID_PAMIS_ODO: //Changement de la position du Pamis par le Bt
            {
              int16_t x = (rxMsgBLE[FIFO_lecture].data[1] << 8) | rxMsgBLE[FIFO_lecture].data[0];
              int16_t y = (rxMsgBLE[FIFO_lecture].data[3] << 8) | rxMsgBLE[FIFO_lecture].data[2];
              int16_t theta = (rxMsgBLE[FIFO_lecture].data[5] << 8) | rxMsgBLE[FIFO_lecture].data[4];

              x_robot = x; y_robot = y; theta_robot = theta;
            }break;
        default:
            break;
        }

        FIFO_lecture=(FIFO_lecture+1)%SIZE_FIFO;
}
    
    


