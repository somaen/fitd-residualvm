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

#include <cstdio>
#include "resource.h"
#include "pak.h"
#include "file_access.h"
#include "filestream.h"

namespace Fitd {

ResourceLoader *g_resourceLoader;

bool ResourceLoader::getFileExists(const char *name) {
	Common::ReadFileStream file(name);
	return file.isOpen();
}

char *ResourceLoader::loadFromItd(const char *name) {
	Common::ReadFileStream file(name);
	if(!file.isOpen()) {
		theEnd(0, name);
		return NULL;
	}
	int filesize = file.size();
	char *ptr;
	ptr = new char[filesize];

	if(!ptr) {
		theEnd(1, name);
		return NULL;
	}
	file.read(ptr, filesize);
	file.close();
	return ptr;
}

int ResourceLoader::getFileSize(const char *name) {
	Common::ReadFileStream file(name);
	return file.size();
}

char *ResourceLoader::loadPakSafe(const char *name, int index) {
	char *ptr;
	ptr = loadPak(name, index);
	if(!ptr) {
		theEnd(0, name);
	}
	return ptr;
}

} // end of namespace Fitd
