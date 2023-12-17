// #include <espBLE.h>



// BLEMessage rxMsgBLE[SIZE_FIFO], InstantRxMsgBLE;
// unsigned char FIFO_ecriture = 0;

// ROBOTIDBLE robotIDChoisi = NO_ID;
// ROBOTIDBLE myID = ID_PAMIS_1;

// bool newCAN = false;



// /* UUID's of the service, characteristic that we want to read*/
// // BLE Service
// static BLEUUID serviceUUID("69a68877-c627-42d1-a087-6a1307f83616");
// static BLEUUID      rxUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
// static BLEUUID      txUUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
// //-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_SLAVE

// //Address of the peripheral device. Address will be found during scanning...
//  BLEAddress *pServerAddress;

//  boolean doConnect = false;
//  boolean connected = false;
//  boolean doScan = false;
// static BLERemoteCharacteristic* prxRemoteCharacteristic;//Characteristicd that we want to read
// static BLERemoteCharacteristic* ptxRemoteCharacteristic;
// static BLEAdvertisedDevice* myDevice;
// //Activate notify
// const uint8_t notificationOn[] = {0x1, 0x0};
// const uint8_t notificationOff[] = {0x0, 0x0};

// // char* contenuBT;

// void readDATA(std::string contenuBT){//Fonction à testée le bon positionnement des variables

//   InstantRxMsgBLE.robotID = contenuBT[0];
//   InstantRxMsgBLE.ID = contenuBT[1] + (contenuBT[2]<<8) + (contenuBT[3]<<16) + (contenuBT[4]<<24);
//   InstantRxMsgBLE.ln = contenuBT[5];
//   for(int i=0;i<InstantRxMsgBLE.ln;i++)
//   {
//     InstantRxMsgBLE.dt[i]=contenuBT[i+6];
//   }
    
//   rxMsgBLE[FIFO_ecriture] = InstantRxMsgBLE;
//   FIFO_ecriture=(FIFO_ecriture+1)%SIZE_FIFO;
// }

// void refreshBLE(){
//   // If the flag "doConnect" is true then we have scanned for and found the desired
//   // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
//   // connected we set the connected flag to be true.
//   if (doConnect == true) {
//     if (connectToServer(*pServerAddress)) {
//       Serial.println("We are now connected to the BLE Server.");
//       //Activate the Notify property of each Characteristic
//       ptxRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      
//     } else {
//       Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
//     }
//     doConnect = false;
//   }
// }

// void sendNewCAN(){
//     if (newCAN){
//     newCAN = false;
//     printBLEStruct(InstantRxMsgBLE);
//   }
// }


// void convCANmsgIntoBLEmsg(CANMessage &CANmsg, BLEMessage &BLEmsg, uint8_t robotID){
//     BLEmsg.robotID = robotID;
//     BLEmsg.ID = CANmsg.ID;
//     BLEmsg.ln = CANmsg.ln;
//     for(int i = 0; i<CANmsg.ln; i++){BLEmsg.dt[i] = CANmsg.dt[i];}
// }

// void convBLEmsgIntoCANmsg(BLEMessage &BLEmsg, CANMessage &CANmsg){
//     CANmsg.extented = false;
//     CANmsg.RTR = false;
//     CANmsg.ID = CANmsg.ID;
//     CANmsg.ln = CANmsg.ln;
//     for(int i = 0; i<BLEmsg.ln; i++){CANmsg.dt[i] = BLEmsg.dt[i];}
// }

// void printBLEStruct(BLEMessage msg){
//     printf("  RobotID    = %d\n", msg.robotID);
//     printf("  ID      = 0x%.3x\n", msg.ID);
//     printf("  Length  = %d\n", msg.ln);
//     printf("  Data    = 0x");            
//     for(int i = 0 ; i < msg.ln ; i++){printf(" %.2X", msg.dt[i]);}
//     printf("\n");
//  }


// void slaveBTConnect(std::string name){
//   // Create the BLE Device
//   BLEDevice::init(name);
//   BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
//   // Retrieve a Scanner and set the callback we want to use to be informed when we
//   // have detected a new device.  Specify that we want active scanning and start the
//   // scan to run for 30 seconds.
//   BLEScan* pBLEScan = BLEDevice::getScan();
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setActiveScan(true);
//   pBLEScan->start(5, false);
// }


// //Connect to the BLE Server that has the name, Service, and Characteristics
// bool connectToServer(BLEAddress pAddress) {
//    BLEClient* pClient = BLEDevice::createClient();
 
//   // Connect to the remove BLE Server.
//   pClient->connect(pAddress);
//   Serial.println(" - Connected to server");
 
//   // Obtain a reference to the service we are after in the remote BLE server.
//   BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
//   if (pRemoteService == nullptr) {
//     Serial.print("Failed to find our service UUID: ");
//     Serial.println(serviceUUID.toString().c_str());
//     return (false);
//   }
 
//   // Obtain a reference to the characteristics in the service of the remote BLE server.
//   prxRemoteCharacteristic = pRemoteService->getCharacteristic(rxUUID);
//   ptxRemoteCharacteristic = pRemoteService->getCharacteristic(txUUID);

//   if (prxRemoteCharacteristic == nullptr || ptxRemoteCharacteristic == nullptr) {
//     Serial.print("Failed to find our characteristic UUID");
//     return false;
//   }
//   Serial.println(" - Found our characteristics");
 
//   //Assign callback functions for the Characteristics
//   ptxRemoteCharacteristic->registerForNotify(CANNotifyCallback);
//   connected = true;
//   return true;
// }

// void sendStructBLE_Slave(BLEMessage &msg){
//   // Send message to master via bleutooth
//     if (connected)
//     {
//       prxRemoteCharacteristic->writeValue((uint8_t *)&msg, sizeof(msg));
//       Serial.println("Sending via BT...");
//     }
//     else{
//       Serial.println("The device is not connected");
      
//     }
// }

// void sendCANStructBLE_Slave(CANMessage &msg){
//     // Send message to master via bleutooth
//     if (connected)
//     {
//       BLEMessage txMsg;
//       convCANmsgIntoBLEmsg(msg, txMsg, robotIDChoisi);
//       prxRemoteCharacteristic->writeValue((uint8_t *)&txMsg, sizeof(txMsg));
//       Serial.println("Sending via BT...");
//     }
//     else{
//       Serial.println("The device is not connected");
      
//     }
// }