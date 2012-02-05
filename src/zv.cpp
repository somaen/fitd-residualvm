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

#include "common.h"

namespace Fitd {

void getZvCube(char *bodyPtr, ZVStruct *zvPtr) {
	short int *ptr;

	ptr = (short int *)(bodyPtr + 2);

	zvPtr->ZVX1 = *(ptr++);
	zvPtr->ZVX2 = *(ptr++);
	zvPtr->ZVY1 = *(ptr++);
	zvPtr->ZVY2 = *(ptr++);
	zvPtr->ZVZ1 = *(ptr++);
	zvPtr->ZVZ2 = *(ptr++);

	zvPtr->ZVZ2 = zvPtr->ZVX2 = (zvPtr->ZVX2 + zvPtr->ZVZ2) / 2;
	zvPtr->ZVX1 = zvPtr->ZVZ1 = -zvPtr->ZVZ2;
}

void getZvNormal(char *bodyPtr, ZVStruct *zvPtr) {
	short int *ptr;

	ptr = (short int *)(bodyPtr + 2);

	zvPtr->ZVX1 = *(ptr++);
	zvPtr->ZVX2 = *(ptr++);
	zvPtr->ZVY1 = *(ptr++);
	zvPtr->ZVY2 = *(ptr++);
	zvPtr->ZVZ1 = *(ptr++);
	zvPtr->ZVZ2 = *(ptr++);
}

void makeDefaultZV(ZVStruct *zvPtr) {
	zvPtr->ZVX1 = -100;
	zvPtr->ZVX2 = 100;

	zvPtr->ZVY1 = -2000;
	zvPtr->ZVY2 = 0;

	zvPtr->ZVZ1 = -100;
	zvPtr->ZVZ2 = 100;
}

void getZvMax(char *bodyPtr, ZVStruct *zvPtr) {
	int x1;
	int x2;
	int z1;
	int z2;

	getZvNormal(bodyPtr, zvPtr);

	x1 = zvPtr->ZVX1;
	x2 = zvPtr->ZVX2;

	z1 = zvPtr->ZVZ1;
	z2 = zvPtr->ZVZ2;

	x2 = - x1 + x2;
	z2 = - z1 + z2;

	if(x2 < z2) {
		x2 = z2;
	}

	x2 /= 2;

	zvPtr->ZVX1 = -x2;
	zvPtr->ZVX2 = x2;

	zvPtr->ZVZ1 = -x2;
	zvPtr->ZVZ2 = x2;
}

} // end of namespace Fitd
