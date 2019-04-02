/**
 * @file sharp.hpp
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

#ifndef SHARP_HPP_
#define SHARP_HPP_

#include "sharp/Adafruit_SharpMem.h"
#include "widget.hpp"

#include <vector>

#define BLACK 0
#define WHITE 1

constexpr int SHARP_WIDTH = 144;
constexpr int SHARP_HEIGHT = 168;

class Sharp {
private:
	static Adafruit_SharpMem display;
	static TaskHandle_t taskHandle;

	static std::vector<Widget *> widgets;
	static int fullscreenWidget;

	static int topY;
	static int lowestY;

public:
	static void begin(void);

	template<class T, typename... Args>
	inline static void addWidget(Args... args) {
		widgets.emplace_back(new T(args...));
		lowestY = 0;
		for (unsigned int i = 0; i < widgets.size() - 1; i++)
			lowestY -= widgets[i]->getHeight() + 3;
		if (lowestY != 0)
			lowestY++;
	}

	inline static void setScroll(int scr = 0) {
		static int oldTopY = 0;
		if (scr == 0) {
			oldTopY = topY;
		} else {
			topY = minof(0, oldTopY + scr);
			topY = maxof(lowestY, topY);
		}
	}

	static void sendInput(int ypos);

private:
	static void updateTask(void *);
};

#endif // SHARP_HPP_

