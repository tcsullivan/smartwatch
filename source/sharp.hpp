#ifndef SHARP_HPP_
#define SHARP_HPP_

#include "sharp/Adafruit_SharpMem.h"
#include "widget.hpp"

#include <vector>

#define BLACK 0
#define WHITE 1

class Sharp {
private:
	static Adafruit_SharpMem display;
	static TaskHandle_t taskHandle;

	static std::vector<Widget *> widgets;
public:
	static void begin(void);

	template<class T, typename... Args>
	inline static void addWidget(Args... args) {
		widgets.emplace_back(new T(args...));
	}

private:
	static void updateTask(void *);
};

#endif // SHARP_HPP_

