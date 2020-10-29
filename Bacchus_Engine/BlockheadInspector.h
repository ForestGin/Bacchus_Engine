#ifndef __BLOCKHEAD_INSPECTOR_H__
#define __BLOCKHEAD_INSPECTOR_H__

#include "Blockhead.h"

class BlockheadInspector : public Blockhead
{
public:

	BlockheadInspector(char* name);
	~BlockheadInspector();

	bool Draw();

	bool Startup = true;
};

#endif
