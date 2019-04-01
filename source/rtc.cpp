/**
 * @file rtc.cpp
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

#include "rtc.hpp"
#include <bluefruit.h>

nrf_drv_rtc_t RTC::rtc = NRF_DRV_RTC_INSTANCE(2);
unsigned int RTC::rtcCount = 0;

void RTC::begin(void)
{
	//Initialize RTC instance
	nrf_drv_rtc_config_t rtc_config = {
		4095,
		RTC_DEFAULT_CONFIG_IRQ_PRIORITY,
		RTC_US_TO_TICKS(NRF_MAXIMUM_LATENCY_US, RTC_DEFAULT_CONFIG_FREQUENCY),
		RTC_DEFAULT_CONFIG_RELIABLE
	};
	
	Serial.println(nrf_drv_rtc_init(&rtc, &rtc_config, handler));
	nrf_drv_rtc_tick_enable(&rtc, true);
	nrf_drv_rtc_enable(&rtc);
}

void RTC::handler([[maybe_unused]] nrf_drv_rtc_int_type_t int_type)
{
  static unsigned char counter = 0;
  if (int_type == NRF_DRV_RTC_INT_TICK) {
          if (++counter == 8) {
        	  counter = 0;
        	  rtcCount++;
          }
  }
}

