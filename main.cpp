#define WIN32_LEAN_AND_MEAN
#define WIN32_LEANER_AND_MEANER

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "sudoku.h"

int OPENGL_XRES = 800;
int OPENGL_YRES = 600;
int OPENGL_FULLSCREEN = false;

Window win;

int tickCount;

int mouseCoords [2];
bool mouseLeftButtonClicked = false;

Font verdana20,
	 verdana30;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmd, int n)
{
	MSG msg = { 0 };
	bool done = false;
	Sudoku sudoku;
	int currentStep = -3, 
		tickCountInitDone = 0,
		tickCountYesNoDone = 0, 
		tickCountPreviousStep = 0,
		selectedField = -1;

	if (!win.create ("Interactive Sudoku Puzzle Solver"))
		return 0;

	verdana20.init (20, "Verdana", win);
	verdana30.init (30, "Verdana", win);

	sudoku.init ();

	int startTime = GetTickCount ();

	while (!done && !GetAsyncKeyState (VK_ESCAPE))
	{
			if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					done = true;
				else
				{
					TranslateMessage (&msg);
					DispatchMessage (&msg);
				}
			}
			else
			{
				glEnable (GL_POINT_SMOOTH);
				glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
				glEnable (GL_LINE_SMOOTH);
				glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

				glClearColor (1, 1, 1, 1);
				glDepthMask (1);
				glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport (0, 0, OPENGL_XRES, OPENGL_YRES);

				glMatrixMode (GL_PROJECTION);
				glLoadIdentity ();
				gluPerspective (45.0f, 1024.0f/768.0f, 0.5f, 14000.0f);
				glMatrixMode (GL_MODELVIEW);
				glLoadIdentity ();

				if (currentStep == -3)
				{
					if (GetAsyncKeyState ('Y'))
					{
						if (!sudoku.readInputFile ("sudoku_i.txt"))
							return 0;

						currentStep = -2;
						tickCountYesNoDone = GetTickCount () - startTime;
					}
					else if (GetAsyncKeyState ('N'))
					{
						currentStep = -2;
						tickCountYesNoDone = GetTickCount () - startTime;
					}
				}
				else if (currentStep == -2)
				{
					if (mouseLeftButtonClicked)
					{
						if (mouseCoords [1] >= 84 && mouseCoords [1] <= 84 + 35 * 9 - 1 && mouseCoords [0] >= 218 && mouseCoords [0] <= 218 + 35 * 9 - 1)
							selectedField = (mouseCoords [1] - 84) / 35 * 9 + (mouseCoords [0] - 218) / 35;
					}

					if (selectedField != -1)
					{
						for (char i = '0'; i <= '9'; i++)
							if (GetAsyncKeyState (i))
								{
									sudoku.setNumber (selectedField, i);
									selectedField = -1;
								}

						for (char i = VK_NUMPAD0; i <= VK_NUMPAD9; i++)
							if (GetAsyncKeyState (i))
								{
									sudoku.setNumber (selectedField, i - VK_NUMPAD0 + '0');
									selectedField = -1;
								}
					}

					if (GetAsyncKeyState (VK_RETURN))
					{
						sudoku.solve ();
						currentStep = 0;
					}

					while (GetAsyncKeyState (VK_RETURN));

					tickCountInitDone = GetTickCount () - startTime;
				}
				else
				{
					if (GetAsyncKeyState (VK_RIGHT) && tickCount > tickCountPreviousStep + 300 && currentStep < sudoku.getMaxStep ())
					{
						currentStep++;
						tickCountPreviousStep = tickCount;
					}
					else if (GetAsyncKeyState (VK_LEFT) && tickCount > tickCountPreviousStep + 300 && currentStep > 0)
					{
						currentStep--;
						tickCountPreviousStep = tickCount;
					}
					else if ((GetAsyncKeyState (VK_RETURN) || GetAsyncKeyState (VK_END)) && tickCount > tickCountPreviousStep + 300)
					{
						currentStep = sudoku.getMaxStep ();
						tickCountPreviousStep = tickCount;
					}
					else if (GetAsyncKeyState (VK_HOME) && tickCount > tickCountPreviousStep + 300)
					{
						currentStep = 0;
						tickCountPreviousStep = tickCount;
					}
				}

				tickCount = GetTickCount () - startTime;

				currentStep = sudoku.display (tickCount, tickCountYesNoDone, tickCountInitDone, tickCountPreviousStep, currentStep, selectedField);

				SwapBuffers (win.getDC());
			}
	}

	if (currentStep >= 0)
		sudoku.writeOutputFile ("sudoku_o.txt");

	return 1;
}
