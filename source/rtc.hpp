#include <rtc/nrf_drv_rtc.h>

class RTC {
private:
	static nrf_drv_rtc_t rtc;
	static unsigned int rtcCount;

public:
	static void begin(void);

	inline static unsigned int ticks(void) {
		return rtcCount;
	}

private:
	static void handler(nrf_drv_rtc_int_type_t type);
};

