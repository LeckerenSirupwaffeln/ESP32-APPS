/**
 * @file Central_SendFakeValues_FunctionsAndGlobals.h
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Function prototypes and global variables
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

#include <stdlib.h>
#include <string.h>

/**
 * @brief Create fake values to send them to peripheral.
 * 
 * @return std::string object of the fake values
 */
std::string CreateFakeValues();

/**
 * @brief Connect to the peripheral device.
 * 
 * @return false if unsuccessful
 */
bool connectToServer();

/**
 * @brief Updates the characteristics on the peripheral device.
 * 
 * @param StrengthValue Input. New StrengthValue
 * @param QualityValue Input. New QualityValue
 */
void UpdateCharacteristicOnServer(std::string StrengthValue, std::string QualityValue); //probably should change the return value to check whether update to the server was successful

static BLEUUID ServiceUUID(UUIDs[VoiceTrainer][Service]);
static BLEUUID VoiceStrengthUUID(UUIDs[VoiceTrainer][VoiceStrength]);
static BLEUUID VoiceQualityUUID(UUIDs[VoiceTrainer][VoiceQuality]);


static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristicVoiceStrength;
static BLERemoteCharacteristic* pRemoteCharacteristicVoiceQuality;
static BLEAdvertisedDevice* myDevice;

static unsigned long current_time = 0;
static unsigned long last_time = 0;
