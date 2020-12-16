#include "gae_colour.h"

/* pinched from stackoverflow.com/questions/3018313 */
gae_colour_rgba gae_colour_hsv_to_rgb(gae_colour_hsv in)
{
	float hh, p, q, t, ff;
	long i;
	gae_colour_rgba out;
    
    out.r = 255; out.g = 255; out.b = 255; out.a = 255;
	
	if (0 == in.s) {
		out.r = (gae_byte)(in.v * 255);
		out.g = (gae_byte)(in.v * 255);
		out.b = (gae_byte)(in.v * 255);

		return out;
	}
	
	hh = in.h;
	if (360 <= hh) hh = 0;
	hh /= 60;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0F - in.s);
	q = in.v * (1.0F - (in.s * ff));
	t = in.v * (1.0F - (in.s * (1.0F - ff)));
	
	switch (i) {
		case 0:
			out.r = (gae_byte)(in.v * 255);
			out.g = (gae_byte)(t * 255);
			out.b = (gae_byte)(p * 255);
		break;
		case 1:
			out.r = (gae_byte)(q * 255);
			out.g = (gae_byte)(in.v * 255);
			out.b = (gae_byte)(p * 255);
		break;
		case 2:
			out.r = (gae_byte)(p * 255);
			out.g = (gae_byte)(in.v * 255);
			out.b = (gae_byte)(t * 255);
		break;
		case 3:
			out.r = (gae_byte)(p * 255);
			out.g = (gae_byte)(q * 255);
			out.b = (gae_byte)(in.v * 255);
		break;
		case 4:
			out.r = (gae_byte)(t * 255);
			out.g = (gae_byte)(p * 255);
			out.b = (gae_byte)(in.v * 255);
		break;
		case 5:
			out.r = (gae_byte)(in.v * 255);
			out.g = (gae_byte)(p * 255);
			out.b = (gae_byte)(q * 255);
		break;
	}
	
	return out;
}

/* pinched from stackoverflow.com/questions/3018313 */
gae_colour_hsv gae_colour_rgb_to_hsv(gae_colour_rgba in)
{
	gae_colour_hsv out;
	float min, max, delta;
	float r = in.r / 255.0F;
	float g = in.g / 255.0F;
	float b = in.b / 255.0F;

	min = r < g ? r : g;
	min = min < b ? min : b;

	max = r > g ? r : g;
	max = max > b ? max : b;

	out.v = max;
	delta = max - min;
	if (0.00001 > delta) {
		out.s = 0;
		out.h = 0;
		return out;
	}

	if (0 == max)
		out.s = delta / max;
	else {
		/* if max is 0, then r = g = b = 0 */
		out.s = 0;
		out.h = 0;
		return out;
	}

	if (in.r == max)
		out.h = (in.g - in.b) / delta;
	else if (in.g == max)
		out.h = 2.0F + (in.b - in.r) / delta;
	else
		out.h = 4.0F + (in.r - in.g) / delta;

	out.h *= 60;

	if (0 > out.h)
		out.h += 360;
	
	return out;
}
