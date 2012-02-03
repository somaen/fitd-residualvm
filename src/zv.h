#ifndef _ZV_H_
#define _ZV_H_
namespace Fitd {

void getZvCube(char* bodyPtr, ZVStruct* zvPtr);
void getZvNormal(char* bodyPtr, ZVStruct* zvPtr);
void getZvMax(char* bodyPtr, ZVStruct* zvPtr);
void makeDefaultZV(ZVStruct* zvPtr);
void getZvMax(char* bodyPtr, ZVStruct* zvPtr);

} // end of namespace Fitd

#endif
