#ifndef _VARS_
#define _VARS_

#include "osystem.h"
#include "common/stream.h"

#pragma pack(1)
namespace Fitd {

extern char *currentFoundBody;
extern int currentFoundBodyIdx;
extern int statusVar1;

struct point3dStruct {
	int16 x;
	int16 y;
	int16 z;
};

struct textEntryStruct {
	int16 index;
	char *textPtr;
	int16 width;
};

struct messageStruct {
	textEntryStruct *string;
	int16 time;
};

struct saveEntry {
	void *ptr;
	unsigned int size;
};

struct regularTextEntryStruct {
	char *textPtr;
	int16 width;
};

struct hqrSubEntryStruct {
	int16 key;
	int16 size;
	unsigned int lastTimeUsed;
	char *ptr;
};

struct hqrEntryStruct {
	char string[10];
	uint16 maxFreeData;
	uint16 sizeFreeData;
	uint16 numMaxEntry;
	uint16 numUsedEntry;
	hqrSubEntryStruct *entries;
};

struct ZVStruct {
	int32 ZVX1;
	int32 ZVX2;
	int32 ZVY1;
	int32 ZVY2;
	int32 ZVZ1;
	int32 ZVZ2;
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
	unsigned int CHRONO;
	unsigned int ROOM_CHRONO;
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

struct boxStruct {
	int16 var0;
	int16 var1;
	int16 var2;
	int16 var3;
};

typedef struct boxStruct boxStruct;

struct roomDefStruct {
	int16 offsetToCameraDef; // 0
	int16 offsetToPosDef; // 2
	int16 worldX;//4
	int16 worldY;//6
	int16 worldZ;//8
	int16 numCameraInRoom;//0xA
};

typedef struct roomDefStruct roomDefStruct;

extern hqrEntryStruct *hqrUnk;

extern hqrEntryStruct *listMus;
extern hqrEntryStruct *listSamp;

extern int videoMode;
extern int musicConfigured;
extern int musicEnabled;

extern char *aux;
extern char *aux2;
extern char *bufferAnim;

extern char *screen;

extern int screenBufferSize;
extern int unkScreenVar2;

extern int16 *CVars;

extern char *priority;

extern char *fontData;

extern char *aitdBoxGfx;

extern char palette[0x300];

//extern OSystem osystem;

extern char unkScreenVar[320*200];

extern char rgbaBuffer[320*200*4];

extern unsigned int timer;
extern volatile unsigned int timeGlobal;

extern int currentMenuLeft;
extern int currentMenuTop;
extern int currentMenuRight;
extern int currentMenuBottom;

extern textEntryStruct *tabTextes;
extern char *systemTextes;

extern char inputKey;
extern char input1;
extern char input2;
extern char input3;
extern char input4;
extern char button;

extern char languageNameString[];
extern char *languageNameTable[];

extern regularTextEntryStruct textTable[40];

extern int readVar;

extern int hqrKeyGen;

extern char *screenSm1;
extern char *screenSm2;
extern char *screenSm3;
extern char *screenSm4;
extern char *screenSm5;

extern actorStruct actorTable[NUM_MAX_ACTOR];

extern int16 currentCameraTarget;

//extern int fileSize;

extern hqrEntryStruct *listBody;
extern hqrEntryStruct *listAnim;
extern hqrEntryStruct *listLife;
extern hqrEntryStruct *listTrack;
extern hqrEntryStruct *listMatrix;

extern int16 maxObjects;

extern objectStruct *objectTable; // may be less

extern int16 *vars;

extern int varSize;

extern messageStruct messageTable[5];

extern int16 currentMusic;
extern int action;

extern boxStruct genVar2[15]; // recheckSize
extern boxStruct genVar4[50];
extern boxStruct *genVar1;
extern boxStruct *genVar3;

extern int genVar5;
extern int genVar6;
extern int genVar7;
extern int nextMusic;
extern int16 genVar9;
extern int16 giveUp;
extern int16 inHand;
extern int16 lightVar1;
extern int lightVar2;
extern int16 numObjInInventory;
extern int soundVar1;
extern int soundVar2;
extern int16 statusScreenAllowed;

extern char *etageVar0;
extern char *etageVar1;

extern int changeFloor;
extern int16 currentCamera;
extern int16 currentEtage;
extern int needChangeRoom;

extern char *cameraPtr;
extern roomDefStruct *pCurrentRoomData;
extern int16 currentDisplayedRoom;
extern int mainVar1;
extern int numCameraInRoom;
extern int numCameraZone;
extern char *cameraZoneData;
extern int numRoomZone;
extern char *roomZoneData;
extern char *roomVar5[NUM_MAX_CAMERA_IN_ROOM];
extern int startGameVar1;

extern int transformX;
extern int transformY;
extern int transformZ;
extern int transformXCos;
extern int transformXSin;
extern int transformYCos;
extern int transformYSin;
extern int transformZCos;
extern int transformZSin;
extern bool transformUseX;
extern bool transformUseY;
extern bool transformUseZ;

extern int translateX;
extern int translateY;
extern int translateZ;

extern int cameraCenterX;
extern int cameraCenterY;
extern int cameraX;
extern int cameraY;
extern int cameraZ;

extern char currentCameraVisibilityList[30];

extern int actorTurnedToObj;

extern int currentProcessedActorIdx;
extern actorStruct *currentProcessedActorPtr;

extern int currentLifeActorIdx;
extern actorStruct *currentLifeActorPtr;
extern int currentLifeNum;

extern char *currentLifePtr;

extern int setupCameraVar1;

#ifdef USE_GL
extern float renderPointList[6400];
#else
extern int16 renderPointList[6400];
#endif

extern int numActorInList;
extern int sortedActorTable[NUM_MAX_ACTOR];

extern int angleCompX;
extern int angleCompZ;
extern int angleCompBeta;

extern int bufferAnimCounter;

extern int animCurrentTime;
extern int animKeyframeLength;
extern int animMoveX;
extern int animMoveY;
extern int animMoveZ;
extern int animRot1;
extern int animRot2;
extern int animRot3;
extern char *animVar1;
extern char *animVar3;
extern char *animVar4;

extern int16 newFloor;

extern int paletteVar;

extern char cameraBuffer[256];
extern char cameraBuffer2[256];
extern char cameraBuffer3[400];
extern char cameraBuffer4[400];

extern char *cameraBufferPtr;
extern char *cameraBuffer2Ptr;
extern char *cameraBuffer3Ptr;

extern int overlaySize1;
extern int overlaySize2;

extern int bgOverlayVar1;

extern int16 newRoom;

extern char *listBodySelect[];
extern char *listAnimSelect[];

extern int16 inventory[30];

extern int16 shakeVar1;
extern int16 shakingAmplitude;
extern unsigned int timerFreeze1;

extern hardColStruct *hardColTable[10];

extern int16 hardColVar1;
extern int16 hardColVar2;

extern ZVStruct hardClip;

extern saveEntry saveTable[];

extern int hqrVar1;
extern int mainVar3;
extern int mainVar2;

extern int clipLeft;
extern int clipTop;
extern int clipRight;
extern int clipBottom;

#ifdef INTERNAL_DEBUGGER
enum backgroundModeEnum {
	backgroundModeEnum_2D,
	backgroundModeEnum_3D,
};

extern backgroundModeEnum backgroundMode;
#endif

} // end of namespace Fitd

#endif
