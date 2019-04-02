/**
 * @file sharp.cpp
 * @brief Handles all drawing to the display.
 *
 * Copyright (C) 2019 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "sharp.hpp"
#include "rtc.hpp"

constexpr unsigned int SHARP_SCK = 12;
constexpr unsigned int SHARP_MOSI = 13;
constexpr unsigned int SHARP_SS = 14;

Adafruit_SharpMem Sharp::display(SHARP_SCK, SHARP_MOSI, SHARP_SS, SHARP_WIDTH,
	SHARP_HEIGHT);
TaskHandle_t Sharp::taskHandle;
std::vector<Widget *> Sharp::widgets;

int Sharp::fullscreenWidget = -1;
int Sharp::topY = 0;
int Sharp::lowestY = 0;

static int oldTopY = 0;

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
			switch (widgets[i]->onPress()) {
			case PressAction::Destroy:
				delete widgets[i];
				widgets.erase(widgets.begin() + i);
				display.clearDisplay();
				break;
			case PressAction::Fullscreen:
				if (fullscreenWidget == -1)
					fullscreenWidget = i;
				else
					fullscreenWidget = -1;
				oldTopY = 1;
				break;
			default:
				break;
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
	while (1) {
		if (oldTopY != topY) {
			oldTopY = topY;
			display.clearDisplay();
		}

		if (fullscreenWidget == -1) {
			auto y = topY;
			for (auto& w : widgets) {
				w->render(display, y);
				y += w->getHeight();
    				display.drawFastHLine(0, y + 1, SHARP_WIDTH,
					BLACK);
				y += 3;
				if (y >= SHARP_HEIGHT)
					break;
			}
		} else {
			widgets[fullscreenWidget]->renderFullscreen(display,
				topY);
		}

		display.refresh();
		delay(150);
	}
}

