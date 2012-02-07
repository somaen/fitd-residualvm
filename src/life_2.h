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

#ifndef _LIFE2_
#define _LIFE2_

#define NUM_MAX_SEQUENCE_PARAM 30
namespace Fitd {

struct sequenceParamStruct {
	unsigned int frame;
	unsigned int sample;
};

extern short int numObjInInventoryTable[2];
extern short int inHandTable[2];
extern short int currentInHand;

extern int numSequenceParam;

extern sequenceParamStruct sequenceParams[NUM_MAX_SEQUENCE_PARAM];

void processLife2(int lifeNum);
void convertPaletteIfRequired(unsigned char *lpalette);

extern short int numObjInInventoryTable[2];
extern short int inHandTable[2];
extern short int currentInHand;

} // end of namespace Fitd

#endif

