#ifndef __BLOCKHEAD_IMPORTER_H__
#define __BLOCKHEAD_IMPORTER_H__

#include "Blockhead.h"


class BlockheadImporter : public Blockhead
{
public:

	BlockheadImporter(char* name);
	~BlockheadImporter();

	bool Draw();


public:

	bool applied = false;
	bool defaulted = false;

private:

	bool TestImport();
};

#endif
