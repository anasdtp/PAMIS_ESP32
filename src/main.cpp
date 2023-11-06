#include <Arduino.h>
#include <stepper_robot.h>
#include <espBLE.h>
#include <ident_club_tech.h>
#include <timerAsserBas.h>

void BLEProcessRx();
void EnvoiPosition();
void setup(void) {
  Serial.begin(921600);Serial.println();

  //Init BLE device
  // slaveBTConnect("PAMIS_1");
  robotIDChoisi = NO_ID;
  myID = ID_PAMIS_1;

  init_Timer();

  initDriver();
  stepper1.m0 = 1;
  stepper2.m0 = 1;

  


}

void loop() {
 

  // refreshBLE();
  // TempsEchantionnage(TE_25MS);
}

void BLEProcessRx(){//Ce qu'il reste à faire
    static signed char FIFO_lecture=0,FIFO_occupation=0,FIFO_max_occupation=0;

    FIFO_occupation=FIFO_ecriture-FIFO_lecture;

    if(FIFO_occupation<0){FIFO_occupation=FIFO_occupation+SIZE_FIFO;}
    if(FIFO_max_occupation<FIFO_occupation){FIFO_max_occupation=FIFO_occupation;}
    if(!FIFO_occupation){return;}


    if(rxMsgBLE[FIFO_lecture].robotID == myID || rxMsgBLE[FIFO_lecture].robotID == NO_ID){//NO_ID = Tous le monde est concernés
      switch (rxMsgBLE[FIFO_lecture].ID)
        {
            case ID_DEPART_PAMIS:
            {
              //Lets go
            }
            break;

            case ID_PAMIS_LIGNE_DROITE:
            {
              int16_t distance = (rxMsgBLE[FIFO_lecture].dt[1] << 8) | rxMsgBLE[FIFO_lecture].dt[0];

              Ligne_droite(distance);
            }
            break;
            case ID_PAMIS_ROTATION://Absolue
            {
              int16_t angle = (rxMsgBLE[FIFO_lecture].dt[1] << 8) | rxMsgBLE[FIFO_lecture].dt[0]; //dixieme de degree

              rotation_robot_absolue(angle);
            }
            break;
            case ID_PAMIS_XYT:
            {
              int16_t x = (rxMsgBLE[FIFO_lecture].dt[1] << 8) | rxMsgBLE[FIFO_lecture].dt[0];
              int16_t y = (rxMsgBLE[FIFO_lecture].dt[3] << 8) | rxMsgBLE[FIFO_lecture].dt[2];
              int16_t theta = (rxMsgBLE[FIFO_lecture].dt[5] << 8) | rxMsgBLE[FIFO_lecture].dt[4];

              XYT(x, y, theta);
            }
            break;
            case ID_PAMIS_RECALAGE:
            {
              int16_t distance = (rxMsgBLE[FIFO_lecture].dt[1] << 8) | rxMsgBLE[FIFO_lecture].dt[0];
              uint8_t mode = rxMsgBLE[FIFO_lecture].dt[2];// si mode == 1, recalage sur x, sinon recalage sur y
              int16_t valRecalage = (rxMsgBLE[FIFO_lecture].dt[4] << 8) | rxMsgBLE[FIFO_lecture].dt[3];//Valeur pris au final

              
            }
            break;
        default:
            break;
        }
    }
    
    FIFO_lecture=(FIFO_lecture+1)%SIZE_FIFO;
}

void EnvoiPosition(){
  if(mscountOdo >= (500/TE_25MS))//toutes les 50ms
    {
        mscountOdo = 0;
        Serial.printf("x_robot : %lf ; y_robot : %lf ; theta_robot : %lf\n", x_robot, y_robot, theta_robot);

        // CANenvoiMsg3x2Bytes(ODOMETRIE_SMALL_POSITION, Odo_x, Odo_y, ((int16_t)Odo_theta) % 3600);
        //CANenvoiMsg3x2Bytes(ODOMETRIE_SMALL_POSITION, Odo_x, Odo_y, ((int16)Odo_theta) % 3600);
        //CANenvoiMsg3x2Bytes(DEBUG_ASSERV, QuadDec_D_GetCounter(), QuadDec_G_GetCounter(), consigne_pos);

    }
}
