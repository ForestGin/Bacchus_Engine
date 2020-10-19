#include "Resources.h"


#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")



void MyAssimpCallback(const char* msg, char* userData)
{
	LOG("[Assimp]: %s", msg);
}

bool Resources::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

Resources::Resources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Resources::~Resources()
{
}

bool Resources::Init(/*json file*/)
{
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

bool Resources::Start()
{
	return true;
}