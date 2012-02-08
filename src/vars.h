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

#ifndef _VARS_
#define _VARS_

#include "osystem.h"
#include "common/stream.h"

namespace Fitd {

extern char *currentFoundBody;
extern int32 currentFoundBodyIdx;
extern int32 statusVar1;

struct point3dStruct {
	int16 x;
	int16 y;
	int16 z;
};

struct ZVStruct {
	int32 ZVX1;
	int32 ZVX2;
	int32 ZVY1;
	int32 ZVY2;
	int32 ZVZ1;
	int32 ZVZ2;
	void load(const char* data);
};

struct rotateStruct {
	int16 oldAngle;
	int16 newAngle;
	int16 param;
	int16 timeOfRotate;
};

struct actorFlags {
	uint16 flag0x0001: 1;
	uint16 flag0x0002: 1;
	uint16 flag0x0004: 1;
	uint16 flag0x0008: 1;
	uint16 flag0x0010: 1;
	uint16 flag0x0020: 1;
	uint16 flag0x0040: 1;
	uint16 tackable: 1;
	uint16 flag0x0100: 1;
	uint16 flag0x0200: 1;
	uint16 flag0x0400: 1;
	uint16 flag0x0800: 1;
	uint16 flag0x1000: 1;
	uint16 flag0x2000: 1;
	uint16 flag0x4000: 1;
	uint16 flag0x8000: 1;
};


struct actorStruct { // used to read data from file too
	int16 field_0;
	int16 bodyNum;
	union {
		int16 flags;
		actorFlags bitField;
	};
	int16 dynFlags;
	ZVStruct zv;
	int16 field_14;
	int16 field_16;
	int16 field_18;
	int16 field_1A;
	int16 roomX;
	int16 roomY;
	int16 roomZ;
	int16 worldX;
	int16 worldY;
	int16 worldZ;
	int16 alpha;
	int16 beta;
	int16 gamma;
	int16 stage;
	int16 room;
	int16 lifeMode;
	int16 life;
	uint32 CHRONO;
	uint32 ROOM_CHRONO;
	int16 ANIM;
	int16 field_40;
	int16 field_42;
	int16 field_44;
	int16 field_46;
	int16 field_48;
	int16 FRAME;
	int16 field_4C;
	int16 END_FRAME;
	int16 END_ANIM;
	int16 trackMode;
	int16 trackNumber;
	int16 MARK;
	int16 positionInTrack;

	int16 modX;
	int16 modY;
	int16 modZ;

	rotateStruct field_60;
	int16 falling;
	rotateStruct rotate;
	int16 field_72;
	int16 speed;
	rotateStruct speedChange;
	int16 COL[3];
	int16 COL_BY;
	int16 HARD_DEC;
	int16 HARD_COL;
	int16 HIT;
	int16 HIT_BY;
	int16 animActionType;
	int16 animActionANIM;
	int16 animActionFRAME;
	int16 animActionParam;
	int16 hitForce;
	int16 field_98;
	point3dStruct hotPoint;

