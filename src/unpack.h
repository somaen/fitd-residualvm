#ifndef _UNPACK_DAMIEN_H_
#define _UNPACK_DAMIEN_H_
namespace Fitd {
int PAK_deflate(unsigned char *srcBuffer, unsigned char *dstBuffer, unsigned int compressedSize, unsigned int uncompressedSize);
int PAK_explode(unsigned char *srcBuffer, unsigned char *dstBuffer, unsigned int compressedSize, unsigned int uncompressedSize, unsigned short flags);
} // end of namespace Fitd

#endif
