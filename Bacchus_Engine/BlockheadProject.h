#ifndef __BLOCKHEAD_PROJECT_H__
#define __BLOCKHEAD_PROJECT_H__

#include "Blockhead.h"

#include <string>
#include <vector>

class BlockheadProject : public Blockhead
{
public:

	BlockheadProject(char* name);
	~BlockheadProject();

	bool Draw();

	std::string dragged;

private:

	void RecursiveDirectoryDraw(const char* directory, std::vector<std::string>& filters);
};

#endif

