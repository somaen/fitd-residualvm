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

#ifndef _HQR_
#define _HQR_
namespace Fitd {

struct hqrSubEntryStruct {
	int16 key;
	int16 size;
	uint32 lastTimeUsed;
	char *ptr;
};
	
struct hqrEntryStruct {
	char _string[10];
	uint16 _maxFreeData;
	uint16 _sizeFreeData;
	uint16 _numMaxEntry;
	uint16 _numUsedEntry;
	hqrSubEntryStruct *_entries;
	
	~hqrEntryStruct();
	char *get(int index);
	void reset();
	int printTextSub1(int size);
	char *printTextSub2(int index);
	void setString(const char* str);
};

hqrEntryStruct *HQR_InitRessource(const char *name, int size, int numEntries);
hqrEntryStruct *HQR_Init(int size, int numEntry);


} // end of namespace Fitd

#endif
