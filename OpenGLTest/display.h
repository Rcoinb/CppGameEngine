#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <string>
#include <SDL2/SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void Update();
	void UpdateFPSCounter();

	inline float* GetFPS() { return &framespersecond; }

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	inline float GetAspect() const { return (float)width / (float)height; }

	virtual ~Display();
protected:
private:
	void InitGL();
	void InitFPSCounter();

	SDL_Window* m_window;
	SDL_GLContext m_glContext;

	int width;
	int height;
	bool mouseGrabbed;


	//fps counter
	static int const FRAME_VALUES = 10;
	Uint32 frametimes[FRAME_VALUES];
	Uint32 frametimelast;
	Uint32 framecount;
	float framespersecond;
};

#endif
