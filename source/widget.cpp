#include "rtc.hpp"
#include "widget.hpp"

void TimeWidget::render(Adafruit_GFX& display, int ypos) 
{
	if (auto t = RTC::ticks(); t != prevTicks) {
		prevTicks = t;
		display.setTextSize(3);
		display.setCursor(0, ypos + 4);
		display.printf("%2d:%02d:%02d", t / 3600, (t % 3600) /
			60, t % 60);
	}
}

void NotificationWidget::render(Adafruit_GFX& display, int ypos) 
{
	display.setTextSize(2);
	display.setCursor(0, ypos);
	display.printf("%-36s", message);
}
