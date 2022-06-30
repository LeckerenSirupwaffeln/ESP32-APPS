/**
 * @file ConnectToPeripheral.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Connect this central device to a peripheral device. Belongs to Central_Lightshow_Main.ino
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Fully open-source
 * 
 */

void ConnectToPeripheral() {
  BLEDevice peripheral;

  do {
    //BLE.scanForName("VoiceTrainer Hub");
    BLE.scanForUuid(UUIDs[VoiceTrainer][Service]);
    peripheral = BLE.available();

    if (peripheral) {
    //Serial.println("Oder hier");
      if (peripheral.connect()) {
        Serial.println("Successfully connected to peripheral");
        BLE.stopScan();
      } else {
        Serial.println("Failed to connect!");
        }
    }
  } while (!peripheral.connected());

  //switch LED on and off to show that connection was successfull
  digitalWrite(GREEN, LOW);
  delay(500);
  digitalWrite(GREEN, HIGH); 

  InteractWithPeripheral(peripheral);
}