	// aitd2
	int16 hardMat;
};

struct boxStruct {
	int16 var0;
	int16 var1;
	int16 var2;
	int16 var3;
};

struct hqrEntryStruct;
struct objectStruct;

extern hqrEntryStruct *hqrUnk;

extern int32 videoMode;
extern int32 musicConfigured;
extern int32 musicEnabled;

extern char *aux;
extern char *aux2;
extern char *bufferAnim;

extern char *screen;

extern int32 screenBufferSize;
extern int32 unkScreenVar2;

extern int16 *CVars;

extern char *priority;

extern char *fontData;

extern char *aitdBoxGfx;

//extern OSystem osystem;

extern char unkScreenVar[320*200];

extern char rgbaBuffer[320*200*4];

extern int32 currentMenuLeft;
extern int32 currentMenuTop;
extern int32 currentMenuRight;
extern int32 currentMenuBottom;

extern char inputKey;
extern char input1;
extern char input2;
extern char input3;
extern char input4;
extern char button;

extern char languageNameString[];

extern int32 readVar;

extern char *screenSm1;
extern char *screenSm2;
extern char *screenSm3;
extern char *screenSm4;
extern char *screenSm5;

extern actorStruct actorTable[NUM_MAX_ACTOR];

extern int16 currentCameraTarget;

extern hqrEntryStruct *listBody;
extern hqrEntryStruct *listAnim;
extern hqrEntryStruct *listLife;
extern hqrEntryStruct *listTrack;
extern hqrEntryStruct *listMatrix;

extern int16 maxObjects;

extern objectStruct *objectTable; // may be less

extern int16 *vars;

extern int32 varSize;

extern int16 currentMusic;
extern int32 action;

extern boxStruct genVar2[15]; // recheckSize
extern boxStruct genVar4[50];
extern boxStruct *genVar1;
extern boxStruct *genVar3;

extern int32 genVar5;
extern int32 genVar6;
extern int32 genVar7;
extern int32 nextMusic;
extern int16 genVar9;
extern int16 giveUp;
extern int16 inHand;
extern int16 lightVar1;
extern int32 lightVar2;
extern int16 numObjInInventory;
extern int32 soundVar1;
extern int32 soundVar2;
extern int16 statusScreenAllowed;

extern char *etageVar0;
extern char *etageVar1;

extern int32 changeFloor;
extern int16 currentCamera;
extern int16 currentEtage;
extern int32 needChangeRoom;

extern char *cameraPtr;
extern int16 currentDisplayedRoom;
extern int32 mainVar1;
extern int32 numCameraInRoom;
extern int32 numCameraZone;
extern char *cameraZoneData;
extern int32 numRoomZone;
extern char *roomZoneData;
extern char *roomVar5[NUM_MAX_CAMERA_IN_ROOM];
extern int32 startGameVar1;

extern int32 transformX;
extern int32 transformY;
extern int32 transformZ;
extern int32 transformXCos;
extern int32 transformXSin;
extern int32 transformYCos;
extern int32 transformYSin;
extern int32 transformZCos;
extern int32 transformZSin;
extern bool transformUseX;
extern bool transformUseY;
extern bool transformUseZ;

extern int32 translateX;
extern int32 translateY;
extern int32 translateZ;

extern int32 cameraCenterX;
extern int32 cameraCenterY;
extern int32 cameraX;
extern int32 cameraY;
extern int32 cameraZ;

extern char currentCameraVisibilityList[30];

extern int32 actorTurnedToObj;

extern int32 currentProcessedActorIdx;
extern actorStruct *currentProcessedActorPtr;

extern int32 currentLifeActorIdx;
extern actorStruct *currentLifeActorPtr;
extern int32 currentLifeNum;

extern char *currentLifePtr;

extern int32 setupCameraVar1;

#ifdef USE_GL
extern float renderPointList[6400];
#else
extern int16 renderPointList[6400];
#endif

extern int32 numActorInList;
extern int32 sortedActorTable[NUM_MAX_ACTOR];

extern int32 animMoveX;
extern int32 animMoveY;
extern int32 animMoveZ;
extern int32 animRot1;
extern int32 animRot2;
extern int32 animRot3;

extern int16 newFloor;

extern int32 paletteVar;

extern char cameraBuffer[256];
extern char cameraBuffer2[256];
extern char cameraBuffer3[400];
extern char cameraBuffer4[400];

extern char *cameraBufferPtr;
extern char *cameraBuffer2Ptr;
extern char *cameraBuffer3Ptr;

extern int32 overlaySize1;
extern int32 overlaySize2;

extern int32 bgOverlayVar1;

extern int16 newRoom;

extern const char *listBodySelect[];
extern const char *listAnimSelect[];

extern int16 inventory[30];

extern int16 shakeVar1;
extern int16 shakingAmplitude;
extern uint32 timerFreeze1;

extern hardColStruct *hardColTable[10];

extern int16 hardColVar1;
extern int16 hardColVar2;

extern ZVStruct hardClip;

extern int32 mainVar3;
extern int32 mainVar2;

extern int32 clipLeft;
extern int32 clipTop;
extern int32 clipRight;
extern int32 clipBottom;

#ifdef INTERNAL_DEBUGGER
enum backgroundModeEnum {
	backgroundModeEnum_2D,
	backgroundModeEnum_3D,
};

extern backgroundModeEnum backgroundMode;
#endif

} // end of namespace Fitd

#endif
