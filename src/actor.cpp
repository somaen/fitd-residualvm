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
#include "actor.h"

namespace Fitd {

int processActor1Sub1(int actorIdx, ZVStruct *zvPtr) {
	int currentCollisionSlot = 0;
	actorStruct *currentActor = actorTable;
	int actorRoom = actorTable[actorIdx].room;

	for(int i = 0; i < NUM_MAX_ACTOR; i++) {
		if(currentActor->field_0 != -1 && i != actorIdx) {
			ZVStruct *currentActorZv = &currentActor->zv;

			if(currentActor->room != actorRoom) {
				ZVStruct localZv;

				copyZv(zvPtr, &localZv);

				getZvRelativePosition(&localZv, actorRoom, currentActor->room);

				if(checkZvCollision(&localZv, currentActorZv)) {
					currentProcessedActorPtr->COL[currentCollisionSlot++] = i;

					if(currentCollisionSlot == 3)
						return(3);
				}
			} else {
				if(checkZvCollision(zvPtr, currentActorZv)) {
					currentProcessedActorPtr->COL[currentCollisionSlot++] = i;

					if(currentCollisionSlot == 3)
						return(3);
				}
			}
		}
		currentActor++;
	}

	return(currentCollisionSlot);
}

int processActor1Sub2(rotateStruct *data) {
	if(!data->param)
		return data->newAngle;

	if(g_fitd->getTimer() - data->timeOfRotate > (unsigned int)data->param) {
		data->param = 0;
		return data->newAngle;
	}

	return ((((data->newAngle - data->oldAngle) * (g_fitd->getTimer() - data->timeOfRotate)) / data->param) + data->oldAngle);
}

int manageFall(int actorIdx, ZVStruct *zvPtr) {
	int fallResult = 0;
	int i;
	int room = actorTable[actorIdx].room;

	for(i = 0; i < NUM_MAX_ACTOR; i++) {
		actorStruct *currentTestedActorPtr = &actorTable[i];

		if(currentTestedActorPtr->field_0 != -1 && i != actorIdx) {
			ZVStruct *testedZv = &currentTestedActorPtr->zv;

			if(currentTestedActorPtr->room != room) {
				ZVStruct localZv;
				copyZv(zvPtr, &localZv);
				getZvRelativePosition(&localZv, room, currentTestedActorPtr->room);

				if(checkZvCollision(&localZv, testedZv)) {
					actorTable[i].COL_BY = actorIdx;
					fallResult++;
				}
			} else {
				if(checkZvCollision(zvPtr, testedZv)) {
					actorTable[i].COL_BY = actorIdx;
					fallResult++;
				}
			}
		}
	}

	return(fallResult);
}

void hardColSuB1Sub1(int flag) {
	switch(flag) {
	case 1:
	case 2: {
		hardColVar2 = 0;
		break;
	}
	case 4:
	case 8: {
		hardColVar1 = 0;
		break;
	}
	default: {
		break;
	}
	}
}

void hardColSuB1(ZVStruct *startZv, ZVStruct *zvPtr2, ZVStruct *zvPtr3) {
	int32 flag = 0;
	int32 var_8;
	int32 halfX;
	int32 halfZ;
	int32 var_A;
	int32 var_6;

	if(startZv->ZVX2 > zvPtr3->ZVX1) {
		if(zvPtr3->ZVX2 <= startZv->ZVX1) {
			flag = 8;
		}
	} else {
		flag = 4;
	}

	if(startZv->ZVZ2 > zvPtr3->ZVZ1) {
		if(startZv->ZVZ1 >= zvPtr3->ZVZ2) {
			flag |= 2;
		}
	} else {
		flag |= 1;
	}

	if(flag == 5 || flag == 9 || flag == 6 || flag == 10) {
		var_8 = 2;
	} else {
		if(!flag) {
			var_8 = 0;

			hardColVar2 = 0;
			hardColVar1 = 0;

			return;
		} else {
			var_8 = 1;
		}
	}

	halfX = (zvPtr2->ZVX1 + zvPtr2->ZVX2) / 2;
	halfZ = (zvPtr2->ZVZ1 + zvPtr2->ZVZ2) / 2;

	if(zvPtr3->ZVX1 > halfX) {
		var_A = 4;
	} else {
		if(zvPtr3->ZVX2 < halfX) {
			var_A = 0;
		} else {
			var_A = 8;
		}
	}

	if(zvPtr3->ZVZ1 > halfZ) {
		var_A |= 1;
	} else {
		if(zvPtr3->ZVZ2 < halfZ) {
			var_A |= 0; // once again, not that much usefull
		} else {
			var_A |= 2;
		}
	}

	if(var_A == 5 || var_A == 9 || var_A == 6 || var_A == 10) {
		var_6 = 2;
	} else {
		if(!var_A) {
			var_6 = 0;
		} else {
			var_6 = 1;
		}
	}

	if(var_8 == 1) {
		hardColSuB1Sub1(flag);
		return;
	}

	if(var_6 == 1 && (var_A & flag)) {
		hardColSuB1Sub1(var_A);
		return;
	}

	if(var_A == flag || flag == 15) {
		int Xmod = abs(zvPtr2->ZVX1 - startZv->ZVX1); // recheck
		int Zmod = abs(zvPtr2->ZVZ1 - startZv->ZVZ1);

		if(Xmod > Zmod) {
			hardColVar2 = 0;
		} else {
			hardColVar1 = 0;
		}
	} else {
		if(!var_6 || (var_6 == 1 && !(var_A & flag))) {
			hardColVar2 = 0;
			hardColVar1 = 0;
		} else {
			hardColSuB1Sub1(flag & var_A);
		}
	}
}

void processActor1(void) {
	int var_42 = 0;
	int var_6 = currentProcessedActorPtr->field_44;
	int var_40;
	int var_48 = 0;
	int var_4A = 0;
	int var_4C = 0;
	int var_4E;
	int var_50 = 0;
	int var_52 = 0;
	int var_56;
	int16 localTable[3];
	ZVStruct zvLocal;
	ZVStruct *zvPtr;
	int j;

	static int32 bufferAnimCounter = 0;

	if(var_6 != -1) { // next anim ?
		if(var_6 == -2) { // completly stop anim
			stopAnim(currentProcessedActorIdx);
			currentProcessedActorPtr->field_44 = -1;
			currentProcessedActorPtr->field_46 = 0;
			currentProcessedActorPtr->field_48 = -1;
			currentProcessedActorPtr->END_ANIM = 1;

			return;
		}

		if(currentProcessedActorPtr->END_FRAME == 0) {
			currentProcessedActorPtr->worldX += currentProcessedActorPtr->modX;
			currentProcessedActorPtr->roomX += currentProcessedActorPtr->modX;

			currentProcessedActorPtr->worldZ += currentProcessedActorPtr->modZ;
			currentProcessedActorPtr->roomZ += currentProcessedActorPtr->modZ;

			currentProcessedActorPtr->modX = 0;
			currentProcessedActorPtr->modZ = 0;
		}
		initBufferAnim(bufferAnim + (bufferAnimCounter++) * 248, listBody->get(currentProcessedActorPtr->bodyNum));

		if(bufferAnimCounter == 20)
			bufferAnimCounter = 0;

		currentProcessedActorPtr->ANIM = var_6;
		currentProcessedActorPtr->field_40 = currentProcessedActorPtr->field_46;
		currentProcessedActorPtr->field_42 = currentProcessedActorPtr->field_48;
		currentProcessedActorPtr->field_44 = -1;
		currentProcessedActorPtr->field_46 = 0;
		currentProcessedActorPtr->field_48 = -1;
		currentProcessedActorPtr->END_ANIM = 0;
		currentProcessedActorPtr->FRAME = 0;

		currentProcessedActorPtr->field_4C = getAnimParam(listAnim->get(var_6));
	}

	if(currentProcessedActorPtr->ANIM == -1) { // no animation
		currentProcessedActorPtr->END_FRAME = 0;
		if(currentProcessedActorPtr->speed == 0) {
			var_42 = processActor1Sub1(currentProcessedActorIdx, &currentProcessedActorPtr->zv);

			if(var_42) {
				for(var_40 = 0; var_40 < var_42; var_40 ++) {
					actorTable[currentProcessedActorPtr->COL[var_40]].COL_BY = currentProcessedActorIdx; // collision with current actor
				}
			}

			var_40 = var_42;

			while(var_40 > 0) {
				currentProcessedActorPtr->COL[var_40] = -1;

				var_40 --;
			}

			var_4A = 0;
			var_48 = 0;
			var_52 = 0;
			var_50 = 0;
		} else {
			var_4C = currentProcessedActorPtr->modX;
			var_4A = currentProcessedActorPtr->modY;
			var_48 = currentProcessedActorPtr->modZ;

			animRot3 = 0;
			animRot2 = 0;

			animRot1 = processActor1Sub2(&currentProcessedActorPtr->speedChange);

			walkStep(0, animRot1, currentProcessedActorPtr->beta);

			var_52 = animMoveX - var_4C;
			var_50 = animMoveY - var_48;
		}

		var_4E = 0;
	} else { // animation
		var_4C = currentProcessedActorPtr->modX;
		var_4A = currentProcessedActorPtr->modY;
		var_48 = currentProcessedActorPtr->modZ;

		currentProcessedActorPtr->END_FRAME = setInterAnimObjet(currentProcessedActorPtr->FRAME, listAnim->get(currentProcessedActorPtr->ANIM), listBody->get(currentProcessedActorPtr->bodyNum));

		walkStep(animRot2, animRot1, currentProcessedActorPtr->beta);

		var_52 = animMoveX - var_4C;
		var_50 = animMoveY - var_48;

	}

	if(currentProcessedActorPtr->field_60.param) { // currently falling ?
		if(currentProcessedActorPtr->field_60.param != -1) {
			var_4E = processActor1Sub2(&currentProcessedActorPtr->field_60) - var_4A;
		} else { // stop falling
			var_4E = currentProcessedActorPtr->field_60.newAngle - var_4A;

			currentProcessedActorPtr->field_60.param = 0;
			currentProcessedActorPtr->field_60.newAngle = 0;
			currentProcessedActorPtr->field_60.oldAngle = 0;
		}
	} else {
		var_4E = 0;
	}

	memcpy(localTable, currentProcessedActorPtr->COL, 6);
	var_56 = -1;

	if(var_52 || var_50 || var_4E) { // start of movement management
		zvPtr = &currentProcessedActorPtr->zv;
		copyZv(&currentProcessedActorPtr->zv, &zvLocal);

		zvLocal.ZVX1 += var_52;
		zvLocal.ZVX2 += var_52;

		zvLocal.ZVY1 += var_4E;
		zvLocal.ZVY2 += var_4E;

		zvLocal.ZVZ1 += var_50;
		zvLocal.ZVZ2 += var_50;

		if(currentProcessedActorPtr->dynFlags & 1) { // hard collision enabled for actor ?
			int i;

			var_42 = checkForHardCol(&zvLocal, &roomDataTable[currentProcessedActorPtr->room]);

			for(i = 0; i < var_42; i++) {
				hardColStruct *var_3E = hardColTable[i];

				if(var_3E->type == 9) {
					currentProcessedActorPtr->HARD_COL = (short)var_3E->parameter;
				}

				if(var_3E->type == 3) {
					currentProcessedActorPtr->HARD_COL = 255;
				}

				if(g_fitd->getGameType() == GType_AITD1 || (g_fitd->getGameType() >= GType_JACK && (var_3E->type != 10 || currentProcessedActorIdx != genVar9))) {
					if(var_52 || var_50) { // move on the X or Y axis ? update to avoid entering the hard col

						hardColVar1 = var_52;
						hardColVar2 = var_50;

						hardColSuB1(zvPtr, &zvLocal, &var_3E->zv);

						zvLocal.ZVX1 +=  hardColVar1 - var_52;
						zvLocal.ZVX2 +=  hardColVar1 - var_52;
						zvLocal.ZVZ1 +=  hardColVar2 - var_50;
						zvLocal.ZVZ2 +=  hardColVar2 - var_50;

						var_52 = hardColVar1;
						var_50 = hardColVar2;
					}
				}
			}
		} else { // no hard collision -> just update the flag without performing the position update
			if(checkForHardCol(&zvLocal, &roomDataTable[currentProcessedActorPtr->room])) {
				currentProcessedActorPtr->HARD_COL = 1;
			} else {
				currentProcessedActorPtr->HARD_COL = 0;
			}
		}

		var_42 = processActor1Sub1(currentProcessedActorIdx, &zvLocal); // get the number of actor/actor collision

		for(j = 0; j < var_42; j++) { // process the actor/actor collision
			ZVStruct *touchedZv;
			actorStruct *actorTouchedPtr;

			var_56 = currentProcessedActorPtr->COL[j];

			actorTouchedPtr = &actorTable[var_56];

			actorTouchedPtr->COL_BY = currentProcessedActorIdx;

			touchedZv = &actorTouchedPtr->zv;

			if(actorTouchedPtr->bitField.tackable) { // takable
				if(currentProcessedActorPtr->trackMode == 1 /*&& ((g_fitd->getGameType() == GType_AITD1 && defines.field_1E == 0) || (g_fitd->getGameType() >= GType_JACK && defines.field_6 == 0))*/) { // TODO: check if character isn't dead...
					foundObject(actorTouchedPtr->field_0, 0);
				}
			} else {
				if(actorTouchedPtr->flags & 0x10) { // can be pushed ?
					ZVStruct localZv2;

					bool isPushPossible = true;

					copyZv(touchedZv, &localZv2);

					localZv2.ZVX1 += var_52;
					localZv2.ZVX2 += var_52;

					localZv2.ZVZ1 += var_50;
					localZv2.ZVZ2 += var_50;

					if(!checkForHardCol(&localZv2, &roomDataTable[currentProcessedActorPtr->room])) {
						if(processActor1Sub1(var_56, &localZv2)) {
							isPushPossible = false;
						}
					} else {
						isPushPossible = false;
					}

					if(!isPushPossible) {
						if(var_52 || var_50) { //if we're trying to move
							if(actorTouchedPtr->room != currentProcessedActorPtr->room) {
								ZVStruct localZv3;

								copyZv(touchedZv, &localZv3);

								getZvRelativePosition(&localZv3, actorTouchedPtr->room, currentProcessedActorPtr->room);

								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, &localZv3);

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							} else {
								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, touchedZv); // manage as hard collision

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							}
						}
					} else { // push succeed
						if(actorTouchedPtr->flags & 8) {
							deleteSub(var_56);
						}

						actorTouchedPtr->flags |= 1;

						actorTouchedPtr->worldX += var_52; // apply push to object
						actorTouchedPtr->worldZ += var_50;

						actorTouchedPtr->roomX += var_52;
						actorTouchedPtr->roomZ += var_50;

						copyZv(&localZv2, touchedZv);
					}
				} else { // can be pushed
					if(currentProcessedActorPtr->dynFlags & 1) {
						if(var_52 || var_50) { // if moving
							if(actorTouchedPtr->room == currentProcessedActorPtr->room) { // same room -> easy case
								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, touchedZv); // manage as hard collision

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							} else { // different room
								ZVStruct localZv3;

								copyZv(touchedZv, &localZv3);

								getZvRelativePosition(&localZv3, actorTouchedPtr->room, currentProcessedActorPtr->room);

								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, &localZv3); // manage as hard collision

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							}
						}
					}
				}
			}
		} // end of actor/actor collision

		currentProcessedActorPtr->modX = var_52 + var_4C;
		currentProcessedActorPtr->modY = var_4E + var_4A;
		currentProcessedActorPtr->modZ = var_50 + var_48;

		currentProcessedActorPtr->zv.ZVX1 += var_52;
		currentProcessedActorPtr->zv.ZVX2 += var_52;

		currentProcessedActorPtr->zv.ZVY1 += var_4E;
		currentProcessedActorPtr->zv.ZVY2 += var_4E;

		currentProcessedActorPtr->zv.ZVZ1 += var_50;
		currentProcessedActorPtr->zv.ZVZ2 += var_50;
	} // end of movement management

	if(!currentProcessedActorPtr->field_60.param) {
		// fall management ?
		currentProcessedActorPtr->worldY += currentProcessedActorPtr->modY;
		currentProcessedActorPtr->roomY += currentProcessedActorPtr->modY;

		currentProcessedActorPtr->modY = 0;

		if(currentProcessedActorPtr->flags & 0x100) {
			zvPtr = &currentProcessedActorPtr->zv;

			copyZv(zvPtr, &zvLocal);

			zvLocal.ZVY2 += 100;

			if(currentProcessedActorPtr->roomY < -10 && !checkForHardCol(&zvLocal, &roomDataTable[currentProcessedActorPtr->room]) && !manageFall(currentProcessedActorIdx, &zvLocal)) {
				startActorRotation(0, 2000, 40, &currentProcessedActorPtr->field_60);
			} else {
				currentProcessedActorPtr->falling = 0;
			}
		}
	} else {
		if((currentProcessedActorPtr->field_60.param != -1) && (currentProcessedActorPtr->flags & 0x100)) {
			currentProcessedActorPtr->falling = 1;
		}
	}

	var_40 = var_42;

	while(var_40 > 0) {
		currentProcessedActorPtr->COL[var_40] = -1;

		var_40 --;
	}

	for(var_40 = 0; var_40 < 3; var_40++) {
		var_56 = localTable[var_40];

		if(var_56 != -1) {
			actorStruct *actorTouchedPtr = &actorTable[var_56];

			if(actorTouchedPtr->flags & 0x10) {
				int i;

				for(i = 0; i < 3; i++) {
					if(currentProcessedActorPtr->COL[i] == var_56)
						break;
				}

				if(i == 3) {
					actorTouchedPtr->flags &= 0xFFFE;
					stopAnim(var_56);
				}
			}
		}
	}

	if(currentProcessedActorPtr->END_FRAME) { // key frame change
		currentProcessedActorPtr->FRAME++;

		if(currentProcessedActorPtr->FRAME >= currentProcessedActorPtr->field_4C) { // end of anim ?
			currentProcessedActorPtr->END_ANIM = 1; // end of anim
			currentProcessedActorPtr->FRAME = 0; // restart anim

			if(!(currentProcessedActorPtr->field_40 & 1) && (currentProcessedActorPtr->field_44 == -1)) { // is another anim waiting ?
				currentProcessedActorPtr->field_40 &= 0xFFFD;

				anim(currentProcessedActorPtr->field_42, 1, -1);
			}
		}
		currentProcessedActorPtr->worldX += currentProcessedActorPtr->modX;
		currentProcessedActorPtr->roomX += currentProcessedActorPtr->modX;

		currentProcessedActorPtr->worldZ += currentProcessedActorPtr->modZ;
		currentProcessedActorPtr->roomZ += currentProcessedActorPtr->modZ;

		currentProcessedActorPtr->modX = 0;
		currentProcessedActorPtr->modZ = 0;
	} else { // not the end of anim
		if((currentProcessedActorPtr->ANIM == -1) && (currentProcessedActorPtr->speed != 0) && (currentProcessedActorPtr->speedChange.param == 0)) {
			currentProcessedActorPtr->worldX += currentProcessedActorPtr->modX;
			currentProcessedActorPtr->roomX += currentProcessedActorPtr->modX;

			currentProcessedActorPtr->worldZ += currentProcessedActorPtr->modZ;
			currentProcessedActorPtr->roomZ += currentProcessedActorPtr->modZ;

			currentProcessedActorPtr->modX = 0;
			currentProcessedActorPtr->modZ = 0;

			startActorRotation(0, currentProcessedActorPtr->speed, 60, &currentProcessedActorPtr->speedChange);
		}

		currentProcessedActorPtr->END_ANIM = 0;
	}
}

