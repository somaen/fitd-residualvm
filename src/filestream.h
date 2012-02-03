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

namespace Common {

class ReadFileStream {
void *_fileHandle;
public:
	ReadFileStream();
	ReadFileStream(const char* filename);
	~ReadFileStream();
	bool open(const char* filename);
	bool isOpen();
	uint32 read(byte *target, uint32 size);
	uint32 readUint32LE();
	int32 size();
	void seek(int32 offset, int whence = SEEK_SET);
	void close();
};

}

#endif

