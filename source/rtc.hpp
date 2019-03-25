#ifndef RTC_HPP_
#define RTC_HPP_

#include <rtc/nrf_drv_rtc.h>
#include "sharp.hpp"

class RTC {
private:
	static nrf_drv_rtc_t rtc;
	static unsigned int rtcCount;

public:
	static void begin(void);

	inline static unsigned int ticks(void) {
		return rtcCount;
	}

	inline static void setTicks(unsigned int t) {
		rtcCount = t;
	}

private:
	static void handler(nrf_drv_rtc_int_type_t type);
};

#endif // RTC_HPP_

