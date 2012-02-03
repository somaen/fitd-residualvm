#ifndef _PAK_
#define _PAK_
namespace Fitd {

char* loadPak(char* name, int index);
int loadPakToPtr(char* name, int index, char* ptr);
int getPakSize(char* name, int index);
unsigned int PAK_getNumFiles(char* name);

} // end of namespace Fitd

#endif
