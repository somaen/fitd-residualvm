// Dummy-class designed to be a workalike for OSystem-streams:

#include "filestream.h"

namespace Common {

ReadFileStream::ReadFileStream() {
	_fileHandle = NULL;
}
ReadFileStream::ReadFileStream(const char *filename) {
	_fileHandle = NULL;
	open(filename);
}

ReadFileStream::~ReadFileStream() {
	close();
}

bool ReadFileStream::open(const char *filename) {
	_fileHandle = (void*)fopen(filename, "rb");
	if(_fileHandle)
		return true;
	else
		return false;
}

uint32 ReadFileStream::read(byte *target, uint32 size) {
	fread(target, 1, size, (FILE*)_fileHandle);
}

void ReadFileStream::seek(int32 offset, int whence) {
	fseek((FILE*)_fileHandle, offset, whence);
}

void ReadFileStream::close() {
	fclose((FILE*)_fileHandle);
}

uint32 ReadFileStream::readUint32LE() {
	uint32 retval;
	read((byte *)&retval, 4);
	return READ_LE_UINT32(&retval);
}

uint16 ReadFileStream::readUint16LE() {
	uint16 retval;
	read((byte *)&retval, 2);
	return READ_LE_UINT16(&retval);
		
}
	
byte ReadFileStream::readByte() {
	byte retval;
	read(&retval, 1);
	return retval;
}
	
int32 ReadFileStream::size() {
	uint32 oldpos = ftell((FILE*)_fileHandle);
	fseek((FILE*)_fileHandle, 0, SEEK_END);
	uint32 retval = ftell((FILE*)_fileHandle);
	fseek((FILE*)_fileHandle, oldpos, SEEK_SET);
	return retval;
}

bool ReadFileStream::isOpen() {
	return (_fileHandle ? true : false);
}

} // end of namespace Common


