/**
 * @file CreateFakeValues.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Switches between fake values that can then be sent to the peripheral device.
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

std::string CreateFakeValues() {
  static std::string returnString = "";
  current_time = millis();
  static int switcher = 0;
  if(current_time - last_time > 1000){ 
      last_time = current_time;
      switcher ++;
      if (switcher > 2) switcher = 0;

      switch (switcher) {
        case 0:
          returnString = "b";
          break;
        case 1:
          returnString = "g";
          break;
        case 2:
          returnString = "n";
          break;
        default:
          returnString = "e"; //error
      }
      
  }
    //myString = pRemoteCharacteristicVoiceStrength->readValue();
    //char check[myString.length() + 1] = {};
    //strcpy(check, myString.c_str());
    //Serial.print("Check: ");
    //Serial.println(check);

  return returnString;
}
    
    