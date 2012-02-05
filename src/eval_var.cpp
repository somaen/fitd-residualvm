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

#include "common/textconsole.h"
#include "fitd.h"
#include "common.h"

namespace Fitd {

int32 getPosRelTable[] = {4, 1, 8, 2, 4, 1, 8, 0};

int32 getMatrix(int32 param1, int32 actorIdx, int32 param2) {
	int32 matrixWidth;
	char *matrixPtr = listMatrix->get(param1);

	matrixWidth = *matrixPtr;
	matrixPtr++;

	matrixPtr += (actorTable[actorIdx].hardMat) * matrixWidth;
	matrixPtr += (actorTable[param2].hardMat);

	return *matrixPtr;
}

int32 getPosRel(actorStruct *actor1, actorStruct *actor2) {
	int32 beta1 = actor1->beta;
	int32 counter = 3;
	ZVStruct localZv;
	int32 centerX;
	int32 centerZ;

	if(beta1 >= 0x80 && beta1 < 0x180) {
		counter = 2;
	}

	if(beta1 >= 0x180 && beta1 < 0x280) {
		counter = 1;
	}

	if(beta1 >= 0x280 && beta1 < 0x380) {
		counter = 0;
	}


	copyZv(&actor2->zv, &localZv);

	if(actor1->room != actor2->room) {
		getZvRelativePosition(&localZv, actor2->room, actor1->room);
	}

	centerX = (localZv.ZVX1 + localZv.ZVX2) / 2;
	centerZ = (localZv.ZVZ1 + localZv.ZVZ2) / 2;

	if(actor1->zv.ZVZ2 >= centerZ && actor1->zv.ZVZ1 <= centerZ) {
		if(actor1->zv.ZVX2 < centerX) {
			counter++;
		} else {
			if(actor1->zv.ZVX1 <= centerX) {
				return(0);
			} else {
				counter += 3;
			}
		}
	} else if(actor1->zv.ZVX2 >= centerX || actor1->zv.ZVX1 <= centerX) {
		if(actor1->zv.ZVZ2 < centerZ) {
			counter += 2;
		} else {
			if(actor1->zv.ZVZ1 <= centerZ) {
				return(0);
			}
		}
	} else {
		return(0);
	}

	return(getPosRelTable[counter]);
}

int32 calcDist(int32 X1, int32 Y1, int32 Z1, int32 X2, int32 Y2, int32 Z2) {
	int32 Xdist = abs(X1 - X2);
	int32 Ydist = abs(Y1 - Y2);
	int32 Zdist = abs(Z1 - Z2);

	return(Xdist + Ydist + Zdist); // recheck overflow
}

int32 testZvEndAnim(actorStruct *actorPtr, char *animPtr, int32 param) {
	int16 var_16;
	int16 var_14;
	int16 var_E = 0;
	int16 var_12 = 0;
	int16 var_10 = param;
	int16 var_18;
	ZVStruct localZv;

	ASSERT(actorPtr);
	ASSERT(animPtr);

	var_16 = *(int16 *)(animPtr);
	animPtr += 2;
	var_14 = *(int16 *)(animPtr);
	animPtr += 2;

	for(var_18 = 0; var_18 < var_16; var_18 ++) {
		animPtr += 2;
		var_12 += *(int16 *)animPtr;
		animPtr += 2;
		animPtr += 2;
		var_E += *(int16 *)animPtr; // step depth
		animPtr += 2;

		animPtr += var_14 * 8;
	}

	copyZv(&actorPtr->zv, &localZv);

	walkStep(0, var_E, actorPtr->beta);

	localZv.ZVX1 += animMoveX;
	localZv.ZVX2 += animMoveX;
	localZv.ZVY1 += var_10;
	localZv.ZVY2 += var_10;
	localZv.ZVZ1 += animMoveY;
	localZv.ZVZ2 += animMoveY;

	if(checkForHardCol(&localZv, &roomDataTable[actorPtr->room])) {
		return(0);
	}

	localZv.ZVY1 += 100;
	localZv.ZVY2 += 100;

	if(checkForHardCol(&localZv, &roomDataTable[actorPtr->room])) {
		return(1);
	}

	return(0);
}

int evalVar(void) {
	int var1;

	if(g_fitd->getGameType() >= GType_JACK) {
		return evalVar2();
	}

	var1 = *(int16 *)(currentLifePtr);
	currentLifePtr += 2;

	if(var1 == -1) {
		int temp = *(int16 *)(currentLifePtr);
		currentLifePtr += 2;

		return(temp);
	} else if(var1 == 0) {
		int temp = *(int16 *)(currentLifePtr);
		currentLifePtr += 2;

		return(vars[temp]);
	} else {
		actorStruct *actorPtr = currentLifeActorPtr;
		int actorIdx = currentLifeActorIdx;

		if(var1 & 0x8000) {
			int objectNumber;

			objectNumber = *(int16 *)currentLifePtr;

			actorIdx = objectTable[objectNumber].ownerIdx;

			currentLifePtr += 2;
			actorPtr = &actorTable[actorIdx];

			if(actorIdx == -1) {
				switch(var1 & 0x7FFF) {
				case 0x1F: {
					return(objectTable[objectNumber].room);
					break;
				}
				case 0x26: {
					return(objectTable[objectNumber].stage);
					break;
				}
				default: {
					error("Unsupported evalVar %X when actor not in room !\n", var1 & 0x7FFF);
				}
				}
			}
		}
		{

			var1 &= 0x7FFF;

			var1--;

			switch(var1) {
			case 0x0: {
				int temp1 = actorPtr->COL[0];

				if(temp1 != -1) {
					return(actorTable[temp1].field_0);
				} else {
					return(-1);
				}
				break;
			}
			case 0x1: {
				return(actorPtr->HARD_DEC);
				break;
			}
			case 0x2: {
				return(actorPtr->HARD_COL);
				break;
			}
			case 0x3: {
				int temp = actorPtr->HIT;

				if(temp == -1) {
					return(-1);
				} else {
					return(actorTable[temp].field_0);
				}

				break;
			}
			case 0x4: {
				int temp = actorPtr->HIT_BY;

				if(temp == -1) {
					return(-1);
				} else {
					return(actorTable[temp].field_0);
				}

				break;
			}
			case 0x5: {
				return(actorPtr->ANIM);
				break;
			}
			case 0x6: {
				return(actorPtr->END_ANIM);
				break;
			}
			case 0x7: {
				return(actorPtr->FRAME);
				break;
			}
			case 0x8: {
				return(actorPtr->END_FRAME);
				break;
			}
			case 0x9: {
				return(actorPtr->bodyNum);
				break;
			}
			case 0xA: { // MARK
				return(actorPtr->MARK);
				break;
			}
			case 0xB: { // NUM_TRACK
				return(actorPtr->trackNumber);
				break;
			}
			case 0xC: { // CHRONO
				return(evalChrono(&actorPtr->CHRONO) / 60); // recheck
				break;
			}
			case 0xD: {
				return(evalChrono(&actorPtr->ROOM_CHRONO) / 60); // recheck
				break;
			}
			case 0xE: { // DIST
				int actorNumber = objectTable[*(int16 *)currentLifePtr].ownerIdx;
				currentLifePtr += 2;

				if(actorNumber == -1) {
					return(32000);
				} else {
					int tempX = actorTable[actorNumber].worldX;
					int tempY = actorTable[actorNumber].worldY;
					int tempZ = actorTable[actorNumber].worldZ;

					return(calcDist(actorPtr->worldX, actorPtr->worldY, actorPtr->worldZ, tempX, tempY, tempZ));
				}

				break;
			}
			case 0xF: { // COL_BY
				if(actorPtr->COL_BY == -1)
					return(-1);
				else
					return(actorTable[actorPtr->COL_BY].field_0);
				break;
			}
			case 0x10: { // found
				if(objectTable[evalVar()].flags2 & 0x8000) {
					return(1);
				} else {
					return(0);
				}

				break;
			}
			case 0x11: {
				return action;
				break;
			}
			case 0x12: { // POSREL
				int objNum;

				objNum = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				if(objectTable[objNum].ownerIdx == -1) {
					return 0;
				}

				return (getPosRel(actorPtr, &actorTable[objectTable[objNum].ownerIdx]));

				break;
			}
			case 0x13: {
				if(input4 & 4)
					return 4;
				if(input4 & 8)
					return 8;
				if(input4 & 1)
					return 1;
				if(input4 & 2)
					return 2;

				return 0;
				break;
			}
			case 0x14: {
				return(button);
				break;
			}
			case 0x15: {
				int temp1 = actorPtr->COL_BY;
				if(temp1 == -1)
					return -1;

				return actorTable[temp1].field_0;
				break;
			}
			case 0x16: {
				return(actorPtr->alpha);
				break;
			}
			case 0x17: {
				return(actorPtr->beta);
				break;
			}
			case 0x18: {
				return(actorPtr->gamma);
				break;
			}
			case 0x19: {
				return(inHand);
				break;
			}
			case 0x1A: {
				return(actorPtr->hitForce);
				break;
			}
			case 0x1B: {
				return(*(uint16 *)(((currentCamera + 6) * 2) + cameraPtr));
				break;
			}
			case 0x1C: {
				int temp = *(int16 *)currentLifePtr;
				currentLifePtr += 2;
				return(rand() % temp);
				break;
			}
			case 0x1D: {
				return(actorPtr->falling);
				break;
			}
			case 0x1E: {
				return(actorPtr->room);
				break;
			}
			case 0x1F: {
				return(actorPtr->life);
				break;
			}
			case 0x20: {
				int objNum;

				objNum = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				if(objectTable[objNum].flags2 & 0xC000) {
					return(1);
				} else {
					return(0);
				}

				break;
			}
			case 0x21: {
				return(actorPtr->roomY);
				break;
			}
			case 0x22: { // TEST_ZV_END_ANIM
				int temp1;
				int temp2;

				temp1 = *(int16 *)currentLifePtr;
				currentLifePtr += 2;
				temp2 = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				return(testZvEndAnim(actorPtr, listAnim->get(temp1), temp2));

				break;
			}
			case 0x23: { // TODO: music
				return(1);
				break;
			}
			case 0x24: {
				int temp = CVars[*(int16 *)currentLifePtr];
				currentLifePtr += 2;
				return(temp);
				break;
			}
			case 0x25: {
				return(actorPtr->stage);
				break;
			}
			case 0x26: { // THROW
				int objNum;

				objNum = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				if(objectTable[objNum].flags2 & 0x1000) {
					return 1;
				} else {
					return 0;
				}
				break;
			}
			default: {
				error("Unhandled test type %X in evalVar\n", var1);
				break;
			}
			}
		}
	}
}

int evalVar2(void) {
	int var1;

	var1 = *(int16 *)(currentLifePtr);
	currentLifePtr += 2;

	if(var1 == -1) {
		int temp = *(int16 *)(currentLifePtr);
		currentLifePtr += 2;

		return(temp);
	} else if(var1 == 0) {
		int temp = *(int16 *)(currentLifePtr);
		currentLifePtr += 2;

		return(vars[temp]);
	} else {
		actorStruct *actorPtr = currentLifeActorPtr;
		int actorIdx = currentLifeActorIdx;

		if(var1 & 0x8000) {
			int objectNumber;

			objectNumber = *(int16 *)currentLifePtr;

			actorIdx = objectTable[objectNumber].ownerIdx;

			currentLifePtr += 2;
			actorPtr = &actorTable[actorIdx];

			if(actorIdx == -1) {
				switch(var1 & 0x7FFF) {
				case 0x1F: {
					return(objectTable[objectNumber].room);
					break;
				}
				case 0x24: {
					return(objectTable[objectNumber].stage);
					break;
				}
				default: {
					error("Unsupported evalVar %X when actor not in room !\n", var1 & 0x7FFF);
				}
				}
			}
		}
		{

			var1 &= 0x7FFF;

			var1--;

			switch(var1) {
			case 0x0: {
				int temp1 = actorPtr->COL[0];

				if(temp1 != -1) {
					return(actorTable[temp1].field_0);
				} else {
					return(-1);
				}
				break;
			}
			case 0x1: {
				return(actorPtr->HARD_DEC);
				break;
			}
			case 0x2: {
				return(actorPtr->HARD_COL);
				break;
			}
			case 0x3: {
				int temp = actorPtr->HIT;

				if(temp == -1) {
					return(-1);
				} else {
					return(actorTable[temp].field_0);
				}

				break;
			}
			case 0x4: {
				int temp = actorPtr->HIT_BY;

				if(temp == -1) {
					return(-1);
				} else {
					return(actorTable[temp].field_0);
				}

				break;
			}
			case 0x5: {
				return(actorPtr->ANIM);
				break;
			}
			case 0x6: {
				return(actorPtr->END_ANIM);
				break;
			}
			case 0x7: {
				return(actorPtr->FRAME);
				break;
			}
			case 0x8: {
				return(actorPtr->END_FRAME);
				break;
			}
			case 0x9: {
				return(actorPtr->bodyNum);
				break;
			}
			case 0xA: { // MARK
				return(actorPtr->MARK);
				break;
			}
			case 0xB: { // NUM_TRACK
				return(actorPtr->trackNumber);
				break;
			}
			case 0xC: { // CHRONO
				return(evalChrono(&actorPtr->CHRONO) / 60); // recheck
				break;
			}
			case 0xD: {
				return(evalChrono(&actorPtr->ROOM_CHRONO) / 60); // recheck
				break;
			}
			case 0xE: { // DIST
				int actorNumber = objectTable[*(int16 *)currentLifePtr].ownerIdx;
				currentLifePtr += 2;

				if(actorNumber == -1) {
					return(32000);
				} else {
					int tempX = actorTable[actorNumber].worldX;
					int tempY = actorTable[actorNumber].worldY;
					int tempZ = actorTable[actorNumber].worldZ;

					return(calcDist(actorPtr->worldX, actorPtr->worldY, actorPtr->worldZ, tempX, tempY, tempZ));
				}

				break;
			}
			case 0xF: { // COL_BY
				if(actorPtr->COL_BY == -1)
					return(-1);
				else
					return(actorTable[actorPtr->COL_BY].field_0);
				break;
			}
			case 0x10: { // found
				if(objectTable[evalVar()].flags2 & 0x8000) {
					return(1);
				} else {
					return(0);
				}

				break;
			}
			case 0x11: {
				return action;
				break;
			}
			case 0x12: { // POSREL
				int objNum;

				objNum = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				if(objectTable[objNum].ownerIdx == -1) {
					return 0;
				}

				return (getPosRel(actorPtr, &actorTable[objectTable[objNum].ownerIdx]));

				break;
			}
			case 0x13: {
				if(input4 & 4)
					return 4;
				if(input4 & 8)
					return 8;
				if(input4 & 1)
					return 1;
				if(input4 & 2)
					return 2;

				return 0;
				break;
			}
			case 0x14: {
				return(button);
				break;
			}
			case 0x15: {
				int temp1 = actorPtr->COL_BY;
				if(temp1 == -1)
					return -1;

				return actorTable[temp1].field_0;
				break;
			}
			case 0x16: {
				return(actorPtr->alpha);
				break;
			}
			case 0x17: {
				return(actorPtr->beta);
				break;
			}
			case 0x18: {
				return(actorPtr->gamma);
				break;
			}
			case 0x19: {
				return(inHand);
				break;
			}
			case 0x1A: {
				return(actorPtr->hitForce);
				break;
			}
			case 0x1B: {
				return(*(uint16 *)(((currentCamera + 6) * 2) + cameraPtr));
				break;
			}
			case 0x1C: {
				int temp = *(int16 *)currentLifePtr;
				currentLifePtr += 2;
				return(rand() % temp);
				break;
			}
			case 0x1D: {
				return(actorPtr->falling);
				break;
			}
			case 0x1E: {
				return(actorPtr->room);
				break;
			}
			case 0x1F: {
				return(actorPtr->life);
				break;
			}
			case 0x20: {
				int objNum;

				objNum = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				if(objectTable[objNum].flags2 & 0xC000) {
					return(1);
				} else {
					return(0);
				}

				break;
			}
			case 0x21: { // TODO: music
				return(1);
				break;
			}
			case 0x22: { // c_var
				int temp = CVars[*(int16 *)currentLifePtr];
				currentLifePtr += 2;
				return(temp);
				break;
			}
			case 0x23: {
				return(actorPtr->stage);
				break;
			}
			case 0x24: { // THROW
				int objNum;

				objNum = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				if(objectTable[objNum].flags2 & 0x1000) {
					return 1;
				} else {
					return 0;
				}
				break;
			}
			case 0x25: { // get_matrix
				int param1;
				int param2;

				param1 = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				param2 = *(int16 *)currentLifePtr;
				currentLifePtr += 2;

				return getMatrix(param1, actorIdx, objectTable[param2].ownerIdx);
				break;
			}
			case 0x26: { // hard_mat
				return actorPtr->hardMat;
				break;
			}
			case 0x27: { // protection
				return 1;
				break;
			}
			default: {
				error("Unhandled test type %X in evalVar\n", var1);
				break;
			}
			}
		}
	}
}

} // end of namespace Fitd
