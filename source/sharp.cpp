#include "sharp.hpp"
#include "rtc.hpp"

constexpr unsigned int SHARP_WIDTH = 144;
constexpr unsigned int SHARP_HEIGHT = 168;

constexpr unsigned int SHARP_SCK = 12;
constexpr unsigned int SHARP_MOSI = 13;
constexpr unsigned int SHARP_SS = 14;

Adafruit_SharpMem Sharp::display(SHARP_SCK, SHARP_MOSI, SHARP_SS, SHARP_WIDTH,
	SHARP_HEIGHT);
TaskHandle_t Sharp::taskHandle;
std::vector<Widget *> Sharp::widgets;

void Sharp::begin(void)
{
	widgets.reserve(10);
	display.begin();
	display.clearDisplay();
	display.setTextSize(3);
	display.setTextColor(BLACK, WHITE);

	xTaskCreate(updateTask, "sharp", 512, nullptr, TASK_PRIO_LOW,
		&taskHandle);
}

void Sharp::updateTask([[maybe_unused]] void *arg)
{
	while (1) {
		unsigned int y = 0;
		for (auto& w : widgets) {
			w->render(display, y);
			y += w->getHeight();
    			display.drawFastHLine(0, y + 1, SHARP_WIDTH, BLACK);
			y += 3;
			if (y >= SHARP_HEIGHT)
				break;
		}

		display.refresh();
		delay(300);
	}
}

