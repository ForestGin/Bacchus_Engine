#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	width = 0;
	height = 0;
	fullscreen = WIN_FULLSCREEN;
	resizable = WIN_RESIZABLE;
	borderless = WIN_BORDERLESS;
	fulldesktop = WIN_FULLSCREEN_DESKTOP;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(/*json file*/)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}


void ModuleWindow::SetFullScreen(bool _fullscreen)
{

	Uint32 fullsFlag = _fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_MINIMIZED;
	fullscreen = _fullscreen;
	SDL_SetWindowFullscreen(window, fullsFlag);
}

void ModuleWindow::SetResizable(bool _resizable)
{
	resizable = _resizable;
	SDL_bool res = SDL_bool(_resizable);
	SDL_SetWindowResizable(window, res);
}
void ModuleWindow::SetBorderless(bool _borderless)
{
	borderless = _borderless;
	SDL_bool bord = SDL_bool(!_borderless);
	SDL_SetWindowBordered(window, bord);
}

void ModuleWindow::SetFullScreenDesktop(bool _fulldesktop)
{
	fulldesktop = _fulldesktop;
	SDL_DisplayMode dMode;
	SDL_GetDesktopDisplayMode(0, &dMode);

	if (_fulldesktop)
	{
		SDL_SetWindowSize(window, dMode.w, dMode.h);
		SDL_SetWindowPosition(window, 0, 0);
	}
	else
	{
		SDL_SetWindowSize(window, dMode.w / 2, dMode.h / 2);
		SDL_SetWindowPosition(window, dMode.w / 4, dMode.h / 4);
	}
}