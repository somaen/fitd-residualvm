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

#ifndef _ROOM_H_
#define _ROOM_H_

namespace Fitd {
struct hardColStruct;
}
#include "vars.h" // temporary fix to cross include

namespace Fitd {

struct hardColStruct {
	ZVStruct zv;
	uint32 type;
	uint32 parameter;
};

struct sceZoneStruct {
	ZVStruct zv;
	uint32 type;
	uint32 parameter;
};

typedef struct sceZoneStruct sceZoneStruct;

struct cameraZonePointStruct {
	int16 x;
	int16 y;
};

typedef struct cameraZonePointStruct cameraZonePointStruct;

struct cameraZoneEntryStruct {
	uint16 numPoints;

	cameraZonePointStruct *pointTable;
};

typedef struct cameraZoneEntryStruct cameraZoneEntryStruct;

struct cameraZoneDefStruct {
	int16 dummy1;
	int16 dummy2;
	int16 dummy3;
	int16 dummy4;
	int16 dummy5;
	int16 dummy6;
	int16 dummy7;
	int16 dummy8;

	uint16 numZones;
	cameraZoneEntryStruct *cameraZoneEntryTable;
};

typedef struct cameraZoneDefStruct cameraZoneDefStruct;

struct cameraDataStruct {
	int16 alpha;
	int16 beta;
	int16 gamma;

	int16 x;
	int16 y;
	int16 z;

	int16 focal1;
	int16 focal2;
	int16 focal3;

	uint16 numCameraZoneDef;

	cameraZoneDefStruct *cameraZoneDefTable;
};
typedef struct cameraDataStruct cameraDataStruct;

struct roomDataStruct {
	uint32 numCameraInRoom;

	uint32 numHardCol;
	hardColStruct *hardColTable;

	uint32 numSceZone;
	sceZoneStruct *sceZoneTable;

	int32 worldX;
	int32 worldY;
	int32 worldZ;

	uint16 *cameraIdxTable;
};
typedef struct roomDataStruct roomDataStruct;

extern cameraDataStruct *cameraDataTable[NUM_MAX_CAMERA_IN_ROOM];
extern cameraZoneDefStruct *currentCameraZoneList[NUM_MAX_CAMERA_IN_ROOM];
extern roomDataStruct *roomDataTable;

roomDefStruct *getRoomData(int roomNumber);
void loadRoom(int roomNumber);
int getNumberOfRoom();

} // end of namespace Fitd

#endif
