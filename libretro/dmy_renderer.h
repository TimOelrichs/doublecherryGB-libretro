/*--------------------------------------------------
   TGB Dual - Gameboy Emulator -
   Copyright (C) 2001  Hii

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <algorithm>
#include <cmath>
#include "../gb_core/renderer.h"


class dmy_renderer : public renderer
{
public:
	dmy_renderer(int which);
	virtual ~dmy_renderer(){};

	virtual void reset() {}
	virtual word get_sensor(bool x_y) { return 0; }
	virtual void set_bibrate(bool bibrate) {}

	virtual void render_screen(byte *buf,int width,int height,int depth);
	virtual word map_color(word gb_col);
	virtual word unmap_color(word gb_col);
	virtual void add_gbc_interlacing_effect(byte* buf, int width, int height, int depth);
	virtual int check_pad();
	virtual void refresh();
	virtual byte get_time(int type);
	virtual void set_time(int type,byte dat);

	void rgb_to_hsl(int r, int g, int b, float& h, float& s, float& l) {
		float rf = r / 255.0f;
		float gf = g / 255.0f;
		float bf = b / 255.0f;

		float max = std::max({ rf, gf, bf });
		float min = std::min({ rf, gf, bf });
		l = (max + min) * 0.5f;

		if (max == min) {
			h = s = 0.0f;
		}
		else {
			float d = max - min;
			s = l > 0.5f ? d / (2.0f - max - min) : d / (max + min);

			if (max == rf)
				h = (gf - bf) / d + (gf < bf ? 6.0f : 0.0f);
			else if (max == gf)
				h = (bf - rf) / d + 2.0f;
			else
				h = (rf - gf) / d + 4.0f;
			h /= 6.0f;
		}
	}

	// HSL → RGB888
	void hsl_to_rgb(float h, float s, float l, int& r, int& g, int& b) {
		auto hue2rgb = [](float p, float q, float t) -> float {
			if (t < 0.0f) t += 1.0f;
			if (t > 1.0f) t -= 1.0f;
			if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
			if (t < 1.0f / 2.0f) return q;
			if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
			return p;
			};

		float r_f, g_f, b_f;
		if (s == 0.0f) {
			r_f = g_f = b_f = l;
		}
		else {
			float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
			float p = 2.0f * l - q;
			r_f = hue2rgb(p, q, h + 1.0f / 3.0f);
			g_f = hue2rgb(p, q, h);
			b_f = hue2rgb(p, q, h - 1.0f / 3.0f);
		}

		r = int(clampf(r_f, 0.0f, 1.0f) * 255);
		g = int(clampf(g_f, 0.0f, 1.0f) * 255);
		b = int(clampf(b_f, 0.0f, 1.0f) * 255);
	}

	word brighten_rgb565_hsl(word pixel, float brighten_step = 0.05f) {
		int r5 = (pixel >> 11) & 0x1F;
		int g6 = (pixel >> 5) & 0x3F;
		int b5 = pixel & 0x1F;

		// Upscale to 8-bit per channel
		int r8 = (r5 << 3) | (r5 >> 2);
		int g8 = (g6 << 2) | (g6 >> 4);
		int b8 = (b5 << 3) | (b5 >> 2);

		// HSL conversion + brighten
		float h, s, l;
		rgb_to_hsl(r8, g8, b8, h, s, l);
		l = clampf(l + brighten_step, 0.0f, 1.0f);
		hsl_to_rgb(h, s, l, r8, g8, b8);

		// Downscale back to RGB565
		r5 = r8 >> 3;
		g6 = g8 >> 2;
		b5 = b8 >> 3;

		return (r5 << 11) | (g6 << 5) | b5;
	}

	dword fixed_time;
private:
	float clampf(float v, float min, float max) {
		return std::max(min, std::min(max, v));
	}
	int cur_time;
	int which_gb;
	bool rgb565;
	byte is_odd_frame = 0;
};
