/**
 * @file widget.cpp
 * @brief Defines widgets that may be put on the display.
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

#include "rtc.hpp"
#include "widget.hpp"

PressAction TimeWidget::onPress(void)
{
	if (getHeight() == 30)
		setHeight(SHARP_HEIGHT);
	else
		setHeight(30);

	return PressAction::Fullscreen;
}

void TimeWidget::render(Adafruit_GFX& display, int ypos) 
{
	if (auto t = RTC::ticks(); t != prevTicks) {
		prevTicks = t;
		display.setTextSize(3);
		display.setCursor(0, ypos + 4);
		display.printf("%2d:%02d:%02d", t / 3600, (t % 3600) / 60,
			t % 60);
	}
}

void TimeWidget::renderFullscreen(Adafruit_GFX& display, int ypos)
{
	if (auto t = RTC::ticks(); t != prevTicks) {
		prevTicks = t;
		display.setTextSize(4);
		display.setCursor(12, SHARP_HEIGHT / 3);
		display.printf("%2d:%02d", t / 3600, (t % 3600) / 60);
	}

}

void NotificationWidget::render(Adafruit_GFX& display, int ypos) 
{
	display.setTextSize(2);
	display.setCursor(0, ypos);
	display.printf("%-36s", message);
}

void NoteWidget::render(Adafruit_GFX& display, int ypos)
{
	display.setTextSize(2);
	display.setCursor(0, ypos);
	display.printf("NOTE:\n%9s...", text);
}

void NoteWidget::renderFullscreen(Adafruit_GFX& display, int ypos)
{
	display.setTextSize(2);
	display.setCursor(0, ypos);
	display.print(text);
}

PressAction NoteWidget::onPress(void)
{
	if (getHeight() == 24)
		setHeight(SHARP_HEIGHT);
	else
		setHeight(24);

	return PressAction::Fullscreen;
}

