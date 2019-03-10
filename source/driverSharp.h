#include "sharp/Adafruit_SharpMem.h"

class Sharp {
private:
	static Adafruit_SharpMem display;
	static TaskHandle_t taskHandle;
	static bool holdRendering;

public:
	static void begin(void);

	inline static void pause(void) {
		holdRendering = true;
	}

	inline static void unpause(void) {
		holdRendering = false;
	}

private:
	static void updateTask(void *);
};

