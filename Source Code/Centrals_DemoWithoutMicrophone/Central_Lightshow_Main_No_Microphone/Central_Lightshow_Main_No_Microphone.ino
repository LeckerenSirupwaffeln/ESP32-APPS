/**
 * @file Central_Lightshow_Main.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Same as the usual Central_Lightshow. Only difference is in TurnOnlights.ino since it needed to be adapted to the different status values.
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Fully open-source
 * 
 */

#include "UUIDs.h"
#include "Central_Lightshow_FunctionsAndGlobals.h"

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.begin(9600);

  if (!BLE.begin()) {
    Serial.println("* Starting BLE module failed!");
    while (1);
  }

  //turn all LEDs off at start
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);

}


// execution happens here
void loop() {
  ConnectToPeripheral();
}
