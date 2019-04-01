/**
 * @file vibrate.hpp
 * @brief Provides access to the vibration motor.
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

#ifndef VIBRATE_HPP_
#define VIBRATE_HPP_

class Vibrate {
private:
	constexpr static unsigned int motorPin = 27;
	constexpr static unsigned int pulseDuration = 200;

public:
	inline static void begin(void) {
		pinMode(motorPin, OUTPUT);
		digitalWrite(motorPin, false);
	}

	inline static void pulse(void) {
		digitalWrite(motorPin, true);
		delay(pulseDuration);
		digitalWrite(motorPin, false);
	}
};

#endif // VIBRATE_HPP_
