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