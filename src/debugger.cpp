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

#include "common.h"

#ifdef INTERNAL_DEBUGGER

typedef struct
{
  u32 X;
  u32 Y;
  u8 name[40];
  u8* returnVar;
} debugger_buttonEntry;

bool debuggerVar_debugMenuDisplayed = false;

////// debug var used in engine
bool debuggerVar_drawModelZv = false;
bool debuggerVar_drawCameraCoverZone = false;
bool debuggerVar_noHardClip = false;
bool debuggerVar_topCamera = false;
long int debufferVar_topCameraZoom = -4000;

bool debuggerVar_useBlackBG = false;
///////////////////////////////

u8 debugger_mainDebugButtonVar_toggleDrawModelZv = 0;
u8 debugger_mainDebugButtonVar_toggleDrawCameraCoverZone = 0;

const debugger_buttonEntry debugger_mainDebugButtonTable[]=
{
  {0,0,"Draw model ZV",&debugger_mainDebugButtonVar_toggleDrawModelZv},
  {0,8,"Draw camera cover zone",&debugger_mainDebugButtonVar_toggleDrawCameraCoverZone},
  {0,0,"",NULL},
};

const debugger_buttonEntry* debugger_currentMenu;

void debugger_processDebugMenu(void)
{
}

void debugger_enterMainDebug(void)
{
  debuggerVar_debugMenuDisplayed = true;

  debugger_currentMenu = debugger_mainDebugButtonTable;
}

void debugger_drawMenuEntry(const debugger_buttonEntry* menuEntry)
{
  osystem_drawDebugText(menuEntry->X,menuEntry->Y,menuEntry->name);
}

void debugger_draw(void)
{
  if(debuggerVar_debugMenuDisplayed)
  {
    u32 numEntryInMenu=0;
    u32 currentMenuEntry;

    while(debugger_currentMenu[numEntryInMenu].name[0] != 0)
    {
      numEntryInMenu++;
    }

    for(currentMenuEntry=0; currentMenuEntry<numEntryInMenu; currentMenuEntry++)
    {
      debugger_drawMenuEntry(&debugger_currentMenu[currentMenuEntry]);
    }
  }
}

#endif
