#include "rtc.hpp"
#include <bluefruit.h>

nrf_drv_rtc_t RTC::rtc = NRF_DRV_RTC_INSTANCE(2);
unsigned int RTC::rtcCount = 0;

char RTC::message[16] = "";

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

void RTC::showTime(Display& display)
{
	static unsigned int oldTicks = 0;
	if (auto t = rtcCount; t != oldTicks) {
		oldTicks = t;

		display.setCursor(0, 10);
		display.printf("%2d:%02d:%02d", t / 3600, (t % 3600) / 60,
			t % 60);
		if (*message != '\0') {
			display.setCursor(0, 100);
			display.printf("%-16s", message);
		}
	}
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

