#include "window.h"

Window::Window ()
{
}

Window::~Window ()
{
}

bool Window::create (char *title)
{
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;	

    WNDCLASS wc;
		
	hInstance = GetModuleHandle (NULL);
	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon (hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass (&wc)) 
		return false;

	RtlZeroMemory (&screenSettings, sizeof (screenSettings));
	p.nSize = sizeof (PIXELFORMATDESCRIPTOR);
	p.nVersion = 1;
	p.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	p.iPixelType = PFD_TYPE_RGBA;
	p.cColorBits = 32;
	p.cDepthBits = 16;
	p.iLayerType = PFD_MAIN_PLANE;

	RtlZeroMemory (&screenSettings, sizeof (screenSettings));
	screenSettings.dmSize = sizeof (screenSettings);
	screenSettings.dmPelsWidth = OPENGL_XRES;
	screenSettings.dmPelsHeight = OPENGL_YRES;
	screenSettings.dmBitsPerPel = 32;
	screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
	if (OPENGL_FULLSCREEN)
	{
		DEVMODE dmScreenSettings;
		memset (&dmScreenSettings, 0, sizeof (dmScreenSettings));
		dmScreenSettings.dmSize = sizeof (dmScreenSettings);
		dmScreenSettings.dmPelsWidth = OPENGL_XRES;
		dmScreenSettings.dmPelsHeight = OPENGL_YRES;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			return false;

		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
	}
	
	hwnd = CreateWindowEx (dwExStyle, "OpenGL", title, dwStyle, 0, 0, OPENGL_XRES, OPENGL_YRES, NULL, NULL, hInstance, NULL);
	hdc = GetDC (hwnd);
		
	pixelformat = ChoosePixelFormat (hdc, &p);
	SetPixelFormat (hdc, pixelformat, &p);
	hrc = wglCreateContext (hdc);
	wglMakeCurrent (hdc, hrc);
	if (OPENGL_FULLSCREEN)
		ShowCursor (false);
	else
		ShowCursor (true);

	ShowWindow (hwnd, SW_SHOW);
	SetForegroundWindow (hwnd);
	SetFocus (hwnd);

	return true;
}

void Window::shut()
{
	ChangeDisplaySettings (NULL, 0);
	ReleaseDC (hwnd, hdc);
	DestroyWindow (hwnd);
}

HDC Window::getDC()
{
	return hdc;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{			
	switch (iMsg)
	{
		case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}		
			break;									

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;		

		case WM_MOUSEMOVE:
			mouseCoords [0] = LOWORD (lParam);
			mouseCoords [1] = HIWORD (lParam);
			break;

		case WM_LBUTTONDOWN:
			mouseLeftButtonClicked = true;
			break;

		case WM_LBUTTONUP:
			mouseLeftButtonClicked = false;
			break;
	}	

	return DefWindowProc (hwnd, iMsg, wParam, lParam);
}
