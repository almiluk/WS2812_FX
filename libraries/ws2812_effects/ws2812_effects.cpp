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
	case partial_rainbow: partial_rainbow_ef(); break;
	
	default:
		SetEffect(off);
		Show();
		break;
	}
}

void WS2812Effecter::SetEffect(effect_t new_effect){
	effect = new_effect;
	sat = val = 255;
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
	case partial_rainbow: step = 1; delay_time = 50; hue = ihue = 255; break;
	}
}

void WS2812Effecter::SetColorHSV(byte h, byte s, byte v){
	ihue = h;
	sat = s;
	val = v;

	one_color_all();
}

void WS2812Effecter::SetColorRGB(byte r, byte g, byte b){
	CHSV hsv = rgb2hsv_approximate(CRGB(r, g ,b));
	ihue = hsv.h;
	sat = hsv.s;
}

void WS2812Effecter::one_color_all(){
	for (int i = 0 ; i < led_num; i++ ) {
		leds[i] = CHSV(ihue, sat, val);
	}
	LEDS.show();
}

void WS2812Effecter::rainbow_fade_ef(){
	ihue = ihue != 255 ? ihue + 1 : 0;
	for (int i = 0 ; i < led_num; i++ ) {
		leds[i] = CHSV(ihue, sat, 255);
	}
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::rainbow_loop_ef(){
	ihue += step;
	index = index < led_num - 1 ? index + 1 : 0;
	leds[index] = CHSV(ihue, sat, 255);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::new_rainbow_loop_ef(){
	ihue--;
	fill_rainbow(leds, led_num, ihue, step);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::random_burst_ef() {
	index = random(0, led_num);
	ihue = random(0, 255);
	leds[index] = CHSV(ihue, sat, 255);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::rainbow_vertical_ef(){
	ihue += step;
	index = index < led_num / 2 ? index + 1 : 0;

	leds[index] = leds[horizontal_index(index)] = CHSV(ihue, sat, 255);
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::continuous_vertical_rainbow_ef(){
	ihue--;
	byte vert_led_cnt = led_num / 2 + led_num % 2;
	fill_rainbow(leds, vert_led_cnt, ihue, step);
	for(int i = 0; i < led_num - vert_led_cnt; i++)
		leds[led_num - 1 - i] = leds[i];
	LEDS.show();
	if (safeDelay(delay_time)) return;
}

void WS2812Effecter::partial_rainbow_ef(){
	ihue += step;
	if(!is_near_hsv(ihue, hue, RAINBOW_PART_SIZE))
		step = -step;
	SetColorHSV(ihue, sat, val);
	Serial.println(ihue);
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

bool WS2812Effecter::is_near_hsv(byte c1, byte c2, byte range){
	if (abs(c1 - c2) < range)
		return true;
	if (c1 + range > 255 && c2 <= c1 + range - 255)
		return true;
	if (c1 - range < 0 && c2 >= 255 - (range - c1))
		return true;
	return false;
}
