#include <windows.h>

extern int OPENGL_XRES;
extern int OPENGL_YRES;
extern int OPENGL_FULLSCREEN;
extern int mouseCoords [2];
extern bool mouseLeftButtonClicked;

class Window
{
public:
	Window ();
	~Window ();

	bool create (char *title);
	void shut ();
	HDC getDC ();

private:
	HWND hwnd;
	HDC hdc;
	HGLRC hrc;
	PIXELFORMATDESCRIPTOR p;
	int pixelformat;

	DEVMODE screenSettings;
	HINSTANCE hInstance;
};

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

