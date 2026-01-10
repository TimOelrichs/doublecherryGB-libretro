#pragma once	
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
#include <cstdint>
#include <cmath>
#include <array>
#include "../gb_core/renderer.h"
#include "libretro.h"

extern retro_environment_t environ_cb;

struct gb_audio_profile
{
	float lp;    // lowpass coefficient
	float hp;    // highpass feedback
	float clip;  // softclip strength
};

static const gb_audio_profile GB_PROFILE_DMG = {
	0.12f,   // ~8.5 kHz @ 44.1 kHz
	0.996f,  // strong DC kill
	1.0f
};

static const gb_audio_profile GB_PROFILE_GBC = {
	0.22f,   // ~14 kHz @ 44.1 kHz
	0.990f,  // light DC kill
	0.35f
};

#define OVERSAMPLE 4
#define SINC_TAPS 16
#define HQ_BUF_SIZE 4096

static const float sinc_kernel[SINC_TAPS * 2 + 1] = {
	-0.0012f, -0.0028f, -0.0049f, -0.0071f,
	-0.0087f, -0.0088f, -0.0062f,  0.0000f,
	 0.0114f,  0.0287f,  0.0516f,  0.0789f,
	 0.1084f,  0.1370f,  0.1611f,  0.1768f,
	 0.1822f,
	 0.1768f,  0.1611f,  0.1370f,  0.1084f,
	 0.0789f,  0.0516f,  0.0287f,  0.0114f,
	 0.0000f, -0.0062f, -0.0088f, -0.0087f,
	-0.0071f, -0.0049f, -0.0028f, -0.0012f
};

struct gb_hq_state
{
	float l[HQ_BUF_SIZE] = {};
	float r[HQ_BUF_SIZE] = {};
	int write = 0;
	int filled = 0;
};

static gb_hq_state hq;

static inline void hq_push(float l, float r)
{
	hq.l[hq.write] = l;
	hq.r[hq.write] = r;
	hq.write = (hq.write + 1) % HQ_BUF_SIZE;
	if (hq.filled < HQ_BUF_SIZE)
		hq.filled++;
}

static inline float hq_sinc(const float* buf, int center)
{
	float acc = 0.0f;
	for (int i = -SINC_TAPS; i <= SINC_TAPS; ++i)
	{
		int idx = (center + i + HQ_BUF_SIZE) % HQ_BUF_SIZE;
		acc += buf[idx] * sinc_kernel[i + SINC_TAPS];
	}
	return acc;
}


static inline float sinc_convolve(
	const float* hist,
	int center
)
{
	float acc = 0.0f;
	for (int i = -SINC_TAPS; i <= SINC_TAPS; ++i)
		acc += hist[center + i] * sinc_kernel[i + SINC_TAPS];
	return acc;
}




static inline int16_t gb_softclip(float x, float strength)
{
	// cubic softclip
	x -= strength * (x * x * x) * 1.0e-10f;

	if (x >  32767.f) return  32767;
	if (x < -32768.f) return -32768;
	return (int16_t)x;
}

struct gb_audio_state
{
	float l_lp = 0.0f, r_lp = 0.0f;
	float l_hp = 0.0f, r_hp = 0.0f;
	float l_prev = 0.0f, r_prev = 0.0f;
};

static gb_audio_state gb_audio;




#define clampf(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

constexpr std::array<uint32_t, 4> DMG_PALETTE = {
0xFFFFFFFF, // Weiß
0xAAAAAAFF, // Hellgrau
0x555555FF, // Dunkelgrau
0x000000FF  // Schwarz
};

constexpr int GRADIENT_STEPS = 64;
extern std::array<uint16_t, GRADIENT_STEPS> blended_palette;

enum class GhostingMode {
	RGB565_BLEND,
	PALETTE_BLEND
};


class dmy_renderer : public renderer
{
public:
	dmy_renderer(int which);
	virtual ~dmy_renderer(){};

	virtual void reset() {}
	virtual word get_sensor(bool x_y) override; // { return 0; }
	virtual void set_bibrate(bool bibrate) override
	{
		if (bibrate == last_rumble)
			return;

		last_rumble = bibrate;

		if (!environ_cb)
			return;

		retro_rumble_interface rumble;
		if (!environ_cb(RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE, &rumble))
			return;

		rumble.set_rumble_state(
			which_gb,
			RETRO_RUMBLE_STRONG,
			bibrate ? 0xFFFF : 0
		);
		rumble.set_rumble_state(
			which_gb,
			RETRO_RUMBLE_WEAK,
			bibrate ? 0xFFFF : 0
		);
	}

	virtual void render_screen(byte *buf,int width,int height,int depth);
	virtual word map_color(word gb_col);
	virtual word unmap_color(word gb_col);
	virtual void add_gbc_interlacing_effect(byte* buf, int width, int height, int depth);
	virtual int check_pad();
	virtual void refresh();
	virtual byte get_time(int type);
	virtual void set_time(int type,byte dat);

	float hue2rgb(float p, float q, float t) {
		if (t < 0.0f) t += 1.0f;
		if (t > 1.0f) t -= 1.0f;
		if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
		if (t < 1.0f / 2.0f) return q;
		if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
		return p;
	}

