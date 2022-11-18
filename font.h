#ifndef _FONT_H_
#define _FONT_H_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "window.h"

class Font
{
public:
	Font ();
	~Font ();

	void write (float xpos, float ypos, const char *string);
	void init (int height, char *font_name, Window win);
	void free ();

private:
	GLuint fontbase;
};

#endif