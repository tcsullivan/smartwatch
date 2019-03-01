#include "sharp/Adafruit_SharpMem.h"

#define SHARP_SCK  12
#define SHARP_MOSI 13
#define SHARP_SS   6

Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);

#define BLACK 0
#define WHITE 1

void sharpInit(void)
{
	display.begin();
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(0, 0);
	display.println("Hello!");
}

void sharpRefresh(unsigned int pause)
{
	display.refresh();
	delay(pause);
}

