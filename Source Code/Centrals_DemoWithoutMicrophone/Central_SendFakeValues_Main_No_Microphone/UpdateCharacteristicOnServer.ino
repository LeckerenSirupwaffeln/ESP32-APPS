/**
 * @file UpdateCharacteristicOnServer.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Updates the characteristics on the peripheral device.
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

void UpdateCharacteristicOnServer(std::string StrengthValue, std::string QualityValue) {
  pRemoteCharacteristicVoiceStrength->writeValue(StrengthValue.c_str(), StrengthValue.length());
  pRemoteCharacteristicVoiceQuality->writeValue(QualityValue.c_str(), QualityValue.length());
}