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

// Dummy-class designed to be a workalike for OSystem-streams:
#ifndef FITD_FILESTREAM_H
#define FITD_FILESTREAM_H

#ifndef UNIX
#define UNIX
#endif

#ifdef HAVE_CONFIG_H
#undef HAVE_CONFIG_H
#include "common/endian.h"
#define HAVE_CONFIG_H
#else
#include "common/endian.h"
#endif

#include "common/stream.h"

namespace Common {

class ReadFileStream : public SeekableReadStream {
	void *_fileHandle;
public:
	ReadFileStream();
	ReadFileStream(const char *filename);
	~ReadFileStream();
	bool open(const char *filename);
	bool isOpen();
	uint32 read(byte *target, uint32 size);
	uint32 readUint32LE();
	uint16 readUint16LE();
	byte readByte();
	int32 size();

	void seek(int32 offset, int whence = SEEK_SET);
	void close();
};

}

#endif

