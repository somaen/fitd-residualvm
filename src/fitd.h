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

#ifndef FITD_FITD_H
#define FITD_FITD_H

#define FORBIDDEN_SYMBOL_EXCEPTION_rand

#include <cstdio>
#include <cstdlib>
#include "cvars.h"
#include "common.h"

namespace Fitd {

enum gameType {
	GType_AITD1 = 1,
	GType_JACK = 2,
	GType_AITD2 = 3,
	GType_AITD3 = 4,
	GType_TIMEGATE = 5,
};

void allocTextes(void);
void sysInitSub1(char *var0, char *var1);

class FitdEngine {
	gameType _gameType;
	enumCVars *_currentCVarTable;
	uint32 _numCVars;

	void sysInit();
	void detectGame();
public:
	FitdEngine();
	void run();
	void startup();
	void preloadResource();
	uint32 getGameType() {
		return _gameType;
	}
	int getNumCVars() {
		return _numCVars;
	}
	uint32 randRange(int min, int max) {
		return((rand() % (max - min)) + min);
	}
	int32 getCVarsIdx(enumCVars searchedType);
	void delay(uint32 ms);
	int32 getTicks();
};

extern FitdEngine *g_fitd;

} // end of namespace Fitd

#endif
