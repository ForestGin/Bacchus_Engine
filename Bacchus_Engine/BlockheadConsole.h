#ifndef __BLOCKHEAD_CONSOLE_H__
#define __BLOCKHEAD_CONSOLE_H__

#include "Blockhead.h"
#include "imgui/imgui.h"

class BlockheadConsole : public Blockhead
{
public:

	BlockheadConsole(char* name);
	~BlockheadConsole();

	bool Draw();

private:

	void Clear();
	ImGuiTextFilter filter;
	char* error_key = "|";
};

#endif
