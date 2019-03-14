#ifndef SHARP_HPP_
#define SHARP_HPP_

#include "sharp/Adafruit_SharpMem.h"

#include <functional>

using RenderFunc = std::function<void(Adafruit_GFX&)>;
using Display = Adafruit_GFX;

class Sharp {
private:
	static Adafruit_SharpMem display;
	static TaskHandle_t taskHandle;
	static bool holdRendering;

	static RenderFunc currentScreen;

public:
	static void begin(void);

	inline static void pause(void) {
		holdRendering = true;
	}

	inline static void unpause(void) {
		holdRendering = false;
	}

	inline static void setScreen(const RenderFunc& rf) {
		currentScreen = rf;
	}

private:
	static void updateTask(void *);
};

#endif // SHARP_HPP_

