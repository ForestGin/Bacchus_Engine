#ifndef __BLOCKHEAD_HIERARCHY_H__
#define __BLOCKHEAD_HIERARCHY_H__

#include "Blockhead.h"

class BlockheadHierarchy : public Blockhead
{
public:

	BlockheadHierarchy(char* name);
	~BlockheadHierarchy();

	bool Draw();
};

#endif