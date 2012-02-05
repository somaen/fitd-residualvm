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
#include "fitd.h"
#include "common.h"

namespace Fitd {

void drawPartOfAITDBox(int32 left, int32 top, int32 index, char *gfxData) {
	char *outPtr;
	char *inPtr;

	int32 width;
	int32 height;

	int32 offset;

	int32 i;
	int32 j;

	if(g_fitd->getGameType() != GType_AITD1)
		return;

	outPtr = screen + top * 320 + left;
	inPtr = gfxData + READ_LE_UINT16(index * 2 + gfxData); // alignement unsafe

	inPtr += 4;

	width = READ_LE_UINT16(inPtr); // alignement unsafe
	inPtr += 2;
	height = READ_LE_UINT16(inPtr); // alignement unsafe
	inPtr += 2;

	offset = 320 - width;

	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			*(outPtr++) = *(inPtr++);
		}

		outPtr += offset;
	}
}

void drawAITDBox(int32 x, int32 y, int32 width, int32 height) {
	int32 top;
	int32 right;
	int32 left;
	int32 bottom;

	int32 currentLeftPosition;
	int32 currentTopPosition;

	int32 halfWidth;
	int32 halfHeight;

	setClipSize(0, 0, 319, 199);

	halfWidth = width / 2;
	currentLeftPosition = left = x - halfWidth;

	halfHeight = height / 2;
	currentTopPosition = top = y - halfHeight;

	right = x + halfWidth;
	bottom = y + halfHeight;

	drawPartOfAITDBox(currentLeftPosition, currentTopPosition, 0, aitdBoxGfx); // draw top left corner

	while(1) { // draw top bar
		currentLeftPosition += 20;

		if(right - 20 <= currentLeftPosition)
			break;

		drawPartOfAITDBox(currentLeftPosition, currentTopPosition, 4, aitdBoxGfx);
	}

	drawPartOfAITDBox(currentLeftPosition, currentTopPosition, 1, aitdBoxGfx); // draw top right corner

	currentLeftPosition = left;

	while(1) { // draw left bar
		currentTopPosition += 20;

		if(bottom - 20 <= currentTopPosition)
			break;

		drawPartOfAITDBox(currentLeftPosition, currentTopPosition, 6, aitdBoxGfx);
	}

	currentLeftPosition = right - 8;
	currentTopPosition = top + 20;

	while(bottom - 20 > currentTopPosition) {
		drawPartOfAITDBox(currentLeftPosition, currentTopPosition, 7, aitdBoxGfx);

		currentTopPosition += 20;
	}

	currentLeftPosition = left;

	drawPartOfAITDBox(currentLeftPosition, currentTopPosition, 2, aitdBoxGfx); // draw bottom left corner

	while(1) { // draw bottom bar
		currentLeftPosition += 20;

		if(right - 20 <= currentLeftPosition)
			break;

		drawPartOfAITDBox(currentLeftPosition, currentTopPosition + 12, 5, aitdBoxGfx);
	}

	drawPartOfAITDBox(currentLeftPosition, currentTopPosition, 3, aitdBoxGfx); // draw bottom right corner

	drawPartOfAITDBox(x - 20, currentTopPosition + 12, 8, aitdBoxGfx); // draw "in the dark"

	currentMenuLeft = left + 8;
	currentMenuTop = top + 8;
	currentMenuRight = right - 9;
	currentMenuBottom = bottom - 9;

	fillBox(currentMenuLeft, currentMenuTop, currentMenuRight, currentMenuBottom, 0);
	setClipSize(currentMenuLeft, currentMenuTop, currentMenuRight, currentMenuBottom);

}

} // end of namespace Fitd
