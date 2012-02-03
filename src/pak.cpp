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

// seg 55
#include "filestream.h"
#include "common.h"

namespace Fitd {

typedef struct pakInfoStruct // warning: allignement unsafe
{
	long int discSize;
	long int uncompressedSize;
	char compressionFlag;
	char info5;
	short int offset;
};

typedef struct pakInfoStruct pakInfoStruct;

//#define USE_UNPACKED_DATA

void readPakInfo(pakInfoStruct* pPakInfo, FILE* fileHandle)
{
	fread(&pPakInfo->discSize,4,1,fileHandle);
	fread(&pPakInfo->uncompressedSize,4,1,fileHandle);
	fread(&pPakInfo->compressionFlag,1,1,fileHandle);
	fread(&pPakInfo->info5,1,1,fileHandle);
	fread(&pPakInfo->offset,2,1,fileHandle);
	
	pPakInfo->discSize = READ_LE_UINT32(&pPakInfo->discSize);
	pPakInfo->uncompressedSize = READ_LE_UINT32(&pPakInfo->uncompressedSize);
	pPakInfo->offset = READ_LE_UINT16(&pPakInfo->offset);
}

unsigned int PAK_getNumFiles(char* name)
{
	char bufferName[256];
	FILE* fileHandle;
	long int fileOffset;
	char* ptr=0;
	long int size=0;
	
	strcpy(bufferName, name); // temporary until makeExtention is coded
	strcat(bufferName,".PAK");
	
	Common::ReadFileStream file(bufferName);
	file.seek(4, SEEK_CUR);
	fileOffset = file.readUint32LE();
	
	file.close();
	
	return((fileOffset/4)-2);
}

int loadPakToPtr(char* name, int index, char* ptr)
{
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	FILE* fHandle;
	int size;
	
	sprintf(buffer,"%s/%04X.OUT",name,index);

	Common::ReadFileStream file(buffer);

	if(!file.isOpen())
		return(0);
	
	size = file.size();
	
	file.read(ptr, size);
	file.close();
	
	return(1);
#else
	char* lptr;
	
	lptr = loadPak(name,index);
	
	memcpy(ptr,lptr,getPakSize(name,index));
	
	free(lptr);
	
	return(1);
#endif
}

int getPakSize(char* name, int index) {
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	FILE* fHandle;
	int size;
	
	sprintf(buffer,"%s/%04X.OUT",name,index);
	
	ReadFileStream file(buffer);

	if (!file.isOpen())
		return(0);

	size = file.size();	
	file.close();
	
	return (size);
#else
	char bufferName[256];
	FILE* fileHandle;
	long int fileOffset;
	long int additionalDescriptorSize;
	pakInfoStruct pakInfo;
	char* ptr=0;
	long int size=0;
	
	strcpy(bufferName, name); // temporary until makeExtention is coded
	strcat(bufferName,".PAK");
	
	Common::ReadFileStream file(bufferName);
	
	if(file.isOpen()) // a bit stupid, should return NULL right away
	{
		file.seek((index + 1) * 4, SEEK_SET);
	
		fileOffset = file.readUint32LE();
		file.seek(fileOffset, SEEK_SET);
		
		additionalDescriptorSize = file.readUint32LE();
		
		readPakInfo(&pakInfo,fileHandle);
	
		file.seek(pakInfo.offset, SEEK_CUR);
		
		if(pakInfo.compressionFlag == 0) {// uncompressed
			size = pakInfo.discSize;
		} else if(pakInfo.compressionFlag == 1) { // compressed
			size = pakInfo.uncompressedSize;
		} else if(pakInfo.compressionFlag == 4)	{
			size = pakInfo.uncompressedSize;
		}
		
		file.close();
	}
	
	return size;
#endif
}

char* loadPak(const char* name, int index)
{
#ifdef USE_UNPACKED_DATA
	char buffer[256];
	FILE* fHandle;
	int size;
	char* ptr;
	
	sprintf(buffer,"%s/%04X.OUT",name,index);

	Common::ReadFileStream file(buffer);

	if(!file.isOpen())
		return NULL;

	size = file.size();
	
	ptr = new char[size];

	file.read(ptr, size);
	file.close();
	
	return ptr;
#else
	char bufferName[256];
	FILE* fileHandle;
	long int fileOffset;
	long int additionalDescriptorSize;
	pakInfoStruct pakInfo;
	char* ptr=0;
	
	//makeExtention(bufferName, name, ".PAK");
	strcpy(bufferName, name); // temporary until makeExtention is coded
	strcat(bufferName,".PAK");

	Common::ReadFileStream file(bufferName);

	if(file.isOpen()) { // a bit stupid, should return NULL right away
		char nameBuffer[256];
		
		file.seek((index + 1) * 4, SEEK_SET);
	
		fileOffset = file.readUint32LE();
		
		file.seek(fileOffset, SEEK_SET);

		additionalDescriptorSize = file.readUint32LE();
		
		readPakInfo(&pakInfo,fileHandle);
		
		if(pakInfo.offset) {
			ASSERT(pakInfo.offset<256);
		
			file.read(nameBuffer, pakInfo.offset);
#ifdef INTERNAL_DEBUGGER
			printf("Loading %s/%s\n", name,nameBuffer+2);
#endif
		} else {
			file.seek(pakInfo.offset, SEEK_CUR);
		}
		
		switch(pakInfo.compressionFlag)
		{
			case 0:
			{
				ptr = (char*)malloc(pakInfo.discSize);
				file.read(ptr, pakInfo.discSize);
				break;
			}
			case 1:
			{
				char * compressedDataPtr = (char *) malloc(pakInfo.discSize);
				file.read(compressedDataPtr, pakInfo.discSize);
				ptr = new char[pakInfo.uncompressedSize];

				PAK_explode((unsigned char*)compressedDataPtr, (unsigned char*)ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5);
				
				delete[] compressedDataPtr;
				break;
			}
			case 4:
			{
				char * compressedDataPtr = (char *) malloc(pakInfo.discSize);
				file.read(compressedDataPtr, pakInfo.discSize);
				ptr = new char[pakInfo.uncompressedSize];
				
				PAK_deflate((unsigned char*)compressedDataPtr, (unsigned char*)ptr, pakInfo.discSize, pakInfo.uncompressedSize);
				
				delete[] compressedDataPtr;
				break;
			}
		}
		file.close();
	}
	
	return ptr;
#endif
}
	
} // end of namespace Fitd
