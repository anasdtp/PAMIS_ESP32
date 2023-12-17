// #include <Arduino.h>
// // #include <BLEDevice.h>
// // #include <BLEServer.h>
// // #include <BLEUtils.h>
// // #include <BLE2902.h>


// typedef struct CANMessage {
//   bool extented = false;
//   bool RTR = false;
//   unsigned int ID;
//   char ln;
//   unsigned char dt[8];
// } CANMessage;
// extern bool newCAN;


// // See the following for generating UUIDs:
// // https://www.uuidgenerator.net/
// #define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
// #define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
// #define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// #define bleServerName "ESP32E"


// extern BLEServer *pServer;
// extern bool deviceConnected;

// #define SIZE_FIFO 32
// typedef struct BLEMessage {
//   uint8_t robotID;
//   unsigned int ID;
//   uint8_t ln;
//   uint8_t dt[8];
// } BLEMessage;
// extern BLEMessage rxMsgBLE[SIZE_FIFO];
// extern unsigned char FIFO_ecriture;

// typedef enum ROBOTIDBLE{
//     NO_ID,
//     ID_ROBOT,
//     ID_PAMIS_1,
//     ID_PAMIS_2,
//     ID_PAMIS_3,
// }ROBOTIDBLE;
// extern ROBOTIDBLE robotIDChoisi;
// extern ROBOTIDBLE myID;
// //both
// void readDATA(std::string contenuBT);
// void refreshBLE();
// void sendNewCAN();
// void convCANmsgIntoBLEmsg(CANMessage &CANmsg, BLEMessage &BLEmsg, uint8_t robotID);
// void convBLEmsgIntoCANmsg(BLEMessage &BLEmsg, CANMessage &CANmsg);
// void printBLEStruct(BLEMessage msg);

// //slave
// void slaveBTConnect(std::string name);
// bool connectToServer(BLEAddress pAddress);
// void sendStructBLE_Slave(BLEMessage &msg);
// void sendCANStructBLE_Slave(CANMessage &msg);
// //----------------------------------------------------------------------callback fonctions


// extern BLEAddress *pServerAddress;


// extern boolean doConnect;
// extern boolean connected;
// extern boolean doScan;

// static void notifyCallback(
//   BLERemoteCharacteristic* pBLERemoteCharacteristic,
//   uint8_t* pData,
//   size_t length,
//   bool isNotify) {
//     Serial.print("Notify callback for characteristic ");
//     Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//     Serial.print(" of data length ");
//     Serial.println(length);
//     Serial.print("data: ");
//     Serial.println((char*)pData);
// }

// class MyClientCallback : public BLEClientCallbacks {
//   void onConnect(BLEClient* pclient) {
//   }

//   void onDisconnect(BLEClient* pclient) {
//     connected = false;
//     Serial.println("onDisconnect");
//   }
// };

// //Callback function that gets called, when another device's advertisement has been received
// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     if (advertisedDevice.getName() == bleServerName) { //Check if the name of the advertiser matches
//       advertisedDevice.getScan()->stop(); //Scan can be stopped, we found what we are looking for
//       pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
//       doConnect = true; //Set indicator, stating that we are ready to connect
//       Serial.println("Device found. Connecting!");
//     }
//   }
// };


// static void CANNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
//   //store can value
//   // contenuBT = (char*)pData;
//   newCAN = true;
//   readDATA((char*)pData);
// }


