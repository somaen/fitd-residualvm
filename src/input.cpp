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
#include <SDL.h>

extern float nearVal;
extern float farVal;
extern float cameraZoom;
extern float fov;

void readKeyboard(void)
{
	SDL_Event event;
	int size;
	int j;
	unsigned char *keyboard;
	
	inputKey = 0;
	input1 = 0;
	input2 = 0;
	
	while( SDL_PollEvent( &event ) ) {
		
		switch( event.type ) {
			case SDL_QUIT:
				cleanupAndExit();
				break;
		}
		
	}
	
	keyboard = SDL_GetKeyState(&size);
	
	for (j = 0; j < size; j++)
	{
		if (keyboard[j])
		{
			switch (j)
			{
					/*        case SDLK_z:
					 nearVal-=1;
					 break;
					 case SDLK_x:
					 nearVal+=1;
					 break;
					 case SDLK_a:
					 cameraZoom-=100;
					 break;
					 case SDLK_s:
					 cameraZoom+=100;
					 break;
					 case SDLK_w:
					 {
					 float factor = (float)cameraY/(float)cameraZ;
					 cameraY+=1;
					 cameraZ=(float)cameraY/factor;
					 break;
					 }
					 case SDLK_q:
					 {
					 float factor = (float)cameraY/(float)cameraZ;
					 cameraY-=1;
					 cameraZ=(float)cameraY/factor;
					 break;
					 }
					 case SDLK_e:
					 fov-=1;
					 break;
					 case SDLK_r:
					 fov+=2;
					 break; */
				case SDLK_RETURN:
					input2 = 0x1C;
					break;
				case SDLK_ESCAPE:
					input2 = 0x1B;
					break;
					
				case SDLK_UP:
					inputKey |= 1;
					break;
					
				case SDLK_DOWN:
					inputKey |= 2;
					break;
					
				case SDLK_RIGHT:
					inputKey |= 8;
					break;
					
				case SDLK_LEFT:
					inputKey |= 4;
					break;
				case SDLK_SPACE:
					input1 = 1;
					break;
#ifdef INTERNAL_DEBUGGER
				case SDLK_o:
					debufferVar_topCameraZoom+=100;
					break;
				case SDLK_p:
					debufferVar_topCameraZoom-=100;
					break;
				case SDLK_t:
					debuggerVar_topCamera = !debuggerVar_topCamera;
					
					if(!debuggerVar_topCamera)
					{
						backgroundMode = backgroundModeEnum_2D;
					}
					break;
				case SDLK_c:
					debuggerVar_noHardClip = !debuggerVar_noHardClip;
					break;
				case SDLK_d:
					debugger_enterMainDebug();
					break;
				case SDLK_b:
					backgroundMode = backgroundModeEnum_3D;
					break;
				case SDLK_n:
					backgroundMode = backgroundModeEnum_2D;
					break;
#endif
			}
		}
	}
#ifdef INTERNAL_DEBUGGER
	if(debuggerVar_topCamera)
	{
		backgroundMode = backgroundModeEnum_3D;
	}
#endif
}
