// Copied from ResidualVM, covered by GPLv2+
#include <cstdio>
#include <cstdlib>
#include "textconsole.h"

void warning(const char *s, ...) {

}
void error(const char *s, ...) {
	va_list va;
	printf(s, va);
	exit(0);
}
