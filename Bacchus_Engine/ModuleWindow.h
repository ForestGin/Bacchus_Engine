#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	bool IsFullScreen() const { return fullscreen; }
	bool IsResizable() const { return resizable; }
	bool IsBorderless() const { return borderless; }
	bool IsFullScreenDesktop() const { return fulldesktop; }



	void SetTitle(const char* title);
	void SetFullScreen(bool _fullscreen);
	void SetResizable(bool _resizable);
	void SetBorderless(bool borderless);
	void SetFullScreenDesktop(bool fullsdesktop);

public:

	float brightness = 1.0;
	bool resizable = false;
	bool fullscreen = false;
	bool borderless = false;
	bool fulldesktop = false;

	int width;
	int height;
	Uint32 flags;
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


};

#endif // __ModuleWindow_H__