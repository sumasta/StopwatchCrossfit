

#include "BLEHandler.h"

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setupBLE() {

	SerialBT.begin("SumastaBLETest"); //Bluetooth device name
	Serial.println("The device started, now you can pair it with bluetooth!");
}
