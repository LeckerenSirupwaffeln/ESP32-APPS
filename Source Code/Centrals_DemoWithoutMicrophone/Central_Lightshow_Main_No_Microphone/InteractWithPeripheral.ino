/**
 * @file InteractWithPeripheral.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Get characteristics from peripheral device, subscribe to them and check the characteristics' values that are stored on it. Belongs to Central_Lightshow_Main.ino
 * @param peripheral Input. It's the peripheral device that this central device has connected to.
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Fully open-source
 * 
 */

void InteractWithPeripheral(BLEDevice peripheral) {
  if (!peripheral.discoverAttributes()) { //probably never gonna happen
    peripheral.disconnect();
    return;
  }

  // get Characteristics from Peripheral and subscribe to them
  VoiceCharacteristics[VoiceStrength] = peripheral.characteristic(UUIDs[VoiceTrainer][VoiceStrength]);
  VoiceCharacteristics[VoiceQuality] = peripheral.characteristic(UUIDs[VoiceTrainer][VoiceQuality]);

  VoiceCharacteristics[VoiceStrength].subscribe();
  VoiceCharacteristics[VoiceQuality].subscribe();

  
  while (peripheral.connected()) {
    CheckSensorData();
  }
}