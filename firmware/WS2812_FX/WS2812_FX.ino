#define LED_COUNT 30
#define LED_DT 6
#define MAX_BRIGHTNESS 100

#include "ws2812_effects.h"


CRGB leds[LED_COUNT];
WS2812Effecter ws2812_ef(leds, LED_COUNT);

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));

	LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);
	LEDS.setBrightness(MAX_BRIGHTNESS);

	ws2812_ef.SetEffect(WS2812Effecter::effect_t::rainbow_fade);
}


void loop() {
	if (Serial.available() > 0) {     // если что то прислали
		int ledMode = Serial.parseInt();    // парсим в тип данных int
		ws2812_ef.SetEffect(ledMode);           // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)
		Serial.println(ledMode);
	}
	ws2812_ef.Show();
}
