/**
 * @file Callbacks.h
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Defines own callbacks for connecting this central device to a peripheral device.
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Fully open-source
 * 
 */

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  //... needs to be here
}


class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    //... same for this, don't delete
  }

  void onDisconnect(BLEClient* pclient) {
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

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks