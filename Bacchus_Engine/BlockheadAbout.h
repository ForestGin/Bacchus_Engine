#ifndef __BLOCKHEAD_ABOUT_H__
#define __BLOCKHEAD_ABOUT_H__

#include "Blockhead.h"

class BlockheadAbout : public Blockhead
{
public:

	BlockheadAbout(char* name);
	~BlockheadAbout();

	bool Draw();
};

#endif