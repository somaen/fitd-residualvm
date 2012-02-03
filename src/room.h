#ifndef _ROOM_H_
#define _ROOM_H_

namespace Fitd {
struct hardColStruct;
}
#include "vars.h" // temporary fix to cross include

namespace Fitd {

struct hardColStruct
{
  ZVStruct zv;
  uint32 type;
  uint32 parameter;
};

struct sceZoneStruct
{
  ZVStruct zv;
  uint32 type;
  uint32 parameter;
};

typedef struct sceZoneStruct sceZoneStruct;

struct cameraZonePointStruct
{
  int16 x;
  int16 y;
};

typedef struct cameraZonePointStruct cameraZonePointStruct;

struct cameraZoneEntryStruct
{
  uint16 numPoints;

  cameraZonePointStruct* pointTable;
};

typedef struct cameraZoneEntryStruct cameraZoneEntryStruct;

struct cameraZoneDefStruct
{
  int16 dummy1;
  int16 dummy2;
  int16 dummy3;
  int16 dummy4;
  int16 dummy5;
  int16 dummy6;
  int16 dummy7;
  int16 dummy8;

  uint16 numZones;
  cameraZoneEntryStruct* cameraZoneEntryTable;
};

typedef struct cameraZoneDefStruct cameraZoneDefStruct;

struct cameraDataStruct
{
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

  cameraZoneDefStruct* cameraZoneDefTable;
};
typedef struct cameraDataStruct cameraDataStruct;

struct roomDataStruct
{
  uint32 numCameraInRoom;

  uint32 numHardCol;
  hardColStruct* hardColTable;

  uint32 numSceZone;
  sceZoneStruct* sceZoneTable;

  int32 worldX;
  int32 worldY;
  int32 worldZ;

  uint16* cameraIdxTable;
};
typedef struct roomDataStruct roomDataStruct;

extern cameraDataStruct* cameraDataTable[NUM_MAX_CAMERA_IN_ROOM];
extern cameraZoneDefStruct* currentCameraZoneList[NUM_MAX_CAMERA_IN_ROOM];
extern roomDataStruct* roomDataTable;

roomDefStruct* getRoomData(int roomNumber);
void loadRoom(int roomNumber);
int getNumberOfRoom();

} // end of namespace Fitd

#endif
