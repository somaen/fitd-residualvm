/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.

 <TODO: Add in GPLv2-notice, need to make sure if we are v2-only, or v2-or-later,
 we are atleast v2>

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
int currentFoundBodyIdx;
int statusVar1;

hqrEntryStruct *hqrUnk;

int videoMode;
int musicConfigured;
int musicEnabled;

char *aux;
char *aux2;
char *bufferAnim;

char *screen;

int screenBufferSize;
int unkScreenVar2;

int16 *CVars;

char *priority;

char *fontData;

char *aitdBoxGfx;

char palette[256*3];

//OSystem osystem;

char unkScreenVar[320*200];

char rgbaBuffer[320*200*4];

unsigned int timer;
volatile unsigned int timeGlobal;

int currentMenuLeft;
int currentMenuTop;
int currentMenuRight;
int currentMenuBottom;

textEntryStruct *tabTextes;
char *systemTextes;

char inputKey = 0;
char input1 = 0;
char input2 = 0;
char input3;
char input4;
char button;

char *languageNameTable[] = {
	"FRANCAIS",
	"ITALIANO",
	"ENGLISH",
	"ESPAGNOL",
	"DEUTSCH",
	NULL,
};

char languageNameString[20] = "";

regularTextEntryStruct textTable[NUM_MAX_TEXT];

int readVar;

int hqrKeyGen = 0;

char *screenSm1;
char *screenSm2;
char *screenSm3;
char *screenSm4;
char *screenSm5;

actorStruct actorTable[NUM_MAX_ACTOR];

int16 currentCameraTarget;

int fileSize;

hqrEntryStruct *listBody;
hqrEntryStruct *listAnim;
hqrEntryStruct *listLife;
hqrEntryStruct *listTrack;
hqrEntryStruct *listMatrix;

int16 maxObjects;

objectStruct *objectTable;

int16 *vars;

int varSize;

messageStruct messageTable[NUM_MAX_MESSAGE];

int16 currentMusic;
int action;

boxStruct genVar2[15]; // recheckSize
boxStruct genVar4[50];
boxStruct *genVar1;
boxStruct *genVar3;

int genVar5;
int genVar6;
int genVar7;
int nextMusic;
int16 genVar9;
int16 giveUp;
int16 inHand;
int16 lightVar1;
int lightVar2;
int16 numObjInInventory;
int soundVar1;
int soundVar2;
int16 statusScreenAllowed;

char *etageVar0 = NULL;
char *etageVar1 = NULL;

int changeFloor;
int16 currentCamera;
int16 currentEtage;
int needChangeRoom;

char *cameraPtr;
roomDefStruct *pCurrentRoomData;

int16 currentDisplayedRoom;
int mainVar1;
int numCameraInRoom;
int numCameraZone;
char *cameraZoneData;
int numRoomZone;
char *roomZoneData;
char *roomVar5[NUM_MAX_CAMERA_IN_ROOM];
int startGameVar1;

int transformX;
int transformY;
int transformZ;
int transformXCos;
int transformXSin;
int transformYCos;
int transformYSin;
int transformZCos;
int transformZSin;
bool transformUseX;
bool transformUseY;
bool transformUseZ;

int translateX;
int translateY;
int translateZ;

int cameraCenterX;
int cameraCenterY;
int cameraX;
int cameraY;
int cameraZ;

char currentCameraVisibilityList[30];

int actorTurnedToObj = 0;

int currentProcessedActorIdx;
actorStruct *currentProcessedActorPtr;

int currentLifeActorIdx;
actorStruct *currentLifeActorPtr;
int currentLifeNum;

char *currentLifePtr;

int setupCameraVar1;

#ifdef USE_GL
float renderPointList[6400];
#else
int16 renderPointList[6400];
#endif

int numActorInList;
int sortedActorTable[NUM_MAX_ACTOR];

int angleCompX;
int angleCompZ;
int angleCompBeta;

int bufferAnimCounter = 0;

int animCurrentTime;
int animKeyframeLength;
int animMoveX;
int animMoveY;
int animMoveZ;
int animRot1;
int animRot2;
int animRot3;
char *animVar1;
char *animVar3;
char *animVar4;

int16 newFloor;

int paletteVar;

char cameraBuffer[256];
char cameraBuffer2[256];
char cameraBuffer3[400];
char cameraBuffer4[400];

char *cameraBufferPtr = cameraBuffer;
char *cameraBuffer2Ptr = cameraBuffer2;
char *cameraBuffer3Ptr = cameraBuffer3;

int overlaySize1;
int overlaySize2;

int bgOverlayVar1;

int16 newRoom;

int16 inventory[INVENTORY_SIZE];

int16 shakeVar1;
int16 shakingAmplitude;
unsigned int timerFreeze1;

hardColStruct *hardColTable[10];

int16 hardColVar1;
int16 hardColVar2;

ZVStruct hardClip;

char *listBodySelect[] = {
	"LISTBODY",
	"LISTBOD2",
};

char *listAnimSelect[] = {
	"LISTANIM",
	"LISTANI2",
};

saveEntry saveTable[40];

int hqrVar1 = 0;
int mainVar3 = 4000;
int mainVar2 = -2000;

int clipLeft = 0;
int clipTop = 0;
int clipRight = 319;
int clipBottom = 119;

hqrEntryStruct *listMus;
hqrEntryStruct *listSamp;

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
