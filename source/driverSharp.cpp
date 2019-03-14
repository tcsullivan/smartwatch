#include "driverSharp.h"
#include "rtc.hpp"

constexpr unsigned int SHARP_SCK = 12;
constexpr unsigned int SHARP_MOSI = 13;
constexpr unsigned int SHARP_SS = 14;

Adafruit_SharpMem Sharp::display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);
TaskHandle_t Sharp::taskHandle;
bool Sharp::holdRendering = false;
char Sharp::message[16] = "";

#define BLACK 0
#define WHITE 1

void Sharp::begin(void)
{
	display.begin();
	display.clearDisplay();
	display.setTextSize(3);
	display.setTextColor(BLACK, WHITE);

	xTaskCreate(updateTask, "sharp", 512, nullptr, TASK_PRIO_LOW,
		&taskHandle);
}

void Sharp::updateTask([[maybe_unused]] void *arg)
{
	static auto old = RTC::ticks();
	while (1) {
		do {
			delay(300);
		} while (holdRendering);

		if (auto t = RTC::ticks(); t != old) {
			old = t;
			display.setCursor(0, 10);
			display.printf("%2d:%02d:%02d", t / 3600, (t % 3600) /
				60, t % 60);
			if (*message != '\0') {
				display.setCursor(0, 100);
				display.printf("%-16s", message);
			}
			display.refresh();
		}
	}
}

