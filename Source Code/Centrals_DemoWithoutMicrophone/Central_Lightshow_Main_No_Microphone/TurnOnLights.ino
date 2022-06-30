/**
 * @file TurnOnLights.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Turn on different LEDs depending on the characteristic's value. Belongs to Central_Lightshow_Main_No_Microphone.ino
 * @param StatusReadFromPeripheral Input. Fake value that was read from the peripheral device.
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

//WRITE LOW to turn LED on
void OnlyOneLight(byte desiredLED);

void TurnOnLights(byte StatusReadFromPeripheral) {
  switch (StatusReadFromPeripheral) {
    case 'b':
      OnlyOneLight(RED);
      break;
    case 'g':
      OnlyOneLight(GREEN);
      break;
    case 'n':
      OnlyOneLight(BLUE);
      break;
    case leds_all: //turn all on
      digitalWrite(RED, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(BLUE, LOW);
    default: //turn all off
      digitalWrite(RED, HIGH);
      digitalWrite(GREEN, HIGH);
      digitalWrite(BLUE, HIGH);
  }
}


void OnlyOneLight(byte desiredLED) {
  int i;
  for (i=RED; i<=BLUE; i++) {
    if (i == desiredLED) digitalWrite(i, LOW);
    else digitalWrite(i, HIGH);
  }
}