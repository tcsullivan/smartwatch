// sharp takes < ~0.05uA

#include <bluefruit.h>

#include "driverSharp.h"
#include "rtc.hpp"

BLEUart bleuart;

void app_error_handler_bare([[maybe_unused]] uint32_t error_code)
{
    Serial.println("Received an error");
    while (1);
}

void setup(void)
{
	Serial.begin(115200);
	while (!Serial)
		delay(10);

	Serial.println(F("Initializing..."));

	RTC::begin();
	Sharp::begin();

	Bluefruit.begin();
	// Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
	Bluefruit.setTxPower(-20);
	Bluefruit.setName("smartwatch");

	// Configure and start the BLE Uart service
	bleuart.begin();

	// Advertising packet
	Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
	Bluefruit.Advertising.addTxPower();
	Bluefruit.Advertising.addService(bleuart);
	Bluefruit.ScanResponse.addName();
	Bluefruit.Advertising.restartOnDisconnect(true);
	Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
	Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
	Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  

	Serial.println(F("Ready."));
}

void loop(void)
{
	if (bleuart.available())
		Serial.print((char)bleuart.read());

	delay(10);
}
