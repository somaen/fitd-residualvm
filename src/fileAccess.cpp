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
// seg 20
void theEnd(int type, char* name)
{
	//  freeScene();
	freeAll();
	printf("Error: %s\n", name);
	exit(1);
}
char* loadFromItd(char* name)
{
	FILE* fHandle;
	char* ptr;
	fHandle = fopen(name,"rb");
	if(!fHandle)
	{
		theEnd(0,name);
		return NULL;
	}
	fseek(fHandle,0,SEEK_END);
	fileSize = ftell(fHandle);
	fseek(fHandle,0,SEEK_SET);
	ptr = (char*)malloc(fileSize);
	
	if(!ptr)
	{
		theEnd(1,name);
		return NULL;
	}
	fread(ptr,fileSize,1,fHandle);
	fclose(fHandle);
	return(ptr);
}

char* loadPakSafe(char* name, int index)
{
	char* ptr;
	ptr = loadPak(name, index);
	if(!ptr)
	{
		theEnd(0,name);
	}
	return ptr;
}
