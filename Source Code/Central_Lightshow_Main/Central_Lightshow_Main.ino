/**
 * @file Central_Lightshow_Main.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Main file for Central_Lightshow (Arduino). Central_Lightshow reads status values from a peripheral device and represents them with LEDs
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
