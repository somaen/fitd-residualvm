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

#ifndef _OBJECT_H_
#define _OBJECT_H_
namespace Fitd {

struct objectStruct {
	int16 ownerIdx;
	int16 body;
	union {
		int16 flags;
		actorFlags bitField;
	};
	int16 field_6;
	int16 foundBody;
	int16 foundName;
	int16 flags2;
	int16 foundLife;
	int16 x;
	int16 y;
	int16 z;
	int16 alpha;
	int16 beta;
	int16 gamma;
	int16 stage;
	int16 room;
	int16 lifeMode;
	int16 life;
	int16 field_24;
	int16 anim;
	int16 frame;
	int16 animType;
	int16 animInfo;
	int16 trackMode;
	int16 trackNumber;
	int16 positionInTrack;

	// AITD2
	int16 mark;
	void readFromStream(Common::SeekableReadStream *stream);
};

int copyObjectToActor(int flag2, int var1, int foundName, short int flag, int x, int y, int z, int stage, int room, int alpha, int beta, int gamma, int var2, int var3, int var4, int var5);

} // end of namespace Fitd

#endif
