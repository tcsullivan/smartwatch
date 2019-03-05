#include "sharp/Adafruit_SharpMem.h"

#define SHARP_SCK  12
#define SHARP_MOSI 13
#define SHARP_SS   6

Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);

#define BLACK 0
#define WHITE 1

static TaskHandle_t sharpHandle;
void sharpTask(void *arg);

void sharpInit(void)
{
	display.begin();
	display.clearDisplay();
	display.setTextSize(3);
	display.setTextColor(BLACK);
	display.setCursor(0, 0);
	display.println("Hello!");

	xTaskCreate(sharpTask, "sharp", 512, nullptr, TASK_PRIO_LOW, &sharpHandle);
}

void sharpTask([[maybe_unused]] void *arg)
{
	while (1) {
		display.refresh();
		delay(500);
	}
}

