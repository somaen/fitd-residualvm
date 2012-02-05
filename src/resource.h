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

#ifndef FITD_RESOURCE_H
#define FITD_RESOURCE_H

#include "common/stream.h"

namespace Fitd {

class ResourceLoader {
public:
	bool getFileExists(const char *);
	char *loadFromItd(const char *name);
	char *loadPakSafe(const char *name, int index);
	int getFileSize(const char *name);
	Common::SeekableReadStream *getFile(const char* name);
};

extern ResourceLoader *g_resourceLoader;

} // end of namespace Fitd

#endif
