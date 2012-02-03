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

int mainLoopSwitch = 0;

void updatePendingEvents(void) {
	// TODO: miss pending events here

	if(currentMusic != -1) {
		if(currentMusic == -2) {
			if(evalChrono(&musicChrono) > 180) {
				playMusic(nextMusic);
			}
		} else {
			if(fadeMusic(0, 0, 0x10) == -1) {
				currentMusic = -1;

				if(nextMusic != -1) {
					playMusic(nextMusic);
					nextMusic = -1;
				}
			}
		}
	}
}

void mainLoop(int allowSystemMenu) {
#define SPEED 15              /* Ticks per Frame */
#define SLEEP_MIN 20          /* Minimum time a sleep takes, usually 2*GRAN */
#define SLEEP_GRAN 1         /* Granularity of sleep */

	int frames = 0;                 /* Number of frames displayed */
	int32 t_start, t_left;
	uint32 t_end;
	int32 q = 0;                   /* Dummy */

	while(input2 == 0x1B || input2 == 0x1C) {
		readKeyboard();
	}

	//atexit(cleanupAndExit);

	while(1) {
		frames++;
		t_start = SDL_GetTicks();

		timeGlobal++;

		timer = timeGlobal;

		process_events();
		readKeyboard();
		input3 = input2;
		input4 = inputKey;
		button = input1;

		if(input3) {
			if(input3 == 0x1B) {
				while(input2 == 0x1B) {
					readKeyboard();
				}
				processSystemMenu();
				while(input2 == 0x1B || input2 == 0x1C) {
					readKeyboard();
					input3 = input2;
				}
			}

			if(input3 == 0x1C || input3 == 0x17) {
				if(allowSystemMenu == 0) {
					break;
				}

				if(statusScreenAllowed) {
					makeStatusScreen();
				}
			}
		} else {
			//      input5 = 0;
		}

		if(button) {
			if(!allowSystemMenu) {
				break;
			}

			action = 0x2000;
		} else {
			action = 0;
		}

		if(g_fitd->getGameType() == GType_AITD1) {
			updateInHand(inHand);
		} else {
			if(g_fitd->getGameType() == GType_AITD2) // seems to crash in Jack for now
				updateInHand(inHandTable[currentInHand]);
		}

		if(changeFloor == 0) {
			if(g_fitd->getGameType() == GType_AITD1) {
				if(CVars[getCVarsIdx(LIGHT_OBJECT)] == -1) {
					//        mainVar2 = 2000;
					//        mainVar3 = 2000;
				}
			}

			currentProcessedActorPtr = actorTable;

			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < NUM_MAX_ACTOR; currentProcessedActorIdx++) {
				if(currentProcessedActorPtr->field_0 >= 0) {
					currentProcessedActorPtr->COL_BY = -1;
					currentProcessedActorPtr->HIT_BY = -1;
					currentProcessedActorPtr->HIT = -1;
					currentProcessedActorPtr->HARD_DEC = -1;
					currentProcessedActorPtr->HARD_COL = -1;
				}

				currentProcessedActorPtr++;
			}

			currentProcessedActorPtr = actorTable;
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < NUM_MAX_ACTOR; currentProcessedActorIdx++) {
				if(currentProcessedActorPtr->field_0 >= 0) {
					int flag = currentProcessedActorPtr->flags;

					if(flag & 1 || (g_fitd->getGameType() >= GType_AITD2 && flag & 0x200)) {
						processActor1();
					}

					if(flag & 0x40) {
						processActor2();
					}

					if(currentProcessedActorPtr->animActionType) {
						processAnimAction();
					}
				}

				currentProcessedActorPtr++;
			}

			currentProcessedActorPtr = actorTable;
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < NUM_MAX_ACTOR; currentProcessedActorIdx++) {
				if(currentProcessedActorPtr->field_0 >= 0) {
					if(currentProcessedActorPtr->life != -1) {
						switch(g_fitd->getGameType()) {
						case GType_AITD2:
						case GType_AITD3: {
							if(currentProcessedActorPtr->lifeMode & 3)
								if(!(currentProcessedActorPtr->lifeMode & 4))
									processLife(currentProcessedActorPtr->life);
							break;
						}
						case GType_JACK: {
							if(currentProcessedActorPtr->life != -1)
								if(currentProcessedActorPtr->lifeMode != -1)
									processLife(currentProcessedActorPtr->life);
							break;
						}
						case GType_AITD1: {
							if(currentProcessedActorPtr->life != -1)
								if(currentProcessedActorPtr->lifeMode != -1)
									processLife(currentProcessedActorPtr->life);
							break;
						}
						}
					}
				}

				if(changeFloor)
					break;

				currentProcessedActorPtr++;
			}

			if(giveUp)
				break;
		}

		if(changeFloor) {
			loadFloor(newFloor);
		}

#if INTERNAL_DEBUGGER
		if(debuggerVar_topCamera)
			setupCamera();
#endif

		if(needChangeRoom) {
			loadRoom(newRoom);
			setupCamera();
		} else {
			checkIfCameraChangeIsRequired();
			if(g_fitd->getGameType() >= GType_AITD2) {
				int tempCurrentCamera;

				tempCurrentCamera = currentCamera;

				currentCamera = startGameVar1;

				for(currentProcessedActorIdx = 0; currentProcessedActorIdx < NUM_MAX_ACTOR; currentProcessedActorIdx++) {
					if(currentProcessedActorPtr->field_0 >= 0) {
						if(currentProcessedActorPtr->life != -1) {
							if(currentProcessedActorPtr->flags & 0x200) {
								if(currentProcessedActorPtr->lifeMode & 3)
									if(!(currentProcessedActorPtr->lifeMode & 4)) {
										processLife(currentProcessedActorPtr->life);
										actorTurnedToObj = 1;
									}
							}
						}
					}

					if(changeFloor)
						break;
				}

				if(giveUp)
					break;

				currentCamera = tempCurrentCamera;
			}
			if(mainVar1) {
				setupCamera();
			}
		}

		//    if(objModifFlag1)
		{
			updateAllActorAndObjects();
		}

		//    if(actorTurnedToObj)
		{
			createActorList();
		}

		sortActorList();

		//    if(objModifFlag2)
		{
			//      setupCameraSub4();
		}

		//    mainLoopSub1();

		mainDraw(setupCameraVar1);

		//osystem_delay(100);

		updatePendingEvents();

		t_end = t_start + SPEED;
		t_left = t_start - SDL_GetTicks() + SPEED;

		if(t_left > 0) {
			if(t_left > SLEEP_MIN)
				SDL_Delay(t_left - SLEEP_GRAN);
			while(SDL_GetTicks() < t_end) {
				q++;
			};
		} else {
			// printf("CPU to slow by %d ticks/round\n",-t_left);
		};
	}

	//  mainLoopVar1 = 0;
	//  shakingState = 0;

	//  stopShaking();
	//  stopSounds();
}

} // end of namespace Fitd
