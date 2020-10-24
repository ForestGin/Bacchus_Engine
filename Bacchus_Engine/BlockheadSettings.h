#ifndef __BLOCKHEAD_SETTINGS_H__
#define __BLOCKHEAD_SETTINGS_H__

#define FPS_TRACKER_SIZE 100

#include "Blockhead.h"
#include <vector>


class BlockheadSettings : public Blockhead
{
public:

	BlockheadSettings(char* name);
	~BlockheadSettings();

	void AddFPS(float fps, float ms);
	bool Draw();

private:

	void ApplicationNode() const;
	void WindowNode() const;
	void InputNode() const;
	void HardwareNode() const;
	void RendererNode() const;

private:
	std::vector<float> FPS_Tracker;
	std::vector<float> MS_Tracker;
};

#endif