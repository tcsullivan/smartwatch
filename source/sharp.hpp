#ifndef SHARP_HPP_
#define SHARP_HPP_

#include "sharp/Adafruit_SharpMem.h"
#include "widget.hpp"

#include <vector>

#define BLACK 0
#define WHITE 1

constexpr int SHARP_WIDTH = 144;
constexpr int SHARP_HEIGHT = 168;

class Sharp {
private:
	static Adafruit_SharpMem display;
	static TaskHandle_t taskHandle;

	static std::vector<Widget *> widgets;

	static int topY;
	static int scrollVelocity;

public:
	static void begin(void);

	template<class T, typename... Args>
	inline static void addWidget(Args... args) {
		widgets.emplace_back(new T(args...));
	}

	inline static void setScrollVelocity(int vel) {
		scrollVelocity = vel;
	}

	static void sendInput(int ypos);

private:
	static void updateTask(void *);
};

#endif // SHARP_HPP_

