#include "FastLED.h"
 

class WS2812Effecter{
public:
	enum effect_t{
		off, white, rainbow_fade, rainbow_loop, new_rainbow_loop
	};

	WS2812Effecter(CHSV* leds, byte led_num, effect_t effect = off, byte brightness = 0);
	void Show();
	void SetEffect(effect_t new_effect);
	void SetColorHSV(byte h, byte s, byte v);
	void SetColorRGB(byte r, byte g, byte b);

private:
	CHSV* leds = nullptr;
	byte led_num = 0;
	byte brightness = 0;

	effect_t effect = off;

	byte index = 0;
	byte hue = 0;
	byte sat = 0;
	byte val = 0;
	byte step = 0;
	byte delay_time = 0;

	void one_color_all();
	void rainbow_fade_ef();
	void rainbow_loop_ef();
	void new_rainbow_loop_ef();

	bool safeDelay(int del_time);
};
