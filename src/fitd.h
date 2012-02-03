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

#ifndef FITD_FITD_H
#define FITD_FITD_H

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
	int _numCVars;

	void sysInit();
	void detectGame();
public:
	FitdEngine();
	void run();
	int getGameType() {
		return _gameType;
	}
	int getNumCVars() {
		return _numCVars;
	}
};

extern FitdEngine *g_fitd;

} // end of namespace Fitd

#endif
