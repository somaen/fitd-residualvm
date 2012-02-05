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

#include "osystem.h"
#include "common.h"

namespace Fitd {

void drawStartupMenu(int selectedEntry) {
	int currentY = 76;
	int currentTextNum = 0;

	drawAITDBox(160, 100, 320, 80);

	while(currentTextNum < 3) {
		if(currentTextNum == selectedEntry) { // hilight selected entry
			fillBox(10, currentY, 309, currentY + 16, 100);
			drawSlectedText(160, currentY, currentTextNum + 11, 15, 4);
		} else {
			drawText(160, currentY, currentTextNum + 11, 4);
		}

		currentY += 16; // next line
		currentTextNum++; // next text
	}
}

int processStartupMenu(void) {
	int currentSelectedEntry = 0;
	unsigned int chrono;
	int selectedEntry = -1;

	flushScreen();

	drawStartupMenu(0);
#ifdef USE_GL
	g_driver->startFrame();
	g_driver->stopFrame();
	g_driver->CopyBlockPhys((unsigned char *)screen, 0, 0, 320, 200);
#endif
	flipScreen();
	make3dTatouUnk1(16, 0);
	startChrono(&chrono);

	while(evalChrono(&chrono) <= 0x10000) { // exit loop only if time out or if choice made
#ifdef USE_GL
		g_driver->CopyBlockPhys((unsigned char *)screen, 0, 0, 320, 200);
		g_driver->startFrame();
#endif

		if(selectedEntry != -1 || evalChrono(&chrono) > 0x10000) {
			break;
		}

		process_events();
		readKeyboard();

		if(inputKey & 1) { // up key
			currentSelectedEntry--;

			if(currentSelectedEntry < 0) {
				currentSelectedEntry = 2;
			}

			drawStartupMenu(currentSelectedEntry);
			flipScreen();
			//      menuWaitVSync();

			startChrono(&chrono);

			while(inputKey) {
				readKeyboard();
			}
		}


		if(inputKey & 2) { // down key
			currentSelectedEntry++;

			if(currentSelectedEntry > 2) {
				currentSelectedEntry = 0;
			}

			drawStartupMenu(currentSelectedEntry);
			//menuWaitVSync();
			flipScreen();

			startChrono(&chrono);

			while(inputKey) {
				readKeyboard();
			}
		}

		if(input2 == 28 || (input2 != 28 && input1 != 0)) { // select current entry
			selectedEntry = currentSelectedEntry;
		}
#ifdef USE_GL
		g_driver->stopFrame();
		flipScreen();
#endif
	}

	if(selectedEntry == 2) { // if exit game, do not fade
		fadeOut(16, 0);
	}

	readKeyboard();
	while(inputKey) {
		readKeyboard();
	}

	return(selectedEntry);
}

} // end of namespace Fitd
