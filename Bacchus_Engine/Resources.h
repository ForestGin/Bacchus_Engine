#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "Module.h"

class Resources : public Module
{
public:

	Resources(Application* app, bool start_enabled = true);
	~Resources();

	bool Init(/*json file*/);
	bool Start();
	//update_status PreUpdate(float dt);
	//update_status Update(float dt);
	//update_status PostUpdate(float dt);
	bool CleanUp();

};

#endif
