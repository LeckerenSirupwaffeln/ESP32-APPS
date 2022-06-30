/**
 * @file BLE.cpp
 * @author Julian Gaenser (julian.ganser@smail.emt.h-brs.de)
 * @brief BLE module used to interface with Arduino's BLE library. Very similar to Central_SendFakeValues_Main_No_Microphone but now with real values from a microphone.
 * @version 0.1
 * @date 2022-06-25
 * 
 * @copyright Fully open-source
 * 
 */

//Standard library
#include <string>

//ESP-IDF library
#include "esp_log.h"

//Arduino library
#include "BLEDevice.h"

//Main header
#include "BLE.hpp"

enum Services {VoiceTrainer, Peltier, numServices};
enum Attributes {Service, FirstCharacteristic, SecondCharacteristic, ThirdCharacteristic, FourthCharacteristic, /* add further characteristics if needed */ numAttributes};
enum VoiceTrainerAttributes {VoiceStrength = 1, VoiceQuality = 2};

constexpr char UUIDs[numServices][numAttributes][37] = {
	//VoiceTrainer:
	 //Service UUID
	{"19B10000-E8F2-537E-4F6C-000000000000",
	 //UUIDs of characteristics
	 "19B10000-E8F2-537E-4F6C-111111111111", "19B10000-E8F2-537E-4F6C-222222222222",
	 "19B10000-E8F2-537E-4F6C-333333333333", "19B10000-E8F2-537E-4F6C-444444444444"},

	//Peltier:
	 //Service UUID
	{"19B10000-E8F2-537E-1111-000000000000",
	 //UUIDs of characteristics
	 "19B10000-E8F2-537E-1111-111111111111", "19B10000-E8F2-537E-1111-222222222222",
	 "19B10000-E8F2-537E-1111-333333333333", "19B10000-E8F2-537E-1111-444444444444"},
};

BLEUUID ServiceUUID(UUIDs[VoiceTrainer][Service]);
BLEUUID VoiceStrengthUUID(UUIDs[VoiceTrainer][VoiceStrength]);
BLEUUID VoiceQualityUUID(UUIDs[VoiceTrainer][VoiceQuality]);

bool doConnect = false;
bool connected = false;
bool doScan = false;
BLERemoteCharacteristic* pRemoteCharacteristicVoiceStrength;
BLERemoteCharacteristic* pRemoteCharacteristicVoiceQuality;
BLEAdvertisedDevice* myDevice;

class MyClientCallback : public BLEClientCallbacks {
	void onConnect(BLEClient* pclient)
	{
		//nothing needs to be defined but the function has to be declared
	}

	void onDisconnect(BLEClient* pclient)
	{
		connected = false;
		doConnect = true;
	}
};


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(ServiceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

bool connectToServer() {
    BLEClient*  pClient  = BLEDevice::createClient();

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    // ---------------------- only tries it once, means that peripheral has to run before esp is started / otherise the time of scanning in the void setup
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    BLERemoteService* pRemoteService = pClient->getService(ServiceUUID);
    if (pRemoteService == nullptr) {
      pClient->disconnect();
      return false;
    }

    pRemoteCharacteristicVoiceStrength = pRemoteService->getCharacteristic(VoiceStrengthUUID);
    pRemoteCharacteristicVoiceQuality = pRemoteService->getCharacteristic(VoiceQualityUUID);
    if (pRemoteCharacteristicVoiceStrength == nullptr || pRemoteCharacteristicVoiceQuality == nullptr) {
      pClient->disconnect();
      return false;
    }

    return connected = true;
}

void UpdateCharacteristicOnServer(std::string StrengthValue, std::string QualityValue) {
	pRemoteCharacteristicVoiceStrength->writeValue(StrengthValue.c_str(), StrengthValue.length());
	pRemoteCharacteristicVoiceQuality->writeValue(QualityValue.c_str(), QualityValue.length());
}

void setup_ble() {
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  do {
	  pBLEScan->start(5, false);
  } while (doConnect == false);
}

void try_send_voice_values(const std::string& strength_value, const std::string& quality_value)
{
	static std::string old_strength_value;
	static std::string old_quality_value;

	if (doConnect == true)
	{
		do
		{
			connectToServer();
			//vTaskDelay(2000 / portTICK_RATE_MS);
		} while (connected != true);

		doConnect = false;
	}

	if (connected)
	{
		if (strength_value.compare(old_strength_value) != 0 || quality_value.compare(old_quality_value) != 0)
		{
			const char value = quality_value[0];
			ESP_LOGI("BLE", "Sending value: %d", value);
			UpdateCharacteristicOnServer(strength_value, quality_value);
			old_strength_value = strength_value;
			old_quality_value = quality_value;
		}
	}

//	if (doScan){
//		BLEDevice::getScan()->start(0);
//	}

}
