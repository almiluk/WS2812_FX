#ifndef WS2812_EFFECTS_H
#define WS2812_EFFECTS_H

#define RAINBOW_PART_SIZE 30

#include "FastLED.h"
 

class WS2812Effecter{
public:
	enum effect_t{
		off, white, rainbow_fade, rainbow_loop, new_rainbow_loop, random_burst,
		rainbow_vertical, continuous_vertical_rainbow, partial_rainbow
	};

	WS2812Effecter(CRGB* leds, byte led_num, effect_t effect = off, byte brightness = 0);
	void Show();
	void SetEffect(effect_t new_effect);
	void SetColorHSV(byte h, byte s, byte v);
	void SetColorRGB(byte r, byte g, byte b);

private:
	CRGB* leds = nullptr;
	byte led_num = 0;
	byte brightness = 0;

	effect_t effect = off;

	byte index = 0;
	byte hue = 0;
	byte ihue = 0;
	byte sat = 0;
	byte val = 255;
	int8_t step = 0;
	byte delay_time = 0;

	void one_color_all();
	void rainbow_fade_ef();
	void rainbow_loop_ef();
	void new_rainbow_loop_ef();
	void random_burst_ef();
	void rainbow_vertical_ef();
	void continuous_vertical_rainbow_ef();
	void partial_rainbow_ef();

	byte horizontal_index(byte i);
	bool safeDelay(int del_time);
	bool is_near_hsv(byte c1, byte c2, byte range);
};

#endif
