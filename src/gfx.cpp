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

#include "common.h"
#include "fitd.h"
#include "gfx.h"

namespace Fitd {

void loadCamera(int cameraIdx) {
	char name[16];
	int useSpecial = -1;

	sprintf(name, "CAMERA%02d", currentEtage);
	//strcat(name,".PAK");

	if(g_fitd->getGameType() == GType_AITD1) {
		if(CVars[getCVarsIdx(LIGHT_OBJECT)] == 1) {
			switch(currentEtage) {
			case 6: {
				if(cameraIdx == 0) {
					useSpecial = 17;
				}
				if(cameraIdx == 5) {
					useSpecial = 18;
				}
				break;
			}
			case 7: {
				if(cameraIdx == 1) {
					useSpecial = 16;
				}
				break;
			}
			}
		}

		if(useSpecial != -1) {
			strcpy(name, "ITD_RESS");
			cameraIdx = useSpecial;
		}
	}

	if(!loadPakToPtr(name, cameraIdx, aux)) {
		theEnd(0, name);
	}

	if(g_fitd->getGameType() == GType_AITD3) {
		memmove(aux, aux + 4, 64000 + 0x300);
	}

	if(g_fitd->getGameType() >= GType_JACK) {
		copyPalette(aux + 64000, g_driver->_palette);

		if(g_fitd->getGameType() == GType_AITD3) {
			//memcpy(palette,defaultPaletteAITD3,0x30);
		} else {
			g_driver->_paletteObj->loadDefault();
			g_driver->_paletteObj->convertIfRequired();
		}

		g_driver->setPalette((byte*)g_driver->_palette);
	}
}

int setupCameraSub1Sub1(int value) {
	char *ptr = currentCameraVisibilityList;
	int var;

	while((var = *(ptr++)) != -1) {
		if(value == var) {
			return(1);
		}
	}

	return(0);
}

// setup visibility list
void setupCameraSub1() {
	uint32 i;
	int j;
	int var_10;

	char *dataTabPos = currentCameraVisibilityList;

	*dataTabPos = -1;

	// visibility list: add linked rooms
	for(i = 0; i < roomDataTable[currentDisplayedRoom].numSceZone; i++) {
		if(roomDataTable[currentDisplayedRoom].sceZoneTable[i].type == 0) {
			var_10 = roomDataTable[currentDisplayedRoom].sceZoneTable[i].parameter;
			if(!setupCameraSub1Sub1(var_10)) {
				*(dataTabPos++) = var_10;
				*(dataTabPos) = -1;
			}
		}
	}

	// visibility list: add room seen by the current camera
	for(j = 0; j < cameraDataTable[currentCamera]->_numCameraZoneDef; j++) {
		if(!setupCameraSub1Sub1(cameraDataTable[currentCamera]->_cameraZoneDefTable[j].dummy1)) {
			*(dataTabPos++) = (char)cameraDataTable[currentCamera]->_cameraZoneDefTable[j].dummy1;
			*(dataTabPos) = -1;
		}
	}
}

void setupCameraSub4(void) {
	copyToScreen(aux, aux2);

	//TODO: implementer la suite
}

void setupCamera() {
	int x;
	int y;
	int z;
	cameraDataStruct *pCamera;

	freezeTime();

	currentCamera = startGameVar1;

	loadCamera(roomDataTable[currentDisplayedRoom].cameraIdxTable[startGameVar1]);

	pCamera = cameraDataTable[currentCamera];

	setupPointTransformSM(pCamera->_alpha, pCamera->_beta, pCamera->_gamma);

#if INTERNAL_DEBUGGER
	if(debuggerVar_topCamera)
		setupPointTransformSM(0x100, 0, 0);
#endif

	x = (pCamera->_x - roomDataTable[currentDisplayedRoom].worldX) * 10;
	y = (roomDataTable[currentDisplayedRoom].worldY - pCamera->_y) * 10;
	z = (roomDataTable[currentDisplayedRoom].worldZ - pCamera->_z) * 10;

#if INTERNAL_DEBUGGER
	if(debuggerVar_topCamera) {
		x = actorTable[genVar9].worldX + actorTable[genVar9].modX;
		y = debufferVar_topCameraZoom;
		z = actorTable[genVar9].worldZ + actorTable[genVar9].modZ;
	}
#endif
	setupSelfModifyingCode(x, y, z); // setup camera position

	setupSMCode(160, 100, pCamera->_focal1, pCamera->_focal2, pCamera->_focal3); // setup focale

#if INTERNAL_DEBUGGER
	if(debuggerVar_topCamera)
		setupSMCode(160, 100, 1000, 100, 100); // setup focale
#endif

	setupCameraSub1();
	updateAllActorAndObjects();
	createActorList();
	//  setupCameraSub3();
	setupCameraSub4();
	/*  setupCameraSub5();
	 */
	if(mainVar1 == 2) {
		setupCameraVar1 = 2;
	} else {
		if(setupCameraVar1 != 2) {
			setupCameraVar1 = 1;
		}
	}

	mainVar1 = 0;
	unfreezeTime();
}

bool isBgOverlayRequired(int X1, int X2, int Z1, int Z2, char *data, int param) {
	int i;
	for(i = 0; i < param; i++) {
		////////////////////////////////////// DEBUG
		//  drawOverlayZone(data, 80);
		/////////////////////////////////////

		int zoneX1 = *(int16 *)(data);
		int zoneZ1 = *(int16 *)(data + 2);
		int zoneX2 = *(int16 *)(data + 4);
		int zoneZ2 = *(int16 *)(data + 6);

		if(X1 >= zoneX1 && Z1 >= zoneZ1 && X2 <= zoneX2 && Z2 <= zoneZ2) {
			return(true);
		}

		data += 0x8;
	}

	return(false);
}

void drawBgOverlaySub2(int size) {
	int bx = 32767;
	int bp = 32767;
	int cx = -32768;
	int dx = -32768;
	int16 *out;
	int tempBxPtr;
	int tempCxPtr;

	char *si;

	int i;
	int saveDx;
	int saveAx;

	char *tempBufferSE;

	int direction = 1;

	int16 *data = (int16 *)cameraBuffer;

	overlaySize1 = size;
	overlaySize2 = size;

	bgOverlayVar1 = 0;

	for(i = 0; i < size; i++) {
		int temp = data[0];

		if(temp < bx)
			bx = temp;
		if(temp > dx)
			dx = temp;

		temp = data[1];

		if(temp < bp)
			bp = temp;
		if(temp > cx)
			cx = temp;

		data += 2;
	}

	out = data;
	data = (int16 *)cameraBuffer;

	out[0] = data[0];
	out[1] = data[1];

	out += 4;
	data += 4;

	if(cx == bp) {
		return;
	}

	cameraBufferPtr = cameraBuffer;
	cameraBuffer2Ptr = cameraBuffer2;
	cameraBuffer3Ptr = cameraBuffer3;

	si = cameraBufferPtr;
	tempBxPtr = *(int16 *)si;
	si += 2;
	tempCxPtr = *(int16 *)si;
	si += 2;

	tempBufferSE = cameraBuffer4;

	direction = 1;

	g_driver->startBgPoly();

	do {
		int dx;
		int ax;

		saveDx = *(int16 *)si;
		si += 2;
		saveAx = *(int16 *)si;
		si += 2;

#ifdef INTERNAL_DEBUGGER
		/*  if(backgroundMode == backgroundModeEnum_2D)
		 g_driver->draw3dLine(tempBxPtr, tempCxPtr, 0, saveDx,saveAx, 0, 130); */
#endif
		g_driver->addBgPolyPoint(tempBxPtr, tempCxPtr);

		dx = saveDx;
		ax = saveAx;

		if(ax != tempCxPtr) {
			char *ptr1;
			if(tempCxPtr == bp || tempCxPtr == cx) {
				char *temp = cameraBuffer3Ptr;
				cameraBuffer3Ptr = tempBufferSE;
				tempBufferSE = temp;
			}

			if(tempBxPtr >= dx) {
				int temp;
				temp = tempBxPtr;
				tempBxPtr = dx;
				dx = temp;

				temp = tempCxPtr;
				tempCxPtr = ax;
				ax = temp;
			}

			ptr1 = tempBufferSE + tempCxPtr * 2;

			if(tempCxPtr > ax) {
				int temp;
				direction = -1;


				temp = tempCxPtr;
				tempCxPtr = ax;
				ax = temp;
			}

			{
				// stupid, need optimisation
				int temp;

				temp = tempCxPtr;
				tempCxPtr = ax;
				ax = temp;
			}

			tempCxPtr -= ax;

			*(int16 *)ptr1 = tempBxPtr;
			ptr1 += 2 * direction;

			ax = tempBxPtr;

			dx -= tempBxPtr;
			tempBxPtr = dx;
			dx = tempCxPtr;
			bp = tempCxPtr / 2;

			do {
				bp += tempBxPtr;

				do {
					if(bp < dx)
						break;

					bp -= dx;
					ax ++;
				} while(1);

				*(int16 *)ptr1 = ax;
				ptr1 += 2 * direction;
			} while(--tempCxPtr);

			direction = 1;
		}

		tempCxPtr = saveAx;
		tempBxPtr = saveDx;
	} while(--overlaySize1);

	g_driver->endBgPoly();
}

void drawBgOverlay(actorStruct *actorPtr) {
	char *data;
	char *data2;
	int numEntry;
	int i;
	int numOverlayZone;

	actorPtr->field_14 = BBox3D1;
	actorPtr->field_16 = BBox3D2;
	actorPtr->field_18 = BBox3D3;
	actorPtr->field_1A = BBox3D4;

	setClipSize(BBox3D1, BBox3D2, BBox3D3, BBox3D4);

	data = roomVar5[currentCamera] + 0x12;
	numEntry = *(int16 *)(data);
	data += 2;

	while(numEntry > 0) {
		if(actorPtr->room == *(int16 *)(data)) {
			break;
		}
		data += 12;
		numEntry--;
	}

	if(numEntry == 0)
		return;

	data += 2;

	data2 = roomVar5[currentCamera] + *(uint16 *)(data);
	data = data2;
	data += 2;

	numOverlayZone = *(int16 *)(data2);

	for(i = 0; i < numOverlayZone; i++) {
		int numOverlay;
		char *src = data2 + *(uint16 *)(data + 2);

		if(isBgOverlayRequired(actorPtr->zv.ZVX1 / 10, actorPtr->zv.ZVX2 / 10,
		                       actorPtr->zv.ZVZ1 / 10, actorPtr->zv.ZVZ2 / 10,
		                       data + 4,
		                       *(int16 *)(data))) {
			int j;
			numOverlay = *(int16 *)src;
			src += 2;

			for(j = 0; j < numOverlay; j++) {
				int param = *(int16 *)(src);
				src += 2;

				memcpy(cameraBuffer, src, param * 4);

				src += param * 4;

				drawBgOverlaySub2(param);
			}

			//      blitOverlay(src);
		}

		numOverlay = *(int16 *)(data);
		data += 2;
		data += ((numOverlay * 4) + 1) * 2;
	}

	setClipSize(0, 0, 319, 199);
}

void mainDrawSub2(int actorIdx) { // draw flow
	actorStruct *actorPtr = &actorTable[actorIdx];

	char *data = hqrUnk->printTextSub2(actorPtr->FRAME);

	// TODO: finish
}

void getHotPoint(int hotPointIdx, char *bodyPtr, point3dStruct *hotPoint) {
	int16 flag;

	flag = *(int16 *)bodyPtr;
	bodyPtr += 2;

	if(flag & 2) {
		int16 offset;
		bodyPtr += 12;

		offset = *(int16 *)bodyPtr;
		bodyPtr += 2;
		bodyPtr += offset;

		offset = *(int16 *)bodyPtr; // num points
		bodyPtr += 2;
		bodyPtr += offset * 6; // skip point buffer

		offset = *(int16 *)bodyPtr; // num bones
		bodyPtr += 2;
		bodyPtr += offset * 2; // skip bone buffer

		ASSERT(hotPointIdx < offset);

		if(hotPointIdx < offset) {
			int pointIdx;
			int16 *source;

			if(flag & 8) {
				bodyPtr += hotPointIdx * 0x18;
			} else {
				bodyPtr += hotPointIdx * 16;
			}

			pointIdx = *(int16 *)(bodyPtr + 4); // first point

			//ASSERT(pointIdx > 0 && pointIdx < 1200);

			source = (int16 *)(((char *)pointBuffer) + pointIdx);

			hotPoint->x = source[0];
			hotPoint->y = source[1];
			hotPoint->z = source[2];
		} else {
			hotPoint->x = 0;
			hotPoint->y = 0;
			hotPoint->z = 0;
		}
	} else {
		hotPoint->x = 0;
		hotPoint->y = 0;
		hotPoint->z = 0;
	}
}

void mainDraw(int mode) {
#ifdef USE_GL
	if(mode == 2)
		g_driver->CopyBlockPhys((unsigned char *)screen, 0, 0, 320, 200);
#endif


	if(mode == 0) {
		//restoreDirtyRects();
	} else {
		genVar5 = 0;
		copyToScreen(aux2, screen);
	}

#ifdef USE_GL
	g_driver->startFrame();
#endif

	setClipSize(0, 0, 319, 199);
	genVar6 = 0;

#ifdef USE_GL
	g_driver->cleanScreenKeepZBuffer();
#endif

#ifdef INTERNAL_DEBUGGER
	if(backgroundMode == backgroundModeEnum_3D) {
		for(int i = 0; i < getNumberOfRoom(); i++) {
			drawHardCol(i);
			drawSceZone(i);
		}

		drawConverZones();
	}
#endif



#ifdef USE_GL
	g_driver->startModelRender();
#endif

	for(int i = 0; i < numActorInList; i++) {
		int currentDrawActor = sortedActorTable[i];
		actorStruct *actorPtr;

		actorPtr = &actorTable[currentDrawActor];

		//if(actorPtr->flags & 0x25)
		{
			actorPtr->flags &= 0xFFFB;

			if(actorPtr->flags & 0x20) {
				mainDrawSub2(currentDrawActor);
			} else {
				char *bodyPtr = listBody->get(actorPtr->bodyNum);

				if(listBody->getVar1()) {
					//          initAnimInBody(actorPtr->FRAME, HQR_Get(listAnim, actorPtr->ANIM), bodyPtr);
				}

				renderModel(actorPtr->worldX + actorPtr->modX, actorPtr->worldY + actorPtr->modY, actorPtr->worldZ + actorPtr->modZ,
				            actorPtr->alpha, actorPtr->beta, actorPtr->gamma, bodyPtr);


				if(actorPtr->animActionType && actorPtr->field_98 != -1) {
					getHotPoint(actorPtr->field_98, bodyPtr, &actorPtr->hotPoint);
				}

				///////////////////////////////////// DEBUG
#ifdef INTERNAL_DEBUGGER
				//  if(debuggerVar_drawModelZv)
				{
					if(backgroundMode == backgroundModeEnum_3D) {
						drawZv(actorPtr);
					}
				}
#endif
				/////////////////////////////////////
			}

			if(BBox3D1 < 0)
				BBox3D1 = 0;
			if(BBox3D3 > 319)
				BBox3D3 = 319;
			if(BBox3D2 < 0)
				BBox3D2 = 0;
			if(BBox3D4 > 199)
				BBox3D4 = 199;

			if(BBox3D1 <= 319 && BBox3D2 <= 199 && BBox3D3 >= 0 && BBox3D4 >= 0) { // is the character on screen ?
				if(g_fitd->getGameType() == GType_AITD1) {
					if(actorPtr->field_0 == CVars[getCVarsIdx(LIGHT_OBJECT)]) {
						mainVar3 = (BBox3D3 + BBox3D1) / 2;
						mainVar2 = (BBox3D4 + BBox3D2) / 2;
					}
				}

#ifdef INTERNAL_DEBUGGER
				if(backgroundMode == backgroundModeEnum_2D)
#endif
				{
					if(g_fitd->getGameType() == GType_AITD1)
						drawBgOverlay(actorPtr);
				}
				//addToRedrawBox();
			} else {
				actorPtr->field_1A = -1;
				actorPtr->field_18 = -1;
				actorPtr->field_16 = -1;
				actorPtr->field_14 = -1;
			}
		}
	}

#ifdef USE_GL
	g_driver->stopModelRender();
#endif

	if(drawTextOverlay()) {
		//addToRedrawBox();
	}

	if(!lightVar1) {
		if(mode) {
			if(mode != 2 || lightVar2) {
				//makeBlackPalette();
				flipScreen();
				make3dTatouUnk1(0x10, 0);
				lightVar2 = 0;
			} else {
				//flipScreen();
			}
		} else {
			//mainDrawSub1();
		}
	} else {
	}

#ifdef INTERNAL_DEBUGGER
	debugger_draw();
#endif

#ifdef USE_GL
	g_driver->stopFrame();
#endif

	flipScreen();
}

#ifdef INTERNAL_DEBUGGER
void line(int x1, int y1, int x2, int y2, char c);

#ifdef USE_GL
void drawProjectedLine(int32 x1s, int32 y1s, int32 z1s, int32 x2s, int32 y2s, int32 z2s, int c)
#else
void drawProjectedLine(int x1, int y1, int z1, int x2, int y2, int z2, int c)
#endif
{
#ifdef USE_GL
	float x1 = (float)x1s;
	float x2 = (float)x2s;
	float y1 = (float)y1s;
	float y2 = (float)y2s;
	float z1 = (float)z1s;
	float z2 = (float)z2s;

	float transformedX1;
	float transformedX2;

	float transformedY1;
	float transformedY2;
#else
	int transformedX1;
	int transformedX2;

	int transformedY1;
	int transformedY2;
#endif


	x1 -= translateX;
	x2 -= translateX;

	y1 -= translateY;
	y2 -= translateY;

	z1 -= translateZ;
	z2 -= translateZ;

	transformPoint(&x1, &y1, &z1);
	transformPoint(&x2, &y2, &z2);

	z1 += cameraX;
	z2 += cameraX;

#ifdef USE_GL
	transformedX1 = ((x1 * cameraY) / (float)z1) + cameraCenterX;
	transformedX2 = ((x2 * cameraY) / (float)z2) + cameraCenterX;

	transformedY1 = ((y1 * cameraZ) / (float)z1) + cameraCenterY;
	transformedY2 = ((y2 * cameraZ) / (float)z2) + cameraCenterY;
#else
	transformedX1 = ((x1 * cameraY) / z1) + cameraCenterX;
	transformedX2 = ((x2 * cameraY) / z2) + cameraCenterX;

	transformedY1 = ((y1 * cameraZ) / z1) + cameraCenterY;
	transformedY2 = ((y2 * cameraZ) / z2) + cameraCenterY;
#endif

#ifdef USE_GL
	if(z1 > 0 && z2 > 0)
		g_driver->draw3dLine(transformedX1, transformedY1, z1, transformedX2, transformedY2, z2, c);
#else
	if(z1 > 0 && z2 > 0)
		line(transformedX1, transformedY1, transformedX2, transformedY2, c);
#endif
}

void drawZv(actorStruct *actorPtr) {
	ZVStruct localZv;

	if(actorPtr->room != actorTable[genVar9].room) {
		getZvRelativePosition(&localZv, actorPtr->room, actorTable[genVar9].room);
	} else {
		copyZv(&actorPtr->zv, &localZv);
	}


	// bottom
	drawProjectedLine(localZv.ZVX1,
	                  localZv.ZVY2,
	                  localZv.ZVZ1,
	                  localZv.ZVX1,
	                  localZv.ZVY2,
	                  localZv.ZVZ2,
	                  10);

	drawProjectedLine(localZv.ZVX1, localZv.ZVY2, localZv.ZVZ2, localZv.ZVX2, localZv.ZVY2, localZv.ZVZ2, 10);
	drawProjectedLine(localZv.ZVX2, localZv.ZVY2, localZv.ZVZ2, localZv.ZVX2, localZv.ZVY2, localZv.ZVZ1, 10);
	drawProjectedLine(localZv.ZVX2, localZv.ZVY2, localZv.ZVZ1, localZv.ZVX1, localZv.ZVY2, localZv.ZVZ1, 10);

	// top
	drawProjectedLine(localZv.ZVX1, localZv.ZVY1, localZv.ZVZ1, localZv.ZVX1, localZv.ZVY1, localZv.ZVZ2, 10);
	drawProjectedLine(localZv.ZVX1, localZv.ZVY1, localZv.ZVZ2, localZv.ZVX2, localZv.ZVY1, localZv.ZVZ2, 10);
	drawProjectedLine(localZv.ZVX2, localZv.ZVY1, localZv.ZVZ2, localZv.ZVX2, localZv.ZVY1, localZv.ZVZ1, 10);
	drawProjectedLine(localZv.ZVX2, localZv.ZVY1, localZv.ZVZ1, localZv.ZVX1, localZv.ZVY1, localZv.ZVZ1, 10);

	drawProjectedLine(localZv.ZVX1, localZv.ZVY2, localZv.ZVZ1, localZv.ZVX1, localZv.ZVY1, localZv.ZVZ1, 10);
	drawProjectedLine(localZv.ZVX1, localZv.ZVY2, localZv.ZVZ2, localZv.ZVX1, localZv.ZVY1, localZv.ZVZ2, 10);
	drawProjectedLine(localZv.ZVX2, localZv.ZVY2, localZv.ZVZ2, localZv.ZVX2, localZv.ZVY1, localZv.ZVZ2, 10);
	drawProjectedLine(localZv.ZVX2, localZv.ZVY2, localZv.ZVZ1, localZv.ZVX2, localZv.ZVY1, localZv.ZVZ1, 10);


}

void drawConverZone(cameraZoneEntryStruct *zonePtr) {
	int i;

	for(i = 0; i < zonePtr->numPoints - 1; i++) {
		drawProjectedLine(zonePtr->pointTable[i].x * 10, 0, zonePtr->pointTable[i].y * 10, zonePtr->pointTable[i+1].x * 10, 0, zonePtr->pointTable[i+1].y * 10, 20);
	}

	// loop first and last

	i = zonePtr->numPoints - 1;
	drawProjectedLine(zonePtr->pointTable[0].x * 10, 0, zonePtr->pointTable[0].y * 10, zonePtr->pointTable[i].x * 10, 0, zonePtr->pointTable[i].y * 10, 20);
}

void drawConverZones() {
	int i;
	for(i = 0; i < numCameraInRoom; i++) {
		int j;
		for(j = 0; j < cameraDataTable[i]->_numCameraZoneDef; j++) {
			int k;

			if(cameraDataTable[i]->_cameraZoneDefTable[j].dummy1 == currentDisplayedRoom) {
				for(k = 0; k < cameraDataTable[i]->_cameraZoneDefTable[j].numZones; k++) {
					drawConverZone(&cameraDataTable[i]->_cameraZoneDefTable[j].cameraZoneEntryTable[k]);
				}
			}
		}
	}
}

#ifdef USE_GL

#define DEPTH_THRESHOLD 1000

void drawProjectedQuad(float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, float z1, float z2, float z3, float z4, int color, int transprency) {
	float transformedX1;
	float transformedX2;
	float transformedX3;
	float transformedX4;

	float transformedY1;
	float transformedY2;
	float transformedY3;
	float transformedY4;

	x1 -= translateX;
	x2 -= translateX;
	x3 -= translateX;
	x4 -= translateX;

	y1 -= translateY;
	y2 -= translateY;
	y3 -= translateY;
	y4 -= translateY;

	z1 -= translateZ;
	z2 -= translateZ;
	z3 -= translateZ;
	z4 -= translateZ;

	transformPoint(&x1, &y1, &z1);
	transformPoint(&x2, &y2, &z2);
	transformPoint(&x3, &y3, &z3);
	transformPoint(&x4, &y4, &z4);

	z1 += cameraX;
	z2 += cameraX;
	z3 += cameraX;
	z4 += cameraX;

	transformedX1 = ((x1 * cameraY) / (float)z1) + cameraCenterX;
	transformedX2 = ((x2 * cameraY) / (float)z2) + cameraCenterX;
	transformedX3 = ((x3 * cameraY) / (float)z3) + cameraCenterX;
	transformedX4 = ((x4 * cameraY) / (float)z4) + cameraCenterX;

	transformedY1 = ((y1 * cameraZ) / (float)z1) + cameraCenterY;
	transformedY2 = ((y2 * cameraZ) / (float)z2) + cameraCenterY;
	transformedY3 = ((y3 * cameraZ) / (float)z3) + cameraCenterY;
	transformedY4 = ((y4 * cameraZ) / (float)z4) + cameraCenterY;

	if(z1 > DEPTH_THRESHOLD && z2 > DEPTH_THRESHOLD && z3 > DEPTH_THRESHOLD && z4 > DEPTH_THRESHOLD) {
		g_driver->draw3dQuad(transformedX1, transformedY1, z1, transformedX2, transformedY2, z2, transformedX3, transformedY3, z3, transformedX4, transformedY4, z4, color, transprency);
	}

	//g_driver->draw3dQuad(x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4, color);
}

void drawProjectedBox(int x1, int x2, int y1, int y2, int z1, int z2, int color, int transprency) {
	//bottom
	drawProjectedQuad((float)x1, (float)x1, (float)x2, (float)x2, (float)y1, (float)y1, (float)y1, (float)y1, (float)z1, (float)z2, (float)z2, (float)z1, color, transprency);
	//top
	drawProjectedQuad((float)x1, (float)x1, (float)x2, (float)x2, (float)y2, (float)y2, (float)y2, (float)y2, (float)z1, (float)z2, (float)z2, (float)z1, color, transprency);
	//left
	drawProjectedQuad((float)x1, (float)x1, (float)x1, (float)x1, (float)y1, (float)y2, (float)y2, (float)y1, (float)z1, (float)z1, (float)z2, (float)z2, color, transprency);
	//right
	drawProjectedQuad((float)x2, (float)x2, (float)x2, (float)x2, (float)y1, (float)y2, (float)y2, (float)y1, (float)z1, (float)z1, (float)z2, (float)z2, color, transprency);
	//front
	drawProjectedQuad((float)x1, (float)x2, (float)x2, (float)x1, (float)y1, (float)y1, (float)y2, (float)y2, (float)z1, (float)z1, (float)z1, (float)z1, color, transprency);
	//back
	drawProjectedQuad((float)x1, (float)x2, (float)x2, (float)x1, (float)y1, (float)y1, (float)y2, (float)y2, (float)z2, (float)z2, (float)z2, (float)z2, color, transprency);
}
#endif

void drawRoomZv(ZVStruct *zoneData, int color, int transparency) {
	ZVStruct cameraZv = { -100, 100, -100, 100, -100, 100};

	cameraZv.ZVX1 += translateX;
	cameraZv.ZVX2 += translateX;

	cameraZv.ZVY1 += translateY;
	cameraZv.ZVY2 += translateY;

	cameraZv.ZVZ1 += translateZ;
	cameraZv.ZVZ2 += translateZ;

	if(checkZvCollision(&cameraZv, zoneData)) {
		return;
	}

#ifdef USE_GL
	drawProjectedBox(zoneData->ZVX1, zoneData->ZVX2, zoneData->ZVY1, zoneData->ZVY2, zoneData->ZVZ1, zoneData->ZVZ2, color, transparency);
#else
	drawProjectedLine(x1, y1, z1, x1, y1, z2, color);
	drawProjectedLine(x1, y1, z2, x2, y1, z2, color);
	drawProjectedLine(x2, y1, z2, x2, y1, z1, color);
	drawProjectedLine(x2, y1, z1, x1, y1, z1, color);

	drawProjectedLine(x1, y2, z1, x1, y2, z2, color);
	drawProjectedLine(x1, y2, z2, x2, y2, z2, color);
	drawProjectedLine(x2, y2, z2, x2, y2, z1, color);
	drawProjectedLine(x2, y2, z1, x1, y2, z1, color);

	drawProjectedLine(x1, y1, z1, x1, y2, z1, color);
	drawProjectedLine(x1, y1, z2, x1, y2, z2, color);
	drawProjectedLine(x2, y1, z2, x2, y2, z2, color);
	drawProjectedLine(x2, y1, z1, x2, y2, z1, color);
#endif
}

void drawRoomZvLine(ZVStruct *zoneData, int color) {
	ZVStruct cameraZv = { -100, 100, -100, 100, -100, 100};

	cameraZv.ZVX1 += translateX;
	cameraZv.ZVX2 += translateX;

	cameraZv.ZVY1 += translateY;
	cameraZv.ZVY2 += translateY;

	cameraZv.ZVZ1 += translateZ;
	cameraZv.ZVZ2 += translateZ;

	if(checkZvCollision(&cameraZv, zoneData)) {
		return;
	}

	drawProjectedLine(zoneData->ZVX1, zoneData->ZVY1, zoneData->ZVZ1, zoneData->ZVX1, zoneData->ZVY1, zoneData->ZVZ2, color);
	drawProjectedLine(zoneData->ZVX1, zoneData->ZVY1, zoneData->ZVZ2, zoneData->ZVX2, zoneData->ZVY1, zoneData->ZVZ2, color);
	drawProjectedLine(zoneData->ZVX2, zoneData->ZVY1, zoneData->ZVZ2, zoneData->ZVX2, zoneData->ZVY1, zoneData->ZVZ1, color);
	drawProjectedLine(zoneData->ZVX2, zoneData->ZVY1, zoneData->ZVZ1, zoneData->ZVX1, zoneData->ZVY1, zoneData->ZVZ1, color);

	drawProjectedLine(zoneData->ZVX1, zoneData->ZVY2, zoneData->ZVZ1, zoneData->ZVX1, zoneData->ZVY2, zoneData->ZVZ2, color);
	drawProjectedLine(zoneData->ZVX1, zoneData->ZVY2, zoneData->ZVZ2, zoneData->ZVX2, zoneData->ZVY2, zoneData->ZVZ2, color);
	drawProjectedLine(zoneData->ZVX2, zoneData->ZVY2, zoneData->ZVZ2, zoneData->ZVX2, zoneData->ZVY2, zoneData->ZVZ1, color);
	drawProjectedLine(zoneData->ZVX2, zoneData->ZVY2, zoneData->ZVZ1, zoneData->ZVX1, zoneData->ZVY2, zoneData->ZVZ1, color);

	drawProjectedLine(zoneData->ZVX1, zoneData->ZVY1, zoneData->ZVZ1, zoneData->ZVX1, zoneData->ZVY2, zoneData->ZVZ1, color);
	drawProjectedLine(zoneData->ZVX1, zoneData->ZVY1, zoneData->ZVZ2, zoneData->ZVX1, zoneData->ZVY2, zoneData->ZVZ2, color);
	drawProjectedLine(zoneData->ZVX2, zoneData->ZVY1, zoneData->ZVZ2, zoneData->ZVX2, zoneData->ZVY2, zoneData->ZVZ2, color);
	drawProjectedLine(zoneData->ZVX2, zoneData->ZVY1, zoneData->ZVZ1, zoneData->ZVX2, zoneData->ZVY2, zoneData->ZVZ1, color);
}

void drawZone(char *zoneData, int color) {
	int x1;
	int x2;

	int y1;
	int y2;

	int z1;
	int z2;

	int type;

	ZVStruct tempZv;

	ZVStruct cameraZv = { -100, 100, -100, 100, -100, 100};

	type = *(int16 *)(zoneData + 0xE);

	x1 = *(int16 *)(zoneData + 0x0);
	x2 = *(int16 *)(zoneData + 0x2);
	y1 = *(int16 *)(zoneData + 0x4);
	y2 = *(int16 *)(zoneData + 0x6);
	z1 = *(int16 *)(zoneData + 0x8);
	z2 = *(int16 *)(zoneData + 0xA);

	cameraZv.ZVX1 += translateX;
	cameraZv.ZVX2 += translateX;

	cameraZv.ZVY1 += translateY;
	cameraZv.ZVY2 += translateY;

	cameraZv.ZVZ1 += translateZ;
	cameraZv.ZVZ2 += translateZ;

	tempZv.ZVX1 = (int16)READ_LE_UINT16(zoneData + 0x00);
	tempZv.ZVX2 = (int16)READ_LE_UINT16(zoneData + 0x02);
	tempZv.ZVY1 = (int16)READ_LE_UINT16(zoneData + 0x04);
	tempZv.ZVY2 = (int16)READ_LE_UINT16(zoneData + 0x06);
	tempZv.ZVZ1 = (int16)READ_LE_UINT16(zoneData + 0x08);
	tempZv.ZVZ2 = (int16)READ_LE_UINT16(zoneData + 0x0A);

	if(checkZvCollision(&cameraZv, &tempZv)) {
		return;
	}

#ifdef USE_GL
	drawProjectedBox(x1, x2, y1, y2, z1, z2, type, 255);
#else
	drawProjectedLine(x1, y1, z1, x1, y1, z2, type);
	drawProjectedLine(x1, y1, z2, x2, y1, z2, type);
	drawProjectedLine(x2, y1, z2, x2, y1, z1, type);
	drawProjectedLine(x2, y1, z1, x1, y1, z1, type);

	drawProjectedLine(x1, y2, z1, x1, y2, z2, type);
	drawProjectedLine(x1, y2, z2, x2, y2, z2, type);
	drawProjectedLine(x2, y2, z2, x2, y2, z1, type);
	drawProjectedLine(x2, y2, z1, x1, y2, z1, type);

	drawProjectedLine(x1, y1, z1, x1, y2, z1, type);
	drawProjectedLine(x1, y1, z2, x1, y2, z2, type);
	drawProjectedLine(x2, y1, z2, x2, y2, z2, type);
	drawProjectedLine(x2, y1, z1, x2, y2, z1, type);
#endif
}

void drawOverlayZone(char *zoneData, int color) {
	int x1;
	int x2;

	int y1;
	int y2;

	int z1;
	int z2;

	x1 = *(int16 *)(zoneData + 0x0) * 10;
	z1 = *(int16 *)(zoneData + 0x2) * 10;
	x2 = *(int16 *)(zoneData + 0x4) * 10;
	z2 = *(int16 *)(zoneData + 0x6) * 10;

	y1 = 0;
	y2 = 0;

#ifdef USE_GL
	drawProjectedBox(x1, x2, y1, y2, z1, z2, color, 255);
#else
	drawProjectedLine(x1, y1, z1, x1, y1, z2, color);
	drawProjectedLine(x1, y1, z2, x2, y1, z2, color);
	drawProjectedLine(x2, y1, z2, x2, y1, z1, color);
	drawProjectedLine(x2, y1, z1, x1, y1, z1, color);

	drawProjectedLine(x1, y2, z1, x1, y2, z2, color);
	drawProjectedLine(x1, y2, z2, x2, y2, z2, color);
	drawProjectedLine(x2, y2, z2, x2, y2, z1, color);
	drawProjectedLine(x2, y2, z1, x1, y2, z1, color);

	drawProjectedLine(x1, y1, z1, x1, y2, z1, color);
	drawProjectedLine(x1, y1, z2, x1, y2, z2, color);
	drawProjectedLine(x2, y1, z2, x2, y2, z2, color);
	drawProjectedLine(x2, y1, z1, x2, y2, z1, color);
#endif
}

void drawSceZone(int roomNumber) {
	uint32 i;
	ZVStruct dataLocal;

	for(i = 0; i < roomDataTable[roomNumber].numSceZone; i++) {
		memcpy(&dataLocal, &roomDataTable[roomNumber].sceZoneTable[i].zv, sizeof(ZVStruct));
		if(roomNumber != currentDisplayedRoom) {
			getZvRelativePosition(&dataLocal, roomNumber, currentDisplayedRoom);
		}

		if(roomDataTable[roomNumber].sceZoneTable[i].parameter == 4)
			if(roomDataTable[roomNumber].sceZoneTable[i].type) {
				drawRoomZv(&dataLocal, 20, 40);
			}
	}
}

void drawHardCol(int roomNumber) {
	uint32 i;
	ZVStruct dataLocal;

	for(i = 0; i < roomDataTable[roomNumber].numHardCol; i++) {
		copyZv(&roomDataTable[roomNumber].hardColTable[i].zv, &dataLocal);

		if(roomNumber != currentDisplayedRoom) {
			getZvRelativePosition(&dataLocal, roomNumber, currentDisplayedRoom);
		}

		switch(roomDataTable[roomNumber].hardColTable[i].type) {
		case 0: // objects 1
			drawRoomZv(&dataLocal, 9, 150);
			break;
		case 1: // walls
			drawRoomZv(&dataLocal, 180, 255);
			break;
		case 9: // objects 2
			drawRoomZv(&dataLocal, 5, 255);
			break;
		default:
			drawRoomZv(&dataLocal, 40, 40);
			break;
		}

	}
}
#endif

} // end of Namespace
