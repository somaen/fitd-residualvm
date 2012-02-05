/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "palette.h"
#include "fitd.h"

namespace Fitd {

void Palette::loadDefault() {
	const unsigned char defaultPalette[0x30] = {
		0x00,	0x00,	0x00,	0x3F,
		0x3F,	0x3F,	0x0C,	0x0C,
		0x0E,	0x30,	0x2F,	0x3F,
		0x23,	0x2C,	0x23,	0x2A,
		0x1D,	0x2A,	0x2A,	0x21,
		0x18,	0x3F,	0x05,	0x2A,
		0x12,	0x14,	0x18,	0x31,
		0x15,	0x17,	0x15,	0x25,
		0x15,	0x15,	0x2F,	0x3F,
		0x3F,	0x22,	0x15,	0x2B,
		0x15,	0x3F,	0x3F,	0x3F,
		0x21,	0x3F,	0x3F,	0x3F
	};
	memcpy(_data, defaultPalette, 0x30);
	//copyFrom(defaultPalette);
}

void Palette::loadDefaultAITD3() {
	const unsigned char defaultPaletteAITD3[0x30] = {
		0x00,	0x00,	0x00,	0xFC,	
		0xFC,	0xFC,	0x30,	0x30,	
		0x38,	0xC0,	0xBC,	0xFC,	
		0x78,	0x58,	0x3C,	0x00,	
		0x00,	0x00,	0xF0,	0x70,
		0x10,	0xFC,	0xFC,	0xFC,
		0x48,	0x50,	0x60,	0xC4,
		0x54,	0x5C,	0x54,	0x94,
		0x54,	0x54,	0xBC,	0xFC,
		0xFC,	0x88,	0x54,	0xAC,
		0x54,	0xFC,	0xFC,	0xFC,	
		0xFC,	0xFC,	0xFC,	0xF8
	};
	//copyFrom(defaultPaletteAITD3);
	memcpy(_data, defaultPaletteAITD3, 0x30);
}

Palette::Palette() {
	_data = (char*)new unsigned char[0x30];
}

Palette::Palette(char *pal) {
	_data = pal;
}

Palette::~Palette() {
	delete[] _data;
}
void Palette::convertIfRequired() {
	if(g_fitd->getGameType() >= GType_JACK && g_fitd->getGameType() < GType_AITD3) {
		unsigned char *ptr2 = (unsigned char*)_data;
		for(int i = 0; i < 256; i++) {
			for(int j = 0; j < 3; j++) {
				unsigned int composante = *(ptr2);
				composante *= 255;
				composante /= 63;
				*(ptr2++) = composante & 0xFF;
			}
		}
	}
}
	
void Palette::fill(unsigned char r, unsigned char g, unsigned b) {
	unsigned char *paletteLocal = (unsigned char *) _data;
	int offset = 0;
	
	r <<= 1;
	g <<= 1;
	b <<= 1;
	
	for(int i = 0; i < 256; i++) {
		paletteLocal[offset] = r;
		paletteLocal[offset+1] = g;
		paletteLocal[offset+2] = b;
		offset += 3;
	}
}

void Palette::copyFrom(const char *source) {
	copyFrom((unsigned char*) source);
}
	
void Palette::copyFrom(const unsigned char *source) {
	for(int i = 0; i < 768; i++) {
		_data[i] = source[i];
	}
}

void Palette::copyFrom(Palette *source) {
	for(int i = 0; i < 768; i++) {
		_data[i] = source->_data[i];
	}
}

const unsigned char *Palette::getData() {
	return (unsigned char*)_data;
}
	
} // end of namespace Fitd
