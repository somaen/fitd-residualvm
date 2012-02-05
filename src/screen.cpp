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

// seg 048

#include "osystem.h"
#include "common.h"

namespace Fitd {

void setupScreen(void) {
	screen = (char *)malloc(64800);

	screenBufferSize = 64800;

	unkScreenVar2 = 3;

	// TODO: remain of screen init

}

void flipScreen() {
	int i;
	char paletteRGBA[256*4];
	char *outPtr = scaledScreen;
	char *inPtr = unkScreenVar;
#ifdef USE_GL
	g_driver->flip(NULL);
	return;
#endif

	memcpy(unkScreenVar, screen, 320 * 200);

	for(i = 0; i < 256; i++) {
		paletteRGBA[i*4] = g_driver->_palette[i*3];
		paletteRGBA[i*4+1] = g_driver->_palette[i*3+1];
		paletteRGBA[i*4+2] = g_driver->_palette[i*3+2];
		paletteRGBA[i*4+3] = -1;
	}

	outPtr = scaledScreen;
	inPtr = unkScreenVar;

	for(i = 0; i < 200; i++) {
		int j;
		char *copySource = outPtr;

		for(j = 0; j < 320; j++) {
			*(outPtr++) = *(inPtr);
			*(outPtr++) = *(inPtr++);
		}

		// copy line
		for(j = 0; j < 640; j++) {
			*(outPtr++) = *(copySource++);
		}

	}

	//g_driver->setPalette(paletteRGBA);
	g_driver->flip((unsigned char *)scaledScreen);
}

void flushScreen(void) {
	int i;
	int j;

	for(i = 0; i < 200; i++) {
		for(j = 0; j < 320; j++) {
			*(screen + i * 320 + j) = 0;
		}
	}
}

} // end of namespace Fitd
