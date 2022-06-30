/**
 * @file TurnOnLights.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Adjust the brightness of the LEDs according to the characteristic's value. Belongs to Central_Lightshow_Main.ino
 * @param StatusReadFromPeripheral Input. Voice quality value that was read from the peripheral device.
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

void TurnOnLights(byte StatusReadFromPeripheral) {
  analogWrite(RED, StatusReadFromPeripheral);
  analogWrite(BLUE, (255-StatusReadFromPeripheral));
}