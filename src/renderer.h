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

#ifndef _RENDERER_H_
#define _RENDERER_H_
namespace Fitd {

extern int BBox3D1;
extern int BBox3D2;
extern int BBox3D3;
extern int BBox3D4;

#define NUM_MAX_POINT_IN_POINT_BUFFER 400
#define NUM_MAX_BONES 40

extern short int pointBuffer[NUM_MAX_POINT_IN_POINT_BUFFER*3];
extern int numOfPoints;

#ifdef USE_GL
void transformPoint(float *ax, float *bx, float *cx);
#else
void transformPoint(int *ax, int *bx, int *cx);
#endif

int renderModel(int var0, int var1, int var2, int var3, int var4, int var5, void *modelPtr);

void computeScreenBox(int x, int y, int z, int alpha, int beta, int gamma, char *bodyPtr);

} // end of namespace Fitd

#endif
