#include "font.h"

Font::Font ()
{
}

Font::~Font ()
{
	free ();
}

void Font::write (float xpos, float ypos, const char *string)
{
	glRasterPos2f (xpos, ypos);
	glPushAttrib (GL_LIST_BIT);
	glListBase (fontbase - 32);
	glCallLists ((GLsizei) strlen (string), GL_UNSIGNED_BYTE, string);
	glPopAttrib ();
}

void Font::init (int height, char *font_name, Window win)
{
	HFONT font, hfont;

	fontbase = glGenLists (96);

	font = CreateFont (height, 0, 0, 0, 100, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, font_name);

	HDC myHDC = win.getDC();

	hfont = (HFONT) SelectObject (myHDC, font);
	wglUseFontBitmaps (myHDC, 32, 96, fontbase);
	SelectObject (myHDC, hfont);
}


void Font::free ()
{
	glDeleteLists (fontbase, 96);
}
