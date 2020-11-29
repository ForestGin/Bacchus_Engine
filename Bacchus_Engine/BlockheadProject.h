#ifndef __BLOCKHEAD_PROJECT_H__
#define __BLOCKHEAD_PROJECT_H__

#include "Blockhead.h"


class BlockheadProject : public Blockhead
{
public:

	BlockheadProject(char* name);
	~BlockheadProject();

	bool Draw();
};

#endif
