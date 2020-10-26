#include "Blockhead.h"

#include "mmgr/mmgr.h"

Blockhead::Blockhead(char* name) : name(name) {}

Blockhead::~Blockhead() {}

void Blockhead::OnOff()
{
	enabled = !enabled;
}

bool Blockhead::IsEnabled() const
{
	return enabled;
}