/**
 * @file Central_Lightshow_FunctionsAndGlobals
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Function prototypes and global variables for Central_Lightshow
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Fully open-source
 * 
 */

BLECharacteristic VoiceCharacteristics[numVoiceCharacteristics];

/**
 * @brief Connect this central device to a peripheral device.
 * 
 */
void ConnectToPeripheral();

/**
 * @brief Get characteristics from peripheral device, subscribe to them and check the characteristics' values that are stored on it.
 * 
 * @param peripheral Input. It's the peripheral device that this central device has connected to.
 */
void InteractWithPeripheral(BLEDevice peripheral);

/**
 * @brief Check whether a characteristic's value was updated.
 * 
 */
void CheckSensorData();

/**
 * @brief Adjust the brightness of the LEDs according to the characteristic's value.
 * 
 * @param StatusReadFromPeripheral Input. Voice quality value that was read from the peripheral device.
 */
void TurnOnLights(byte StatusReadFromPeripheral);

enum LEDs{led_red, led_green, led_blue, leds_all};
enum LED_Pins{RED=22, GREEN, BLUE}; //Pins on the Arduino Nano 33 BLE Sense for the onboard LEDs
