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

#ifndef FITD_ROOM_H
#define FITD_ROOM_H

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

struct cameraZonePointStruct {
	int16 x;
	int16 y;
};

struct cameraZoneEntryStruct {
	uint16 numPoints;

	cameraZonePointStruct *pointTable;
	
	void load(const char *data);
};

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
	void load(const char *data, const char *basedata);
};

struct cameraDataStruct {
	int16 _alpha;
	int16 _beta;
	int16 _gamma;

	int16 _x;
	int16 _y;
	int16 _z;

	int16 _focal1;
	int16 _focal2;
	int16 _focal3;

	uint16 _numCameraZoneDef;

	cameraZoneDefStruct *_cameraZoneDefTable;
	
	void load(const char *data);
};

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
	
	void load(const char* data);
};

extern cameraDataStruct *cameraDataTable[NUM_MAX_CAMERA_IN_ROOM];
extern cameraZoneDefStruct *currentCameraZoneList[NUM_MAX_CAMERA_IN_ROOM];
extern roomDataStruct *roomDataTable;

roomDefStruct *getRoomData(int roomNumber);
void loadRoom(int roomNumber);
int getNumberOfRoom();

} // end of namespace Fitd

#endif
