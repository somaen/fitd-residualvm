#ifndef FITD_GFXSYSTEM_H
#define FITD_GFXSYSTEM_H


#include <cstdio>
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

#include <cstdlib>
#include <cstring>
// TODO: Remember to clean this out before merging
#ifndef UNIX
#define UNIX
#endif

#ifdef HAVE_CONFIG_H
#undef HAVE_CONFIG_H
#include "common/endian.h"
#define HAVE_CONFIG_H
#else
#include "common/endian.h"
#endif
#ifdef INTERNAL_DEBUGGER
#include "debug_font.h"
#endif

#ifndef USE_GL
#define USE_GL
#endif

#include "palette.h"

namespace Fitd {

class GFXSystem {
public:
	GFXSystem();
	void init();

	int _mouseRight;
	int _mouseLeft;
	
	char *_palette;
	Palette *_paletteObj;

	void delay(int time);
	void crossFade(char *buffer, char *palette);
	void fadeBlackToWhite();
	void updateImage();
	void initBuffer(char *buffer, int width, int height);
	void initVideoBuffer(char *buffer, int width, int height);
	void putpixel(int x, int y, int pixel);
	void setColor(byte i, byte R, byte G, byte B);
	void setPalette(byte *palette);
	void setPalette320x200(byte *palette);
	void flip(unsigned char *videoBuffer);
	void draw320x200BufferToScreen(unsigned char *videoBuffer);
	void CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom);
	void drawText(int X, int Y, char *text);
	void drawTextColor(int X, int Y, char *string, unsigned char R, unsigned char G, unsigned char B);
	void drawLine(int X1, int X2, int Y1, int Y2, unsigned char color, unsigned char *palette);
	void getPalette(char *palette);
	void playSampleFromName(char *sampleName);
	void playSample(char *samplePtr, int size);
	//    void getMouseStatus(mouseStatusStruct * mouseData);

	void set320x200Mode(bool mode);

	void startFrame();
	void stopFrame();
	void startModelRender();
	void stopModelRender();
#ifdef USE_GL
	void fillPoly(float *buffer, int numPoint, unsigned char color, uint8 polyType);
	void draw3dLine(float x1, float y1, float z1, float x2, float y2, float z2, unsigned char color);
	void draw3dQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, unsigned char color, int transparency);
	void cleanScreenKeepZBuffer();
	void drawSphere(float X, float Y, float Z, uint8 color, float size);

	void startBgPoly();
	void endBgPoly();
	void addBgPolyPoint(int x, int y);
#else
	void fillPoly(short int *buffer, int numPoint, unsigned char color);
	void draw3dLine(int x1, int y1, int z1, int x2, int y2, int z2, unsigned char color);
	void draw3dQuad(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4, unsigned char color);
#endif

#ifdef INTERNAL_DEBUGGER
	void drawDebugText(const uint32 X, const uint32 Y, const uint8 *string);
#endif

private:
};

extern GFXSystem *g_driver;

} // end of namespace Fitd

#endif
