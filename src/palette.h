#ifndef FITD_PALETTE_H
#define FITD_PALETTE_H

//#include "common/endian.h"

namespace Fitd {

class Palette {
	char *_data;
public:
	Palette();
	Palette(char *pal);
	~Palette();
	void loadDefault();
	void loadDefaultAITD3();
	void convertIfRequired();
	void copyFrom(const unsigned char *source);
	void copyFrom(const char *source); // Remove this.
	void copyFrom(Palette *source);
	void fill(unsigned char r, unsigned char g, unsigned b);
	const unsigned char *getData();
};

}

#endif