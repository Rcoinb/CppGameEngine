#include "display.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
	this->width = width;
	this->height = height;

	SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	m_glContext = SDL_GL_CreateContext(m_window);

    if(glewInit() != GLEW_OK)
		std::cerr << "Glew failed to initialize!" << std::endl;
	
	this->InitGL();
	this->InitFPSCounter();
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::InitFPSCounter() 
{
	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	framespersecond = 0;
	frametimelast = SDL_GetTicks();
}

void Display::UpdateFPSCounter() {
	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	frametimesindex = framecount % FRAME_VALUES;
	getticks = SDL_GetTicks();
	frametimes[frametimesindex] = getticks - frametimelast;
	frametimelast = getticks;
	framecount++;
	if (framecount < FRAME_VALUES)
		count = framecount;
	else
		count = FRAME_VALUES;
	framespersecond = 0;

	for (unsigned int i = 0; i < count; i++) 
	{
		framespersecond += frametimes[i];
	}

	framespersecond /= count;
	framespersecond = 1000.f / framespersecond;
}

void Display::InitGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Update()
{
	SDL_GetWindowSize(m_window, &width, &height);
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SwapWindow(m_window);
}
