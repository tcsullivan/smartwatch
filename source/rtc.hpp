#include <rtc/nrf_drv_rtc.h>
#include "sharp.hpp"

class RTC {
private:
	static nrf_drv_rtc_t rtc;
	static unsigned int rtcCount;

	static char message[16];

public:
	static void begin(void);

	inline static unsigned int ticks(void) {
		return rtcCount;
	}

	inline static void setTicks(unsigned int t) {
		rtcCount = t;
	}

	static void showTime(Display& display);

	inline static void setMessage(const char *s) {
		strncpy(message, s, 16);
	}

private:
	static void handler(nrf_drv_rtc_int_type_t type);
};

