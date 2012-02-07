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
#include "resource.h"
#include "common.h"

namespace Fitd {

unsigned long int etageVar0Size = 0;
unsigned long int numGlobalCamera = 0;
cameraDataStruct *globalCameraDataTable = NULL;

void cameraDataStruct::load(const char *data) {
	_alpha = READ_LE_UINT16(data + 0x00);
	_beta  = READ_LE_UINT16(data + 0x02);
	_gamma = READ_LE_UINT16(data + 0x04);
	
	_x = READ_LE_UINT16(data + 0x06);
	_y = READ_LE_UINT16(data + 0x08);
	_z = READ_LE_UINT16(data + 0x0A);
	
	_focal1 = READ_LE_UINT16(data + 0x0C);
	_focal2 = READ_LE_UINT16(data + 0x0E);
	_focal3 = READ_LE_UINT16(data + 0x10);
	
	_numCameraZoneDef = READ_LE_UINT16(data + 0x12);
	
	_cameraZoneDefTable = new cameraZoneDefStruct[_numCameraZoneDef];

	const char *backupDataPtr = data;
	data += 0x14;
	
	for(int k = 0; k < _numCameraZoneDef; k++) {
		_cameraZoneDefTable[k].load(data, backupDataPtr);
		
		if(g_fitd->getGameType() == GType_AITD1)
			data += 0x0C;
		else
			data += 0x10;
		
		if(g_fitd->getGameType() == GType_TIMEGATE) {
			data += 4;
		}
	}
}
	
void cameraZoneDefStruct::load(const char *data, const char *basedata) {
	dummy1 = READ_LE_UINT16(data + 0x00);
	dummy2 = READ_LE_UINT16(data + 0x02);
	dummy3 = READ_LE_UINT16(data + 0x04);
	dummy4 = READ_LE_UINT16(data + 0x06);
	dummy5 = READ_LE_UINT16(data + 0x08);
	dummy6 = READ_LE_UINT16(data + 0x0A);
	
	if(g_fitd->getGameType() != GType_AITD1) {
		dummy7 = READ_LE_UINT16(data + 0x0C);
		dummy8 = READ_LE_UINT16(data + 0x0E);
	}
	
	// load camera zone
	{
		const char *pZoneData;
		
		pZoneData = basedata + dummy3;
		//pZoneData = currentCameraData;
		
		numZones = numZones = READ_LE_UINT16(pZoneData);
		pZoneData += 2;
		
		cameraZoneEntryTable = (cameraZoneEntryStruct *)malloc(sizeof(cameraZoneEntryStruct) * numZones);
		
		ASSERT(cameraZoneEntryTable);
		
		for(int j = 0; j < numZones; j++) {
			cameraZoneEntryTable[j].load(pZoneData);
			pZoneData += 2 + (4 * cameraZoneEntryTable[j].numPoints);
		}
	}
}

void cameraZoneEntryStruct::load(const char *data) {
	int32 numOfPoints;
	
	numPoints = numOfPoints = READ_LE_UINT16(data);
	data += 2;
	
	pointTable = new cameraZonePointStruct[numOfPoints + 1];
	
	for(int32 pointIdx = 0; pointIdx < numPoints; pointIdx++) {
		pointTable[pointIdx].x = READ_LE_UINT16(data);
		data += 2;
		pointTable[pointIdx].y = READ_LE_UINT16(data);
		data += 2;
	}
	
	pointTable[numOfPoints].x = pointTable[0].x; // copy first point to last position
	pointTable[numOfPoints].y = pointTable[0].y;
}

void ZVStruct::load(const char *data) {
	ZVX1 = (int16)READ_LE_UINT16(data + 0x00);
	ZVX2 = (int16)READ_LE_UINT16(data + 0x02);
	ZVY1 = (int16)READ_LE_UINT16(data + 0x04);
	ZVY2 = (int16)READ_LE_UINT16(data + 0x06);
	ZVZ1 = (int16)READ_LE_UINT16(data + 0x08);
	ZVZ2 = (int16)READ_LE_UINT16(data + 0x0A);
}

void roomDataStruct::load(const char *data) {
	worldX = (int16)READ_LE_UINT16(data + 4);
	worldY = (int16)READ_LE_UINT16(data + 6);
	worldZ = (int16)READ_LE_UINT16(data + 8);
	
	numCameraInRoom = READ_LE_UINT16(data + 0xA);
	
	cameraIdxTable = new uint16[numCameraInRoom];
	
	for(uint32 j = 0; j < numCameraInRoom; j++) {
		cameraIdxTable[j] = READ_LE_UINT16(data + 0xC + 2 * j);
	}
	
	// hard col read
	
	const char *hardColData = data + READ_LE_UINT16(data);
	numHardCol = READ_LE_UINT16(hardColData);
	hardColData += 2;
	
	if(numHardCol) {
		hardColTable = new hardColStruct[numHardCol];
		
		for(uint32 j = 0; j < numHardCol; j++) {
			hardColTable[j].zv.load(hardColData);

			hardColTable[j].parameter = READ_LE_UINT16(hardColData + 0x0C);
			hardColTable[j].type = READ_LE_UINT16(hardColData + 0x0E);

			hardColData += 0x10;
		}
	} else {
		hardColTable = NULL;
	}
	
	// sce zone read
	
	const char *sceZoneData = data + READ_LE_UINT16(data + 2);
	numSceZone = READ_LE_UINT16(sceZoneData);
	sceZoneData += 2;
	
	if(numSceZone) {
		sceZoneTable = new sceZoneStruct[numSceZone];
		
		for(uint32 j = 0; j < numSceZone; j++) {
			sceZoneTable[j].zv.load(sceZoneData);
			
			sceZoneTable[j].parameter = READ_LE_UINT16(sceZoneData + 0x0C);
			sceZoneTable[j].type = READ_LE_UINT16(sceZoneData + 0x0E);
			
			sceZoneData += 0x10;
		}
	} else {
		sceZoneTable = NULL;
	}
}

void loadFloor(int32 floorNumber) {
	int32 i;
	int32 expectedNumberOfRoom;
	int32 expectedNumberOfCamera;
	uint32 cameraDataSize;
	char buffer[256];

	if(etageVar1) {
		free(etageVar1);
		free(etageVar0);
	}

	//stopSounds();

	listBody->reset();
	listAnim->reset();

	currentEtage = floorNumber;

	if(g_fitd->getGameType() < GType_AITD3) {
		sprintf(buffer, "ETAGE%02d", floorNumber);

		etageVar0Size = getPakSize(buffer, 0);
		cameraDataSize = getPakSize(buffer, 1);

		etageVar0 = g_resourceLoader->loadPakSafe(buffer, 0);
		etageVar1 = g_resourceLoader->loadPakSafe(buffer, 1);
	}

	currentCamera = -1;
	needChangeRoom = 1;
	changeFloor = 0;

	//////////////////////////////////

	if(roomDataTable) {
		free(roomDataTable);
		roomDataTable = NULL;
	}

	expectedNumberOfRoom = getNumberOfRoom();

	for(i = 0; i < expectedNumberOfRoom; i++) {
		char *roomData;

		roomDataStruct *currentRoomDataPtr;

		if(roomDataTable) {
			roomDataTable = (roomDataStruct *)realloc(roomDataTable, sizeof(roomDataStruct) * (i + 1));
		} else {
			roomDataTable = (roomDataStruct *)malloc(sizeof(roomDataStruct));
		}

		if(g_fitd->getGameType() >= GType_AITD3) {
			char buffer[256];

			if(g_fitd->getGameType() == GType_AITD3) {
				sprintf(buffer, "SAL%02d", floorNumber);
			} else {
				sprintf(buffer, "ETAGE%02d", floorNumber);
			}

			roomData = g_resourceLoader->loadPakSafe(buffer, i);
		} else {
			roomData = (etageVar0 + READ_LE_UINT32(etageVar0 + i * 4));
		}
		currentRoomDataPtr = &roomDataTable[i];
		
		currentRoomDataPtr->load(roomData);
	}
	///////////////////////////////////

	/////////////////////////////////////////////////
	// camera stuff

	if(g_fitd->getGameType() >= GType_AITD3) {
		char buffer[256];

		if(g_fitd->getGameType() == GType_AITD3) {
			sprintf(buffer, "CAM%02d", floorNumber);
		} else {
			sprintf(buffer, "CAMSAL%02d", floorNumber);
		}

		expectedNumberOfCamera = PAK_getNumFiles(buffer);
	} else {
		expectedNumberOfCamera = ((READ_LE_UINT32(etageVar1)) / 4);
	}

	//globalCameraDataTable = (cameraDataStruct *)malloc(sizeof(cameraDataStruct) * expectedNumberOfCamera);
	globalCameraDataTable = new cameraDataStruct[expectedNumberOfCamera];
	
	for(int i = 0; i < expectedNumberOfCamera; i++) {
		uint32 offset;
		char *currentCameraData;

		if(g_fitd->getGameType() >= GType_AITD3) {
			char buffer[256];

			if(g_fitd->getGameType() == GType_AITD3) {
				sprintf(buffer, "CAM%02d", floorNumber);
			} else {
				sprintf(buffer, "CAMSAL%02d", floorNumber);
			}

			offset = 0;
			cameraDataSize = 1;
			currentCameraData = g_resourceLoader->loadPakSafe(buffer, i);
		} else {
			offset = READ_LE_UINT32(etageVar1 + i * 4);
		}

		// load cameras
		if(offset < cameraDataSize) {
			if(g_fitd->getGameType() < GType_AITD3) {
				currentCameraData = (etageVar1 + READ_LE_UINT32(etageVar1 + i * 4));
			}

			globalCameraDataTable[i].load(currentCameraData);

			ASSERT(globalCameraDataTable[i]._cameraZoneDefTable);

		} else {
			break;
		}
		
		numGlobalCamera++;
	}

	numGlobalCamera--;

	// globalCameraDataTable = (cameraDataStruct*)realloc(globalCameraDataTable,sizeof(cameraDataStruct)*numGlobalCamera);

	/*    roomCameraData+=0x14;

	 }*/
}

} // end of namespace Fitd
