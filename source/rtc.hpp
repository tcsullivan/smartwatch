/**
 * @file rtc.hpp
 * @brief Prepares and uses the RTC to count time.
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

#ifndef RTC_HPP_
#define RTC_HPP_

#include <rtc/nrf_drv_rtc.h>
#include <stdint.h>

#include "sharp.hpp"

class RTC {
private:
	static nrf_drv_rtc_t rtc;
	static uint32_t rtcCount;

public:
	static void begin(void);

	inline static unsigned int ticks(void) {
		return rtcCount;
	}

	inline static void setTicks(unsigned int t) {
		rtcCount = t;
	}

	static const char *getDate(char *buf);

private:
	static void handler(nrf_drv_rtc_int_type_t type);
};

#endif // RTC_HPP_

