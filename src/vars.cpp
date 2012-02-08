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

char *currentFoundBody;
int32 currentFoundBodyIdx;
int32 statusVar1;

hqrEntryStruct *hqrUnk;

int32 videoMode;
int32 musicConfigured;
int32 musicEnabled;

char *aux;
char *aux2;
char *bufferAnim;

char *screen;

int32 screenBufferSize;
int32 unkScreenVar2;

int16 *CVars;

char *priority;

char *fontData;

char *aitdBoxGfx;

//OSystem osystem;

char unkScreenVar[320*200];

char rgbaBuffer[320*200*4];

int32 currentMenuLeft;
int32 currentMenuTop;
int32 currentMenuRight;
int32 currentMenuBottom;

textEntryStruct *tabTextes;
char *systemTextes;

char inputKey = 0;
char input1 = 0;
char input2 = 0;
char input3;
char input4;
char button;

const char *languageNameTable[] = {
	"ENGLISH",
	"FRANCAIS",
	"ITALIANO",	
	"ESPAGNOL",
	"DEUTSCH",
	NULL,
};

char languageNameString[20] = "";

regularTextEntryStruct textTable[NUM_MAX_TEXT];

int32 readVar;

char *screenSm1;
char *screenSm2;
char *screenSm3;
char *screenSm4;
char *screenSm5;

actorStruct actorTable[NUM_MAX_ACTOR];

int16 currentCameraTarget;

int32 fileSize;

hqrEntryStruct *listBody;
hqrEntryStruct *listAnim;
hqrEntryStruct *listLife;
hqrEntryStruct *listTrack;
hqrEntryStruct *listMatrix;

int16 maxObjects;

objectStruct *objectTable;

int16 *vars;

int32 varSize;

messageStruct messageTable[NUM_MAX_MESSAGE];

int16 currentMusic;
int32 action;

boxStruct genVar2[15]; // recheckSize
boxStruct genVar4[50];
boxStruct *genVar1;
boxStruct *genVar3;

int32 genVar5;
int32 genVar6;
int32 genVar7;
int nextMusic;
int16 genVar9;
int16 giveUp;
int16 inHand;
int16 lightVar1;
int32 lightVar2;
int16 numObjInInventory;
int32 soundVar1;
int32 soundVar2;
int16 statusScreenAllowed;

char *etageVar0 = NULL;
char *etageVar1 = NULL;

int32 changeFloor;
int16 currentCamera;
int16 currentEtage;
int32 needChangeRoom;

char *cameraPtr;

int16 currentDisplayedRoom;
int32 mainVar1;
int32 numCameraInRoom;
int32 numCameraZone;
char *cameraZoneData;
int32 numRoomZone;
char *roomZoneData;
char *roomVar5[NUM_MAX_CAMERA_IN_ROOM];
int32 startGameVar1;

int32 transformX;
int32 transformY;
int32 transformZ;
int32 transformXCos;
int32 transformXSin;
int32 transformYCos;
int32 transformYSin;
int32 transformZCos;
int32 transformZSin;
bool transformUseX;
bool transformUseY;
bool transformUseZ;

int32 translateX;
int32 translateY;
int32 translateZ;

int32 cameraCenterX;
int32 cameraCenterY;
int32 cameraX;
int32 cameraY;
int32 cameraZ;

char currentCameraVisibilityList[30];

int32 actorTurnedToObj = 0;

int32 currentProcessedActorIdx;
actorStruct *currentProcessedActorPtr;

int32 currentLifeActorIdx;
actorStruct *currentLifeActorPtr;
int32 currentLifeNum;

char *currentLifePtr;

int32 setupCameraVar1;

#ifdef USE_GL
float renderPointList[6400];
#else
int16 renderPointList[6400];
#endif

int32 numActorInList;
int32 sortedActorTable[NUM_MAX_ACTOR];

int32 angleCompX;
int32 angleCompZ;
int32 angleCompBeta;

int32 bufferAnimCounter = 0;

int32 animMoveX;
int32 animMoveY;
int32 animMoveZ;
int32 animRot1;
int32 animRot2;
int32 animRot3;

int16 newFloor;

int32 paletteVar;

char cameraBuffer[256];
char cameraBuffer2[256];
char cameraBuffer3[400];
char cameraBuffer4[400];

char *cameraBufferPtr = cameraBuffer;
char *cameraBuffer2Ptr = cameraBuffer2;
char *cameraBuffer3Ptr = cameraBuffer3;

int32 overlaySize1;
int32 overlaySize2;

int32 bgOverlayVar1;

int16 newRoom;

int16 inventory[INVENTORY_SIZE];

int16 shakeVar1;
int16 shakingAmplitude;
uint32 timerFreeze1;

hardColStruct *hardColTable[10];

int16 hardColVar1;
int16 hardColVar2;

ZVStruct hardClip;

const char *listBodySelect[] = {
	"LISTBODY",
	"LISTBOD2",
};

const char *listAnimSelect[] = {
	"LISTANIM",
	"LISTANI2",
};

saveEntry saveTable[40];

int32 mainVar3 = 4000;
int32 mainVar2 = -2000;

int32 clipLeft = 0;
int32 clipTop = 0;
int32 clipRight = 319;
int32 clipBottom = 119;

#ifdef INTERNAL_DEBUGGER
backgroundModeEnum backgroundMode = backgroundModeEnum_2D;
#endif

void objectStruct::readFromStream(Common::SeekableReadStream *stream) {
	this->ownerIdx = stream->readUint16LE();
	this->body = stream->readUint16LE();
	this->flags = stream->readUint16LE();
	this->field_6 = stream->readUint16LE();
	this->foundBody = stream->readUint16LE();
	this->foundName = stream->readUint16LE();
	this->flags2 = stream->readUint16LE();
	this->foundLife = stream->readUint16LE();
	this->x = stream->readUint16LE();
	this->y = stream->readUint16LE();
	this->z = stream->readUint16LE();
	this->alpha = stream->readUint16LE();
	this->beta = stream->readUint16LE();
	this->gamma = stream->readUint16LE();
	this->stage = stream->readUint16LE();
	this->room = stream->readUint16LE();
	this->lifeMode = stream->readUint16LE();
	this->life = stream->readUint16LE();	
	this->field_24 = stream->readUint16LE();
	this->anim = stream->readUint16LE();
	this->frame = stream->readUint16LE();
	this->animType = stream->readUint16LE();
	this->animInfo = stream->readUint16LE();
	this->trackMode = stream->readUint16LE();
	this->trackNumber = stream->readUint16LE();
	this->positionInTrack = stream->readUint16LE();

	if(g_fitd->getGameType() >= GType_JACK) {
		this->mark = stream->readUint16LE();
	}
	this->flags |= 0x20;
}

} // end of namespace Fitd
