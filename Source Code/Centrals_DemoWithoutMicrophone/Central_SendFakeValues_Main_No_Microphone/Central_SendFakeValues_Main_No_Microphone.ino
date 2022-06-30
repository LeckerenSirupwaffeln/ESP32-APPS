/**
 * @file Central_SendFakeValues_Main_No_Microphone.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Main file. Create fake values and send them to the peripheral device for demo purposes. Uses BLEDevice.h instead of ArduinoNLE.h so it can be used on ESPs
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

#include <BLEDevice.h>
#include "UUIDs.h"
#include "Central_SendFakeValues_FunctionsAndGlobals.h"
#include "Callbacks.h"

void setup() {
  //Serial.begin(115200);
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  do{
    pBLEScan->start(5, false);
  }while (doConnect == false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {
  static std::string oldStrengthValue = "0";
  static std::string newStrengthValue = "0";
  static std::string oldQualityValue = "0";
  static std::string newQualityValue = "0";

  if (doConnect == true) {
    do{
      connectToServer();
      delay(2000);
    }while (connected != true);
    doConnect = false;
  }

  while (connected) {
    newQualityValue = CreateFakeValues();
    if (newStrengthValue.compare(oldStrengthValue) != 0 || newQualityValue.compare(oldQualityValue) != 0) {
      UpdateCharacteristicOnServer(newStrengthValue, newQualityValue);
      oldStrengthValue = newStrengthValue;
      oldQualityValue = newQualityValue;
    }
    
  }
  if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
} // End of loop
