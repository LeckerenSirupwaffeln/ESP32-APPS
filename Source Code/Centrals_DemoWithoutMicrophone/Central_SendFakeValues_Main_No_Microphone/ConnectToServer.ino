/**
 * @file ConnectToServer.ino
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Try to connect to a specified peripheral device
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

bool connectToServer() {
    BLEClient*  pClient  = BLEDevice::createClient();

    pClient->setClientCallbacks(new MyClientCallback());

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

    connected = true;
}