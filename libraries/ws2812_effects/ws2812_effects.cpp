#include "FastLED.h"
#include "ws2812_effects.h"


WS2812Effecter::WS2812Effecter(CRGB* leds, byte led_num, effect_t effect, byte brightness) 
: leds(leds), led_num(led_num), effect(effect), brightness(brightness) {}

void WS2812Effecter::Show(){
	Serial.println(effect);
	switch (effect)
	{
	case off: break;
	case white: break;
	case rainbow_fade: rainbow_fade_ef(); break;
	
	default:
		SetEffect(off);
		Show();
		break;
	}
}

void WS2812Effecter::SetEffect(effect_t new_effect){
	effect = new_effect;
	sat = 255;
	switch (new_effect)
	{
	case off: SetColorHSV(0, 0, 0); break;
	case white: SetColorHSV(0, 0, 255); break;
	case rainbow_fade: delay_time = 20; break;
	case rainbow_loop: delay_time = 32; step = 32; break;
	case new_rainbow_loop: delay_time = 50; break;
	}
}

void WS2812Effecter::SetColorHSV(byte h, byte s, byte v){
	hue = h;
	sat = s;
	val = v;

	one_color_all();
}

void WS2812Effecter::SetColorRGB(byte r, byte g, byte b){
	CHSV hsv = rgb2hsv_approximate(CRGB(r, g ,b));
	hue = hsv.h;
	sat = hsv.s;
}

void WS2812Effecter::one_color_all(){
	for (int i = 0 ; i < led_num; i++ ) {
		leds[i] = CHSV(hue, sat, val);
	}
	LEDS.show();
}

void WS2812Effecter::rainbow_fade_ef(){
	hue = hue != 255 ? hue + 1 : 0;
	for (int i = 0 ; i < led_num; i++ ) {
		leds[i] = CHSV(hue, sat, 255);
	}
	Serial.println(hue);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::rainbow_loop_ef(){
	index++;
	hue = hue + step;
	if (index >= led_num) {
		index = 0;
	}
	if (hue > 255) {
		hue = 0;
	}
	leds[index] = CHSV(hue, sat, 255);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::new_rainbow_loop_ef(){
	hue--;
	fill_rainbow(leds, led_num, hue);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

bool WS2812Effecter::safeDelay(int del_time) {
	uint32_t thisTime = millis();
	while (millis() - thisTime <= del_time) {
		delay(1);
	}
	return false;
}
