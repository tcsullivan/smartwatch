#ifndef WIDGET_HPP_
#define WIDGET_HPP_

class Adafruit_GFX;

#include <bluefruit.h>
#include <cstring>

class Widget {
private:
	unsigned int height;
public:
	constexpr Widget(unsigned int _height = 0)
		: height(_height) {}
	
	inline unsigned int getHeight(void) const {
		return height;
	}

	virtual void render(Adafruit_GFX& display, int ypos) = 0;
	virtual bool onPress(void) = 0;

protected:
	inline void setHeight(unsigned int _height) {
		height = _height;
	}
};

class TimeWidget : public Widget {
private:
	unsigned int prevTicks;

public:
	constexpr TimeWidget(void)
		: Widget(30), prevTicks(0) {}

	void render(Adafruit_GFX& display, int ypos) final;
	bool onPress(void) final {
		return false;
	}
};

class NotificationWidget : public Widget {
private:
	char message[36];

public:
	NotificationWidget(const char *msg)
		: Widget(48) {
		unsigned int i;
		for (i = 0; msg[i] != '\0' && i < 35; i++)
			message[i] = msg[i];
		message[i] = '\0';
		setHeight(16 * (i / 12 + 1));
	}

	void render(Adafruit_GFX& display, int ypos) final;
	bool onPress(void) final {
		return true;
	}
};

#endif // WIDGET_HPP_