	void rgb_to_hsl(int r, int g, int b, float* h, float* s, float* l) {
		float rf = r / 255.0f;
		float gf = g / 255.0f;
		float bf = b / 255.0f;

		float max_val, min_val, d;

		if (rf > gf) {
			if (rf > bf)
				max_val = rf;
			else
				max_val = bf;
		}
		else {
			if (gf > bf)
				max_val = gf;
			else
				max_val = bf;
		}

		if (rf < gf) {
			if (rf < bf)
				min_val = rf;
			else
				min_val = bf;
		}
		else {
			if (gf < bf)
				min_val = gf;
			else
				min_val = bf;
		}

		*l = (max_val + min_val) * 0.5f;

		if (max_val == min_val) {
			*h = 0.0f;
			*s = 0.0f;
		}
		else {
			d = max_val - min_val;
			if (*l > 0.5f)
				*s = d / (2.0f - max_val - min_val);
			else
				*s = d / (max_val + min_val);

			if (max_val == rf)
				*h = (gf - bf) / d + (gf < bf ? 6.0f : 0.0f);
			else if (max_val == gf)
				*h = (bf - rf) / d + 2.0f;
			else
				*h = (rf - gf) / d + 4.0f;

			*h /= 6.0f;
		}
	}

	// HSL → RGB888
	void hsl_to_rgb(float h, float s, float l, int* r, int* g, int* b) {
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

		*r = (int)(clampf(r_f, 0.0f, 1.0f) * 255);
		*g = (int)(clampf(g_f, 0.0f, 1.0f) * 255);
		*b = (int)(clampf(b_f, 0.0f, 1.0f) * 255);
	}

	uint16_t brighten_rgb565_hsl(uint16_t pixel, float brighten_step) {
		int r5 = (pixel >> 11) & 0x1F;
		int g6 = (pixel >> 5) & 0x3F;
		int b5 = pixel & 0x1F;

		// Upscale to 8-bit per channel
		int r8 = (r5 << 3) | (r5 >> 2);
		int g8 = (g6 << 2) | (g6 >> 4);
		int b8 = (b5 << 3) | (b5 >> 2);

		// HSL conversion + brighten
		float h, s, l;
		rgb_to_hsl(r8, g8, b8, &h, &s, &l);
		l = clampf(l + brighten_step, 0.0f, 1.0f);
		hsl_to_rgb(h, s, l, &r8, &g8, &b8);

		// Downscale back to RGB565
		r5 = r8 >> 3;
		g6 = g8 >> 2;
		b5 = b8 >> 3;

		return (r5 << 11) | (g6 << 5) | b5;
	}
	uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b) {
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}
	void generateGradient() {
		for (int i = 0; i < GRADIENT_STEPS; ++i) {
			float t = static_cast<float>(i) / (GRADIENT_STEPS - 1);
			int lower = static_cast<int>(t * 3.0f);
			int upper = std::min(lower + 1, 3);
			float local_t = (t * 3.0f) - lower;

			uint32_t c1 = DMG_PALETTE[lower];
			uint32_t c2 = DMG_PALETTE[upper];

			uint8_t r = static_cast<uint8_t>(((1.0f - local_t) * ((c1 >> 24) & 0xFF)) + (local_t * ((c2 >> 24) & 0xFF)));
			uint8_t g = static_cast<uint8_t>(((1.0f - local_t) * ((c1 >> 16) & 0xFF)) + (local_t * ((c2 >> 16) & 0xFF)));
			uint8_t b = static_cast<uint8_t>(((1.0f - local_t) * ((c1 >> 8) & 0xFF)) + (local_t * ((c2 >> 8) & 0xFF)));

			blended_palette[i] = rgb888_to_rgb565(r, g, b);
		}
	}

	uint16_t blendPixels(uint16_t pixel1, uint16_t pixel2) {
		if (ghosting_mode == GhostingMode::RGB565_BLEND) {
			// Direktes RGB565-Mischen
			uint8_t r1 = (pixel1 >> 11) & 0x1F;
			uint8_t g1 = (pixel1 >> 5) & 0x3F;
			uint8_t b1 = pixel1 & 0x1F;

			uint8_t r2 = (pixel2 >> 11) & 0x1F;
			uint8_t g2 = (pixel2 >> 5) & 0x3F;
			uint8_t b2 = pixel2 & 0x1F;

			uint8_t r = (r1 + r2) >> 1;
			uint8_t g = (g1 + g2) >> 1;
			uint8_t b = (b1 + b2) >> 1;

			return (r << 11) | (g << 5) | b;
		}
		else {
			// Blended Palette verwenden
			// 16bit Pixel -> Grauwert -> blended_palette
			uint8_t r = ((pixel1 >> 11) & 0x1F) << 3;
			uint8_t g = ((pixel1 >> 5) & 0x3F) << 2;
			uint8_t b = (pixel1 & 0x1F) << 3;
			uint8_t grayscale1 = static_cast<uint8_t>((0.299f * r) + (0.587f * g) + (0.114f * b));

			r = ((pixel2 >> 11) & 0x1F) << 3;
			g = ((pixel2 >> 5) & 0x3F) << 2;
			b = (pixel2 & 0x1F) << 3;
			uint8_t grayscale2 = static_cast<uint8_t>((0.299f * r) + (0.587f * g) + (0.114f * b));

			int index1 = (255 - grayscale1) * (GRADIENT_STEPS - 1) / 255;
			int index2 = (255 - grayscale2) * (GRADIENT_STEPS - 1) / 255;

			int blended_index = (index1 + index2) / 2;

			return blended_palette[blended_index];
		}
	}


	dword fixed_time;
private:
	
	int cur_time;
	int which_gb;
	bool rgb565;
	byte is_odd_frame = 0;

	word last_frame[160*144];
	word current_frame[160*144];

	word last_frame_4p_slit[160 * 144 * 4];
	word current_frame_4p_split[160 * 144 * 4];

	GhostingMode ghosting_mode = GhostingMode::PALETTE_BLEND;

	bool last_rumble = false;

};
