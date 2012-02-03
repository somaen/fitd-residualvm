#ifndef _FILE_ACCESS_
#define _FILE_ACCESS_
namespace Fitd {

void theEnd(int type, char* name);
char* loadFromItd(char* name);
char* loadPakSafe(char* name, int index);

} // end of namespace Fitd

#endif