sceZoneStruct *processActor2Sub(int x, int y, int z, roomDataStruct *pRoomData) {
	uint32 i;
	sceZoneStruct *pCurrentZone;

	pCurrentZone = pRoomData->sceZoneTable;

	for(i = 0; i < pRoomData->numSceZone; i++) {
		if(pCurrentZone->zv.ZVX1 <= x && pCurrentZone->zv.ZVX2 >= x) {
			if(pCurrentZone->zv.ZVY1 <= y && pCurrentZone->zv.ZVY2 >= y) {
				if(pCurrentZone->zv.ZVZ1 <= z && pCurrentZone->zv.ZVZ2 >= z) {
					return(pCurrentZone);
				}
			}
		}

		pCurrentZone++;
	}

	return(NULL);

}

void processActor2() {
	sceZoneStruct *ptr;

	ptr = processActor2Sub(currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
	                       currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY,
	                       currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
	                       &roomDataTable[currentProcessedActorPtr->room]);


	if(!ptr) {
		return;
	}

	switch(ptr->type) {
	case 0: {
		int x;
		int y;
		int z;

		int oldRoom = currentProcessedActorPtr->room;

		currentProcessedActorPtr->room = (short)ptr->parameter;

		x = (roomDataTable[currentProcessedActorPtr->room].worldX - roomDataTable[oldRoom].worldX) * 10;
		y = (roomDataTable[currentProcessedActorPtr->room].worldY - roomDataTable[oldRoom].worldY) * 10;
		z = (roomDataTable[currentProcessedActorPtr->room].worldZ - roomDataTable[oldRoom].worldZ) * 10;

		currentProcessedActorPtr->roomX -= x;
		currentProcessedActorPtr->roomY += y;
		currentProcessedActorPtr->roomZ += z;

		currentProcessedActorPtr->zv.ZVX1 -= x;
		currentProcessedActorPtr->zv.ZVX2 -= x;

		currentProcessedActorPtr->zv.ZVY1 += y;
		currentProcessedActorPtr->zv.ZVY2 += y;

		currentProcessedActorPtr->zv.ZVZ1 += z;
		currentProcessedActorPtr->zv.ZVZ2 += z;

		if(currentProcessedActorIdx == genVar9) {
			needChangeRoom = 1;
			newRoom = (short)ptr->parameter;
		} else {
			actorTurnedToObj = 1;
		}

		startChrono(&currentProcessedActorPtr->ROOM_CHRONO);


		break;
	}
	case 8: {
		if(g_fitd->getGameType() != GType_AITD1) {
			currentProcessedActorPtr->hardMat = (short)ptr->parameter;
		}
		break;
	}
	case 9: {
		currentProcessedActorPtr->HARD_DEC = (short)ptr->parameter;
		break;
	}
	case 10: {
		int life;

		life = objectTable[currentProcessedActorPtr->field_0].field_24;

		if(life == -1)
			return;

		currentProcessedActorPtr->life = life;

		currentProcessedActorPtr->HARD_DEC = (short)ptr->parameter;
		break;
	}
	}
}

} // end of namespace Fitd
