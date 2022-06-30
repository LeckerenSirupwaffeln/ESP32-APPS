/**
 * @file Peripheral.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Set up a peripheral device (server) for the voice trainer
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Fully open-source
 * 
 */

// Standard BLE library for Arduinos
#include <ArduinoBLE.h>

#include "UUIDs.h"

// Prototypes for connection handlers
void ConnectHandler(BLEDevice central);
void DisconnectHandler(BLEDevice central);

BLEService VoiceTrainerService(UUIDs[VoiceTrainer][Service]);  // BLE Sensors Service
BLEByteCharacteristic VoiceStrengthCharacteristic(UUIDs[VoiceTrainer][VoiceStrength], BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic VoiceQualityCharacteristic(UUIDs[VoiceTrainer][VoiceQuality], BLERead | BLENotify | BLEWrite);

void setup() {
  Serial.begin(9600);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setEventHandler(BLEConnected, ConnectHandler);
  BLE.setEventHandler(BLEDisconnected, DisconnectHandler);
  BLE.setAdvertisedService(VoiceTrainerService);

  // add the characteristic to the service
  VoiceTrainerService.addCharacteristic(VoiceStrengthCharacteristic);
  VoiceTrainerService.addCharacteristic(VoiceQualityCharacteristic);
  BLE.addService(VoiceTrainerService);

  // set the initial value for the characeristic: 
  VoiceStrengthCharacteristic.writeValue(0);
  VoiceQualityCharacteristic.writeValue(0);

  // start advertising
  BLE.setDeviceName("VoiceTrainer Hub");
  BLE.advertise();

}


void loop() {
  BLE.poll(); 
  //there is nothing else the peripheral needs to do
}


//Function definitions for connection handlers
void ConnectHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  BLE.advertise();
}

void DisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  BLE.advertise();
}
