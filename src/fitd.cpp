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

#include "common/textconsole.h"
#include "fitd.h"
#include "resource.h"
#include "osystem.h"

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

FitdEngine::FitdEngine() {
	g_resourceLoader = new ResourceLoader();
	g_driver = new GFXSystem();

	g_driver->init();
	g_driver->initBuffer(scaledScreen, 640, 400);
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

		printf("Detected Alone in the Dark 1\n");
		return;
	}
	if(g_resourceLoader->getFileExists("PERE.PAK")) {
		_gameType = GType_JACK;
		_numCVars = 70;
		_currentCVarTable = AITD2KnownCVars;

		printf("Detected Jack in the Dark\n");
		return;
	}
	if(g_resourceLoader->getFileExists("MER.PAK")) {
		_gameType = GType_AITD2;
		_numCVars = 70;
		_currentCVarTable = AITD2KnownCVars;

		printf("Detected Alone in the Dark 2\n");
		return;
	}
	if(g_resourceLoader->getFileExists("AN1.PAK")) {
		_gameType = GType_AITD3;
		_numCVars = 70;
		_currentCVarTable = AITD2KnownCVars;

		printf("Detected Alone in the Dark 3\n");
		return;
	}
	if(g_resourceLoader->getFileExists("PURSUIT.PAK")) {
		_gameType = GType_TIMEGATE;
		_numCVars = 70; // TODO: figure this
		_currentCVarTable = AITD2KnownCVars; // TODO: figure this

		printf("Detected Time Gate\n");
		return;
	}

	printf("FATAL: Game detection failed...\n");
	exit(1);
}

int FitdEngine::getCVarsIdx(enumCVars searchedType) { // TODO: optimize by reversing the table....
	int i;
	
	for(i = 0; i < g_fitd->getNumCVars(); i++) {
		if(_currentCVarTable[i] == -1) {
			ASSERT(0);
		}
		
		
		if(_currentCVarTable[i] == searchedType)
			return i;
	}
}

void FitdEngine::startup() {
	int startupMenuResult;
	paletteFill(palette, 0, 0, 0);
	
	preloadResource();
	
	switch(g_fitd->getGameType()) {
		case GType_AITD1: {
			fadeIn(palette);
			
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
	int i;

#ifndef PCLIKE
	//  unsigned long int ltime;
#else
	//  time_t ltime;
#endif
	FILE *fHandle;

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

	CVars = (short int *)malloc(getNumCVars() * sizeof(short int));

	switch(getGameType()) {
	case GType_JACK:
	case GType_AITD2:
	case GType_AITD3:
	case GType_TIMEGATE: {
		fontData = g_resourceLoader->loadPakSafe("ITD_RESS", 1);
		break;
	}
	case GType_AITD1: {
		fontData = g_resourceLoader->loadPakSafe("ITD_RESS", 5);
		break;
	}
	}

	initFont(fontData, 14);

	if(getGameType() == GType_AITD1) {
		initFont2(2, 0);
	} else {
		initFont2(2, 1);
	}

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

	fHandle = fopen("DEFINES.ITD", "rb");
	if(!fHandle) {
		theEnd(0, "DEFINES.ITD");
	}

	///////////////////////////////////////////////
	{
		fread(CVars, getNumCVars(), 2, fHandle);
		fclose(fHandle);

		for(int i = 0; i < getNumCVars(); i++) {
			CVars[i] = ((CVars[i] & 0xFF) << 8) | ((CVars[i] & 0xFF00) >> 8);
		}
	}
	//////////////////////////////////////////////

	allocTextes();

	//  if(musicConfigured)
	{
		listMus = HQR_InitRessource("ListMus", 110000, 40);
	}

	listSamp = HQR_InitRessource("ListSamp", 64000, 30);

	hqrUnk = HQR_Init(10000, 50);
}

void FitdEngine::preloadResource() {
	char localPalette[768];
	
	if(g_fitd->getGameType() == GType_AITD2) {
		loadPakToPtr("ITD_RESS", 59, aux);
	} else {
		loadPakToPtr("ITD_RESS", 3, aux);
	}
	copyPalette(aux, palette);
	
	copyPalette(palette, localPalette);
	//  fadeInSub1(localPalette);
	
	// to finish
}

void delay(uint32 ms) {
	SDL_Delay(ms);
}

int32 getTicks() {
	return SDL_GetTicks();
}

} // end of namespace Fitd
