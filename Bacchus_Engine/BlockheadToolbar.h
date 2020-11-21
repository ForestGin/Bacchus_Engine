#ifndef __BLOCKHEAD_TOOLBAR_H__
#define __BLOCKHEAD_TOOLBAR_H__

#include "Blockhead.h"

class BlockheadToolbar : public Blockhead
{
public:

	BlockheadToolbar(char* name);
	~BlockheadToolbar();

	bool Draw();
};

#endif
