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

#include "common.h"

namespace Fitd {

hqrSubEntryStruct *quickFindEntry(int index, int numMax, hqrSubEntryStruct *ptr) { // no RE. Original was probably faster
	int i;

	for(i = 0; i < numMax; i++) {
		if((ptr[i].key == index) && ptr[i].ptr) {
			return(&ptr[i]);
		}
	}

	return(NULL);
}

hqrEntryStruct *HQR_InitRessource(const char *name, int size, int numEntries) {
	int i;
	hqrEntryStruct *dest;

	dest = (hqrEntryStruct *)malloc(sizeof(hqrEntryStruct));

	if(!dest)
		return NULL;

	numEntries = 2000;

	strcpy(dest->_string, "        ");
	strncpy(dest->_string, name, 8);

	dest->_sizeFreeData = size;
	dest->_maxFreeData = size;
	dest->_numMaxEntry = numEntries;
	dest->_numUsedEntry = 0;
	dest->_entries = (hqrSubEntryStruct *)malloc(numEntries * sizeof(hqrSubEntryStruct));

	for(i = 0; i < numEntries; i++) {
		dest->_entries[i].ptr = NULL;
	}

	return(dest);
}

int hqrEntryStruct::printTextSub1(int size) {
	hqrSubEntryStruct *dataPtr1;
	hqrSubEntryStruct *dataPtr2;
	int key;
	int entryNum;

	if(_sizeFreeData < size)
		return(-1);

	entryNum = _numUsedEntry;

	dataPtr1 = dataPtr2 = _entries;

	key = hqrKeyGen;

	dataPtr1[entryNum].key = key;

	//  dataPtr1[entryNum].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
	dataPtr1[entryNum].size = size;

	_numUsedEntry++;
	_sizeFreeData -= size;

	hqrKeyGen++;

	return(key);
}

char *hqrEntryStruct::printTextSub2(int index) {
	hqrSubEntryStruct *ptr;
	hqrSubEntryStruct *dataPtr;

	if(index < 0)
		return NULL;

	dataPtr = _entries;

	ptr = quickFindEntry(index, _numUsedEntry, dataPtr);

	if(!ptr)
		return NULL;

	return(ptr->ptr);
}

void moveHqrEntry(hqrEntryStruct *hqrPtr, int index) {
	/*  hqrSubEntryStruct* hqrSubPtr = (hqrSubEntryStruct*)(((char*)hqrPtr)+sizeof(hqrEntryStruct));
	 hqrSubEntryStruct* hqrSubPtr2 = hqrSubPtr;

	 int size = hqrSubPtr[index].size;

	 if(hqrPtr->numUsedEntry - 1 > index ) //if not last entry
	 {
	 char* dest = hqrPtr->dataPtr + hqrSubPtr2[index].offset;
	 char* src = dest + size;

	 memcpy(dest,src,hqrPtr->dataPtr + hqrPtr->maxFreeData - src);

	 dest = (char*)&hqrSubPtr2[index];
	 src = (char*)&hqrSubPtr2[index+1];
	 memcpy(dest,src,hqrPtr->numMaxEntry-(index+1) * sizeof(hqrSubEntryStruct));
	 }*/

	int size = hqrPtr->_entries[index].size;

	free(hqrPtr->_entries[index].ptr);

	hqrPtr->_numUsedEntry --;
	hqrPtr->_sizeFreeData += size;
}

char *hqrEntryStruct::get(int index) {
	hqrSubEntryStruct *foundEntry;

	if(index < 0)
		return NULL;

	foundEntry = quickFindEntry(index, _numUsedEntry, _entries);

	if(foundEntry) {
		foundEntry->lastTimeUsed = timer;
		hqrVar1 = 0;

		return(foundEntry->ptr);
	} else {
		/*    int size;
		 unsigned int time;
		 char* ptr;

		 freezeTime();
		 size = getPakSize(hqrPtr->string,index);

		 if(size>=hqrPtr->maxFreeData)
		 {
		 theEnd(1,hqrPtr->string);
		 }

		 time = timer;

		 foundEntry = hqrSubPtr;

		 while(size>hqrPtr->sizeFreeData || hqrPtr->numUsedEntry>= hqrPtr->numMaxEntry)
		 {
		 int bestEntry = 0;
		 unsigned int bestTime = 0;
		 int entryIdx = 0;

		 for(entryIdx = 0; entryIdx<hqrPtr->numUsedEntry; entryIdx++)
		 {
		 if(time - foundEntry[entryIdx].lastTimeUsed > bestTime)
		 {
		 bestTime = time - foundEntry[entryIdx].lastTimeUsed;
		 bestEntry = entryIdx;
		 }
		 }

		 moveHqrEntry(hqrPtr,bestEntry);
		 }

		 ptr = hqrPtr->dataPtr + (hqrPtr->maxFreeData - hqrPtr->sizeFreeData);

		 if(!loadPakToPtr(hqrPtr->string,index,ptr))
		 {
		 theEnd(1,hqrPtr->string);
		 }

		 hqrVar1 = 1;

		 foundEntry[hqrPtr->numUsedEntry].key = index;
		 foundEntry[hqrPtr->numUsedEntry].lastTimeUsed = timer;
		 foundEntry[hqrPtr->numUsedEntry].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
		 foundEntry[hqrPtr->numUsedEntry].size = size;

		 hqrPtr->numUsedEntry++;
		 hqrPtr->sizeFreeData -= size;

		 unfreezeTime();*/

		int size;
		unsigned int time;
		char *ptr;
		int i;

		freezeTime();
		size = getPakSize(_string, index);

		if(size >= _maxFreeData) {
			theEnd(1, _string);
		}

		time = timer;

		for(i = 0; i < _numMaxEntry; i++) {
			if(_entries[i].ptr == NULL) {
				foundEntry = &_entries[i];
				break;
			}
		}

		ASSERT(foundEntry);

		//    foundEntry = hqrSubPtr;

		hqrVar1 = 1;

		foundEntry->key = index;
		foundEntry->lastTimeUsed = timer;
		//foundEntry[hqrPtr->numUsedEntry].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
		foundEntry->size = size;
		foundEntry->ptr = (char *)malloc(size);

		ptr = foundEntry->ptr;

		loadPakToPtr(_string, index, foundEntry->ptr);

		_numUsedEntry++;
		_sizeFreeData -= size;

		unfreezeTime();

		return(ptr);
	}
}

hqrEntryStruct *HQR_Init(int size, int numEntry) {
	int i;
	hqrEntryStruct *dest;
	char *dest2;

	ASSERT(size > 0);
	ASSERT(numEntry > 0);

	dest = (hqrEntryStruct *)malloc(sizeof(hqrEntryStruct));

	numEntry = 2000;

	ASSERT_PTR(dest);

	if(!dest)
		return NULL;

	dest2 = (char *)malloc(size);

	ASSERT_PTR(dest2);

	if(!dest2)
		return NULL;

	strcpy(dest->_string, "_MEMORY_");

	dest->_sizeFreeData = size;
	dest->_maxFreeData = size;
	dest->_numMaxEntry = numEntry;
	dest->_numUsedEntry = 0;
	dest->_entries = (hqrSubEntryStruct *)malloc(numEntry * sizeof(hqrSubEntryStruct));

	for(i = 0; i < numEntry; i++) {
		dest->_entries[i].ptr = NULL;
	}

	return(dest);
}

void hqrEntryStruct::reset() {
	_sizeFreeData = _maxFreeData;
	_numUsedEntry = 0;

	for(int i = 0; i < _numMaxEntry; i++) {
		if(_entries[i].ptr)
			free(_entries[i].ptr);

		_entries[i].ptr = NULL;
	}

}

hqrEntryStruct::~hqrEntryStruct() {
	for(int i = 0; i < _numMaxEntry; i++) {
		if(_entries[i].ptr)
			free(_entries[i].ptr);
	}
}

void hqrEntryStruct::setString(const char* str) {
	assert(strlen(str) <= 10);
	strcpy(_string, str);
}

} // end of namespace Fitd
