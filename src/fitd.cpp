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

#define FORBIDDEN_SYMBOL_EXCEPTION_srand
#define FORBIDDEN_SYMBOL_EXCEPTION_rand
#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include <ctime>
#include "SDL.h"
#include "common/textconsole.h"
#include "fitd.h"
#include "resource.h"
#include "osystem.h"
#include "common.h"
#include "textes.h"
#include "main_loop.h"
#include "gfx.h"

namespace Fitd {
	
enumCVars AITD1KnownCVars[] = {
	SAMPLE_PAGE,
	BODY_FLAMME,
	MAX_WEIGHT_LOADABLE,
	TEXTE_CREDITS,
	SAMPLE_TONNERRE,
	INTRO_DETECTIVE,
	INTRO_HERITIERE,
	WORLD_NUM_PERSO,
	CHOOSE_PERSO,
	SAMPLE_CHOC,
	SAMPLE_PLOUF,
	REVERSE_OBJECT,
	KILLED_SORCERER,
	LIGHT_OBJECT,
	FOG_FLAG,
	DEAD_PERSO,
	UNKNOWN_CVAR
};

enumCVars AITD2KnownCVars[] = {
	SAMPLE_PAGE,
	BODY_FLAMME,
	MAX_WEIGHT_LOADABLE,
	SAMPLE_CHOC,
	DEAD_PERSO,
	JET_SARBACANE,
	TIR_CANON,
	JET_SCALPEL,
	POIVRE,
	DORTOIR,
	EXT_JACK,
	NUM_MATRICE_PROTECT_1,
	NUM_MATRICE_PROTECT_2,
	NUM_PERSO,
	TYPE_INVENTAIRE,
	PROLOGUE,
	POIGNARD,
	UNKNOWN_CVAR
};

// TODO: Move this around to a usefull place:
void makeIntroScreens();
void sysInitSub1(char *var0, char *var1);

FitdEngine::FitdEngine() {
	g_resourceLoader = new ResourceLoader();
	g_driver = new GFXSystem();

	g_driver->init();
	g_driver->initBuffer(scaledScreen, 640, 400);
}

FitdEngine::~FitdEngine() {
	delete _listMus;
	delete _listSamp;
}

void FitdEngine::run() {
	detectGame();
	sysInit();
	startup();
}

void FitdEngine::detectGame(void) {
	if(g_resourceLoader->getFileExists("LISTBOD2.PAK")) {
		_gameType = GType_AITD1;
		_numCVars = 45;
		_currentCVarTable = AITD1KnownCVars;

		warning("Detected Alone in the Dark 1\n");
		return;
	} else {
		_numCVars = 70;
		_currentCVarTable = AITD2KnownCVars;
		
		if(g_resourceLoader->getFileExists("PERE.PAK")) {
			_gameType = GType_JACK;
			warning("Detected Jack in the Dark\n");
			return;
		}
		if(g_resourceLoader->getFileExists("MER.PAK")) {
			_gameType = GType_AITD2;
			warning("Detected Alone in the Dark 2\n");
			return;
		}
		if(g_resourceLoader->getFileExists("AN1.PAK")) {
			_gameType = GType_AITD3;
			warning("Detected Alone in the Dark 3\n");
			return;
		}
		if(g_resourceLoader->getFileExists("PURSUIT.PAK")) {
			_gameType = GType_TIMEGATE;
			_numCVars = 70; // TODO: figure this
			_currentCVarTable = AITD2KnownCVars; // TODO: figure this
			
			warning("Detected Time Gate\n");
			return;
		}
		
		error("FATAL: Game detection failed...\n");
	}
}

int FitdEngine::getCVarsIdx(enumCVars searchedType) { // TODO: optimize by reversing the table....
	for(int i = 0; i < g_fitd->getNumCVars(); i++) {
		if(_currentCVarTable[i] == -1) {
			ASSERT(0);
		}
		
		if(_currentCVarTable[i] == searchedType)
			return i;
	}
}

void FitdEngine::startup() {
	int startupMenuResult;
	g_driver->_paletteObj->fill(0, 0, 0);
	
	preloadResource();
	
	switch(g_fitd->getGameType()) {
		case GType_AITD1: {
			fadeIn(g_driver->_palette);
			
			if(!make3dTatou()) {
				makeIntroScreens();
			}
			break;
		}
		case GType_JACK: {
			startGame(16, 1, 1);
			break;
		}
		case GType_AITD2: {
			startGame(8, 0, 0);
			break;
		}
		case GType_AITD3: {
			startGame(0, 12, 1);
			startGame(0, 0, 1);
			break;
		}
		case GType_TIMEGATE: {
			startGame(0, 0, 1);
			break;
		}
	}
	
	while(1) {
		startupMenuResult = processStartupMenu();
		
		switch(startupMenuResult) {
			case -1: { // timeout
				CVars[getCVarsIdx(CHOOSE_PERSO)] = randRange(0,2);
				/*  startGame(7,1,0);
				
				 if(!make3dTatou())
				 {
				 if(!makeIntroScreens())
				 {
				 makeSlideshow();
				 }
				 } */
				
				break;
			}
			case 0: { // new game
				/*  if(protectionToBeDone)
				 {
				 makeProtection();
				 protectionToBeDone = 0;
				 }*/
				
				//if(selectHero()!=-1)
				{
					readKeyboard();
					while(input2)
						readKeyboard();
					
					if(g_fitd->getGameType() == GType_AITD1) {
						CVars[getCVarsIdx(CHOOSE_PERSO)] = 0;
					}
					
					switch(g_fitd->getGameType()) {
						case GType_JACK: {
							startGame(16, 1, 0);
							break;
						}
						case GType_AITD2: {
							startGame(8, 7, 1);
							break;
						}
						case GType_AITD3: {
							startGame(0, 12, 1);
							break;
						}
						case GType_AITD1: {
							startGame(7, 1, 0);
							
							/*  if(!protectionState)
							 {
							 freeAll();
							 exit(-1);
							 }
							 */
							readKeyboard();
							while(input2)
								readKeyboard();
							
							startGame(0, 0, 1);
							break;
						}
					}
					/*
					 if(giveUp == 0)
					 {
					 freeAll();
					 exit(-1);
					 }*/
				}
				
				break;
			}
			case 1: { // continue
				/*  if(protectionToBeDone)
				 {
				 makeProtection();
				 protectionToBeDone = 0;
				 }*/
				
				if(restoreSave(12, 0)) {
					/*  if(!protectionState)
					 {
					 freeAll();
					 exit(-1);
					 }*/
					
					//          updateShaking();
					
					mainVar1 = 2;
					
					setupCamera();
					
					mainLoop(1);
					
					//          freeScene();
					
					fadeOut(8, 0);
					
					/*  if(giveUp == 0)
					 {
					 freeAll();
					 exit(-1);
					 } */
				}
				
				break;
			}
			case 2: { // exit
				freeAll();
				error("Exiting");
				//exit(-1);
				
				break;
			}
		}
	}
}


void FitdEngine::sysInit(void) {

#ifndef PCLIKE
	//  unsigned long int ltime;
#else
	//  time_t ltime;
#endif

	setupScreen();
	//setupInterrupt();
	//setupInterrupt2();
	//setupInterrupt3();

	//setupVideoMode();

	// time( &ltime );

	srand(time(NULL));

	if(!initMusicDriver()) {
		musicConfigured = 0;
		musicEnabled = 0;
	}

	// TODO: reverse sound init code


	aux = new char [65068];
	if(!aux) {
		theEnd(1, "Aux");
	}

	aux2 = new char[64000];
	if(!aux2) {
		theEnd(1, "Aux2");
	}

	sysInitSub1(aux2, screen);
	/*  sysInitSub2(aux2);
	 sysInitSub3(aux2); */

	bufferAnim = new char[4960];
	if(!bufferAnim) {
		theEnd(1, "BufferAnim");
	}

	CVars = (int16 *)malloc(getNumCVars() * sizeof(int16));

	uint32 initType;
	if (getGameType() == GType_AITD1) {
		fontData = g_resourceLoader->loadPakSafe("ITD_RESS", 5);
		initType = 1;
	} else {
		fontData = g_resourceLoader->loadPakSafe("ITD_RESS", 1);
		initType = 0;
	}

	initFont(fontData, 14);
	initFont2(2, initType);

	switch(getGameType()) {
	case GType_JACK:
	case GType_AITD2:
	case GType_AITD3: {
		aitdBoxGfx = g_resourceLoader->loadPakSafe("ITD_RESS", 0);
		break;
	}
	case GType_AITD1: {
		aitdBoxGfx = g_resourceLoader->loadPakSafe("ITD_RESS", 4);
		break;
	}
	}

	priority = g_resourceLoader->loadFromItd("PRIORITY.ITD");

	Common::SeekableReadStream *stream = g_resourceLoader->getFile("DEFINES.ITD");
	if(!stream) {
		theEnd(0, "DEFINES.ITD");
	}

	///////////////////////////////////////////////
	{
		for (int i = 0; i < getNumCVars(); i++) {
			CVars[i] = stream->readUint16LE(); // This might not be entirely correct.
		}
		//		fread(CVars, getNumCVars(), 2, fHandle);
		delete stream;

		for(int i = 0; i < getNumCVars(); i++) {
			CVars[i] = ((CVars[i] & 0xFF) << 8) | ((CVars[i] & 0xFF00) >> 8);
		}
	}
	//////////////////////////////////////////////

	g_textes = new Textes();

	//  if(musicConfigured)
	{
		_listMus = new hqrEntryStruct("ListMus", 110000, 40);
	}

	_listSamp = new hqrEntryStruct("ListSamp", 64000, 30);

	hqrUnk = new hqrEntryStruct(10000, 50);
}

void FitdEngine::preloadResource() {
	char localPalette[768];
	
	if(g_fitd->getGameType() == GType_AITD2) {
		loadPakToPtr("ITD_RESS", 59, aux);
	} else {
		loadPakToPtr("ITD_RESS", 3, aux);
	}
	copyPalette(aux, g_driver->_palette);
	
	copyPalette(g_driver->_palette, localPalette);
	//  fadeInSub1(localPalette);
	
	// to finish
}

void FitdEngine::delay(uint32 ms) {
	SDL_Delay(ms);
}

int32 FitdEngine::getTicks() {
	return SDL_GetTicks();
}

uint32 FitdEngine::getGameType() {
	return _gameType;
}

int FitdEngine::getNumCVars() {
	return _numCVars;
}

uint32 FitdEngine::randRange(int min, int max) {
	return((rand() % (max - min)) + min);
}
	
} // end of namespace Fitd
