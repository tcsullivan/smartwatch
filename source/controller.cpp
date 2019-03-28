// sharp takes < ~0.05uA

#include <bluefruit.h>

#include "rtc.hpp"
#include "sharp.hpp"
#include "widget.hpp"

BLEUart bleuart;

void handlePacket(void);

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
	Sharp::addWidget<TimeWidget>();
	Sharp::addWidget<NotificationWidget>("Welcome to smartwatch");
	Sharp::addWidget<NotificationWidget>("Hi");
	Sharp::addWidget<NotificationWidget>("Once upon a time, there lived an old man. His name was Gerg.");
	Sharp::addWidget<NotificationWidget>("NOTICE: Play more games");
	Sharp::addWidget<NotificationWidget>("2 new messages");
	Sharp::addWidget<NotificationWidget>("you have mail");
	Sharp::addWidget<NotificationWidget>("GGGGGGGGGGGGGGGGGGGGGGG");
	Sharp::addWidget<NotificationWidget>("ABCDEFGHIJKLMNOPQRSTUZWXYZ");
}

void loop(void)
{
	if (bleuart.available())
		handlePacket();

	static int last = 0;
	static bool scrolled = false;
	int val = analogRead(A5);
	if (val >= 10) {
		if (last == 0) {
			scrolled = false;
			last = val;
		}

		auto diff = val - last;
		if (diff > 50) {
			Sharp::setScrollVelocity(1);
			scrolled = true;
		} else if (diff < -50) {
			Sharp::setScrollVelocity(-1);
			scrolled = true;
		} else {
			Sharp::setScrollVelocity(0);
		}
	} else {
		if (last != 0 && !scrolled) {
			int ypos = last / 70 * (SHARP_HEIGHT / 10);
			Sharp::sendInput(ypos);
		}
		last = 0;
		Sharp::setScrollVelocity(0);
	}

	delay(10);
}

void handlePacket(void)
{
	char buf[64];
	unsigned int i;
	for (i = 0; bleuart.available() && i < 63; i++)
		buf[i] = bleuart.read();
	buf[i] = '\0';

	switch (buf[0]) {
	case 'L':
		//RTC::setMessage(buf + 1);
		break;
	case 'T':
		Sharp::addWidget<NotificationWidget>("Time updated");
		RTC::setTicks(std::atoi(buf + 1) * 60);
		break;
	default:
		break;
	}
}

