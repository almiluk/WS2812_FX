#include "FastLED.h"
#include "ws2812_effects.h"


WS2812Effecter::WS2812Effecter(CRGB* leds, byte led_num, effect_t effect, byte brightness) 
: leds(leds), led_num(led_num), effect(effect), brightness(brightness) {}

void WS2812Effecter::Show(){
	switch (effect)
	{
	case off: break;
	case white: break;
	case rainbow_fade: rainbow_fade_ef(); break;
	case rainbow_loop: rainbow_loop_ef(); break;
	case new_rainbow_loop: new_rainbow_loop_ef(); break;
	case random_burst: random_burst_ef(); break;
	case rainbow_vertical: rainbow_vertical_ef(); break;
	case continuous_vertical_rainbow: continuous_vertical_rainbow_ef(); break;
	
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
	case new_rainbow_loop: delay_time = 50; step = 5; break;
	case random_burst: delay_time = 30; break;
	case rainbow_vertical: step = 15; delay_time = 50; break;
	case continuous_vertical_rainbow: step = 5; delay_time = 15; break;
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
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::rainbow_loop_ef(){
	hue = hue + step <= 255 ? hue + step : 0;
	index = index < led_num - 1 ? index + 1 : 0;
	leds[index] = CHSV(hue, sat, 255);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::new_rainbow_loop_ef(){
	hue--;
	fill_rainbow(leds, led_num, hue, step);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::random_burst_ef() {
	index = random(0, led_num);
	hue = random(0, 255);
	leds[index] = CHSV(hue, sat, 255);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::rainbow_vertical_ef(){
	hue = hue + step <= 255 ? hue + step : 0;
	index = index < led_num / 2 ? index + 1 : 0;

	leds[index] = leds[horizontal_index(index)] = CHSV(hue, sat, 255);
	LEDS.show();
	if (safeDelay(delay_time)) return;
	
}

void WS2812Effecter::continuous_vertical_rainbow_ef(){
	hue--;
	byte vert_led_cnt = led_num / 2 + led_num % 2;
	fill_rainbow(leds, vert_led_cnt, hue, step);
	for(int i = 0; i < led_num - vert_led_cnt; i++)
		leds[led_num - 1 - i] = leds[i];
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

byte WS2812Effecter::horizontal_index(byte i) {
	byte bottom_ind = 0;
	byte is_even = !(led_num % 2);
	byte top_ind = led_num / 2 - is_even;


	if (i == top_ind){
		return top_ind + is_even;
	}
	return led_num - 1 - i;
}

bool WS2812Effecter::safeDelay(int del_time) {
	uint32_t thisTime = millis();
	while (millis() - thisTime <= del_time) {
		delay(5);
	}
	return false;
}
