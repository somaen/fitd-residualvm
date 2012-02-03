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

#include "SDL.h"
#include "SDL_thread.h"

namespace Fitd {

int musicThread(void *dummy);

int threadTimer(void *test) // AITD time is 50 frames per seconds ie,  a frame every 20 miliseconds
{
    int temp = SDL_GetTicks();
    while (1)
	{
		SDL_Delay(2); // granularity restriction
		if(SDL_GetTicks() - temp >= 15)
        {
			// timeGlobal++;
            temp = SDL_GetTicks();
        }
	}
    return (0);
}

void startThreadTimer()
{
	//   SDL_CreateThread(threadTimer, NULL);
	//   SDL_CreateThread(musicThread, NULL);
}

} // end of namespace Fitd
