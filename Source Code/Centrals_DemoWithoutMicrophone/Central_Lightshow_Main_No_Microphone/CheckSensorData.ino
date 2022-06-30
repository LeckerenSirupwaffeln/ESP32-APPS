/**
 * @file CheckSensorData.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Check whether a characteristic's value was updated. Belongs to Central_Lightshow_Main.ino
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Fully open-source
 * 
 */

void CheckSensorData() {    
  static byte VoiceQualityStatus;
  static byte VoiceStrengthStatus;
  static unsigned long currentTime;
  static unsigned long lastTime = 0;
  static bool timeoutUpdate = false;
  
  // timeout (watch dog timer) because the device is sometimes stuck on an old value because it has somehow missed the notification for an updated value on the peripheral
  currentTime = millis();
  if (currentTime - lastTime > 3000) {
    lastTime = currentTime;
    timeoutUpdate = true;
  }

  if (VoiceCharacteristics[VoiceStrength].valueUpdated() || VoiceCharacteristics[VoiceQuality].valueUpdated() || timeoutUpdate) {
    if (timeoutUpdate) timeoutUpdate = false; //reset timeout
    
    //VoiceCharacteristics[VoiceStrength].readValue(VoiceStrength);
    VoiceCharacteristics[VoiceQuality].readValue(VoiceQualityStatus);

    //Print out the current value (debugging)
    //Serial.print("VoiceQuality Value (Central): "); //------------------------------------------
    //Serial.print((char) VoiceQualityStatus); //------------------------------------------
    //Serial.println(VoiceQualityStatus); //------------------------------------------

    TurnOnLights(VoiceQualityStatus);
  }
}