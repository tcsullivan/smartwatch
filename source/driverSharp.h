#include "sharp/Adafruit_SharpMem.h"

class Sharp {
private:
	static Adafruit_SharpMem display;
	static TaskHandle_t taskHandle;
	static bool holdRendering;
	static char message[16];

public:
	static void begin(void);

	inline static void pause(void) {
		holdRendering = true;
	}

	inline static void unpause(void) {
		holdRendering = false;
	}

	inline static void setMessage(const char *s) {
		strncpy(message, s, 16);
	}

private:
	static void updateTask(void *);
};

