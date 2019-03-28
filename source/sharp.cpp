#include "sharp.hpp"
#include "rtc.hpp"

constexpr unsigned int SHARP_SCK = 12;
constexpr unsigned int SHARP_MOSI = 13;
constexpr unsigned int SHARP_SS = 14;

Adafruit_SharpMem Sharp::display(SHARP_SCK, SHARP_MOSI, SHARP_SS, SHARP_WIDTH,
	SHARP_HEIGHT);
TaskHandle_t Sharp::taskHandle;
std::vector<Widget *> Sharp::widgets;

int Sharp::topY = 0;
int Sharp::scrollVelocity = 0;

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

void Sharp::sendInput(int ypos)
{
	if (ypos < 0 || ypos > SHARP_HEIGHT)
		return;

	int y = topY;
	for (unsigned int i = 0; i < widgets.size(); i++) {
		y += widgets[i]->getHeight();
		if (ypos < y) {
			if (widgets[i]->onPress()) {
				delete widgets[i];
				widgets.erase(widgets.begin() + i);
				display.clearDisplay();
			}
			break;
		}

		y += 3;
		if (y >= SHARP_HEIGHT)
			break;
	}
}

void Sharp::updateTask([[maybe_unused]] void *arg)
{
	static unsigned int counter = 0;
	while (1) {
		if (counter++ == 3) {
			counter = 0;

			auto y = topY;
			for (auto& w : widgets) {
				w->render(display, y);
				y += w->getHeight();
    				display.drawFastHLine(0, y + 1, SHARP_WIDTH, BLACK);
				y += 3;
				if (y >= SHARP_HEIGHT)
					break;
			}

			display.refresh();
		} else {
			topY += scrollVelocity * 20;
			if (scrollVelocity != 0)
				display.clearDisplay();
			if (topY > 0)
				topY = 0;
		}

		delay(50);
	}
}

