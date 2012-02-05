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

// Just a workalike for the ResidualVM/ScummVM streams

#ifndef FITD_COMMONSTREAM_H
#define FITD_COMMONSTREAM_H

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

namespace Common {

class SeekableReadStream {
public:
	virtual ~SeekableReadStream() {}
	virtual bool open(const char *filename) = 0;
	virtual bool isOpen() = 0;
	virtual uint32 read(byte *target, uint32 size) = 0;
	virtual uint32 readUint32LE() = 0;
	virtual uint16 readUint16LE() = 0;
	virtual byte readByte() = 0;
	virtual int32 size() = 0;
	
	virtual void seek(int32 offset, int whence = SEEK_SET) = 0;
	virtual void close() = 0;
};

}

#endif