#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "font.h"
#include "wcpt2d.h"

extern Font verdana20;
extern Font verdana30;

class Sudoku
{
public:
	Sudoku ();
	~Sudoku ();

	void init ();
	bool readInputFile (char *filename);
	void writeOutputFile (char *filename);
	void writeOutputFile1 (char *filename);
	int display (int tickCount, int tickCountYesNoDone, int tickCountInitDone, int tickCountPreviousStep, int currentStep, int selectedField);
	void writeNumbers ();
	void writeNumber (int pos, int step);
	void convertNumbers ();
	int numberZeros (unsigned int numbersIntern_element);
	bool numberIsSolved (unsigned int numbersIntern_element);
	char getNumber (unsigned int numbersIntern_element);
	void copyNumbersHistory (int step);
	void solve ();
	int solveStep ();
	int getMaxStep ();
	void setNumber (int pos, char number);

private:
	unsigned int numbersIntern [81];
	unsigned int numbersInternHistory [82][81];
	int newNumberHistory [81];
	int maxStep;
	bool marked [81];
	bool stepForward;
	bool checked;
	int previousStep;
	int startingStep;
	int noNewNumber;
	int unsolvable;
	int initialNumbers;
	wcPt2D topLeftCorner;
	char numbers [82];
};
