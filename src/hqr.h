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

#ifndef FITD_HQR_H
#define FITD_HQR_H

namespace Fitd {

struct hqrSubEntryStruct;
	
struct hqrEntryStruct {
private:
	char _string[10];
	uint16 _maxFreeData;
	uint16 _sizeFreeData;
	uint16 _numMaxEntry;
	uint16 _numUsedEntry;
	hqrSubEntryStruct *_entries;
public:
	hqrEntryStruct(const char *name, int size, int numEntries);
	hqrEntryStruct(int size, int numEntries);
	~hqrEntryStruct();
	char *get(int index);
	void reset();
	int printTextSub1(int size);
	char *printTextSub2(int index);
	void setString(const char* str);
	void moveHqrEntry(int index);
};

} // end of namespace Fitd

#endif
