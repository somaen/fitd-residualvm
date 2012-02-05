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

#ifndef _TATOU_H_
#define _TATOU_H_
namespace Fitd {

int make3dTatou(void);
//////////////// to mode
	//void paletteFill(void *palette, unsigned char r, unsigned char g, unsigned b);
void fadeIn(void *palette);
void flip();
void startChrono(unsigned int *chrono);
int evalChrono(unsigned int *chrono);
void process_events(void);
void copyPalette(char *source, char *dest);
void copyToScreen(char *source, char *dest);
void makeRotationMtx(unsigned int x, unsigned int y, unsigned int z, int *xOut, int *yOut);
void make3dTatouUnk1(int var1, int var2);
void fadeOut(int var1, int var2);
void playSound(int num);
void rotateModel(int x, int y, int z, int alpha, int beta, int gamma, int time);
//////////////

} // end of namespace Fitd

#endif
