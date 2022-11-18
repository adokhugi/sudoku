//#define DEBUG

#include "sudoku.h"

Sudoku::Sudoku ()
{
}

Sudoku::~Sudoku ()
{
}

void Sudoku::init ()
{
	noNewNumber = 99;
	unsolvable = 98;
	initialNumbers = 97;
	topLeftCorner.x = -.25f;
	topLeftCorner.y = .25f;

	maxStep = 0;
	previousStep = 0;
	startingStep = 0;

	for (int i = 0; i < 81; i++)
		numbers [i] = '0';
}

bool Sudoku::readInputFile (char *filename)
{
	FILE *file;
	int i, j;

	file = fopen (filename, "rb");

	if (file == NULL)
		return false;

	for (i = 0; i < 81; i += 9)
	{
		for (j = 0; j < 9; j++)
			numbers [i + j] = (unsigned int) fgetc (file);
	    fgetc (file);
	    fgetc (file);
	}

	fclose (file);

	return true;
}

void Sudoku::writeOutputFile (char *filename)
{
	FILE *file;
	int i;

	file = fopen (filename, "wb");

	for (i = 0; i < 81; i++)
	{
		if ((i > 0) && ((i % 9) == 0))
		{
			fprintf (file, "%c", 0x0D);
			fprintf (file, "%c", 0x0A);
		}
		fprintf (file, "%c", getNumber (numbersInternHistory [maxStep][i]));
	}
	fprintf (file, "%c", 0x0D);
	fprintf (file, "%c", 0x0A);

	fclose (file);
}

int Sudoku::display (int tickCount, int tickCountYesNoDone, int tickCountInitDone, int tickCountPreviousStep, int currentStep, int selectedField)
{
#ifdef DEBUG
	const float delay = 5.0f;			// for quick testing purposes
#else
	const float delay = 5000.0f;		// for the final program
#endif

	glLoadIdentity ();
	glTranslatef (0, 0, -1.0f);

	glColor3f (0.0f, 0.0f, 0.0f);

	if (currentStep == -3)
	{
		float shade = 1.0f - tickCount / 500.0f;
		if (shade < 0.0f)
			shade = 0.0f;
		glColor3f (shade, shade, shade);
		verdana30.write (topLeftCorner.x + .02f, topLeftCorner.y - .25f, "Load sudoku_i.txt? (Y/N)");

		return currentStep;
	}

	if (currentStep >= -2)
	{
		for (int i = 0; i < 10; i++)
		{
			if ((tickCount - tickCountYesNoDone) / delay >= i * .5f / 10)
			{
				float beginPtCoord;
				float length = (tickCount - tickCountYesNoDone) / delay - i * .5f / 10;
				if (length > .5f * 9 / 10)
					length = .5f * 9 / 10;

				beginPtCoord = topLeftCorner.x + i * .5f / 10;

				glBegin (GL_LINE_STRIP);
					glVertex3f (beginPtCoord, topLeftCorner.y, 0);
					glVertex3f (beginPtCoord, topLeftCorner.y - length, 0);
				glEnd ();

				if (i % 3 == 0)
				{
					glBegin (GL_LINE_STRIP);
						glVertex3f (beginPtCoord + .0015f, topLeftCorner.y, 0);
						glVertex3f (beginPtCoord + .0015f, topLeftCorner.y - length, 0);
					glEnd ();
				}

				beginPtCoord = topLeftCorner.y - i * .5f / 10;

				glBegin (GL_LINE_STRIP);
					glVertex3f (topLeftCorner.x, beginPtCoord, 0);
					glVertex3f (topLeftCorner.x + length + .0015f, beginPtCoord, 0);
				glEnd ();

				if (i % 3 == 0)
				{
					glBegin (GL_LINE_STRIP);
						glVertex3f (topLeftCorner.x, beginPtCoord + .0015f, 0);
						glVertex3f (topLeftCorner.x + length + .0015f, beginPtCoord + .0015f, 0);
					glEnd ();
				}
			}
		}
	}

	if (currentStep == -2)
	{
		if ((tickCount - tickCountYesNoDone) <= delay * .5f * 19 / 10)
			return currentStep;

		if (selectedField != -1)
		{
			glBegin (GL_POLYGON);
				glVertex3f (topLeftCorner.x + (selectedField % 9) * .5f / 10, topLeftCorner.y - (selectedField / 9) * .5f / 10, 0);
				glVertex3f (topLeftCorner.x + ((selectedField % 9) + 1) * .5f / 10, topLeftCorner.y - (selectedField / 9) * .5f / 10, 0);
				glVertex3f (topLeftCorner.x + ((selectedField % 9) + 1) * .5f / 10, topLeftCorner.y - (selectedField / 9 + 1) * .5f / 10, 0);
				glVertex3f (topLeftCorner.x + (selectedField % 9) * .5f / 10, topLeftCorner.y - (selectedField / 9 + 1) * .5f / 10, 0);
			glEnd ();
		}

		float shade = 1.0f - ((tickCount - tickCountYesNoDone) - delay * .5f * 19 / 10) / 500;
		if (shade < 0.0f)
			shade = 0.0f;
		glColor3f (shade, shade, shade);
		writeNumbers ();
		verdana20.write (topLeftCorner.x, topLeftCorner.y - .5f - .01f, "Click the fields and enter the numbers.");
		verdana20.write (topLeftCorner.x, topLeftCorner.y - .5f - .01f - .025f, "(0 to delete.)");
		verdana20.write (topLeftCorner.x, topLeftCorner.y - .5f - .01f - 2 * .025f, "Press return when done.");

		return currentStep;
	}

	glColor3f (0.0f, 0.0f, 0.0f);
	writeNumbers ();

	float shade = 1.0f - (tickCount - tickCountInitDone) / 500.0f;
	if (shade < 0.0f)
		shade = 0.0f;
	glColor3f (shade, shade, shade);
	verdana20.write (topLeftCorner.x, topLeftCorner.y - .5f - .01f, "Usage:");
	verdana20.write (topLeftCorner.x, topLeftCorner.y - .5f - .01f - .025f, "Cursor left/right: Previous/Next step");
	verdana20.write (topLeftCorner.x, topLeftCorner.y - .5f - .01f - 2 * .025f, "Home: Back to initial state");
	verdana20.write (topLeftCorner.x, topLeftCorner.y - .5f - .01f - 3 * .025f, "End/Return: Solution");

	if (tickCount <= tickCountInitDone + 1000)
		return currentStep;

	if (currentStep == previousStep + 1)
	{
		if (currentStep == 0)
			currentStep = 1;

		startingStep = currentStep;

		if (currentStep > maxStep + 1)
			currentStep = maxStep + 1;
		else
		{
			if (newNumberHistory [currentStep] != noNewNumber && newNumberHistory [currentStep] != unsolvable)
				numbers [newNumberHistory [currentStep]] = getNumber (numbersInternHistory [currentStep][newNumberHistory [currentStep]]);

			previousStep = currentStep;

			stepForward = true;
		}
	}
	else if (currentStep == previousStep - 1)
	{
		if (currentStep == maxStep)
			currentStep = maxStep - 1;

		startingStep = currentStep;

		if (currentStep < -1)
			currentStep = -1;
		else
		{
			if (newNumberHistory [currentStep + 1] != noNewNumber && newNumberHistory [currentStep + 1] != unsolvable)
				numbers [newNumberHistory [currentStep + 1]] = '0';

			previousStep = currentStep;

			stepForward = false;
		}
	}
	else if (currentStep == previousStep)
	{
		int step = stepForward ? currentStep : currentStep + 1;

		if (newNumberHistory [step] == noNewNumber)
			for (int i = 0; i < 81; i++)
			{
				numbers [i] = getNumber (numbersInternHistory [currentStep][i]);
				glColor3f (0.0f, 0.0f, 0.0f);
				writeNumber (i, currentStep);
			}
		else if (newNumberHistory [step] == unsolvable)
		{
			glColor3f (0, 0, 0);
			verdana20.write (topLeftCorner.x + .148f, topLeftCorner.y - .5f - .01f - 5 * .025f, "UNSOLVABLE");
		}
		else if (newNumberHistory [step] != initialNumbers)
		{
			float shade;

			if (stepForward)
			{
				shade = 1.0f - (tickCount - tickCountPreviousStep) / 300.0f;
				if (shade < 0.0f)
					shade = 0.0f;
			}
			else
			{
				shade = (tickCount - tickCountPreviousStep) / 300.0f;
				if (shade > 1.0f)
					shade = 1.0f;
			}

			glColor3f (shade, shade, shade);

			if (startingStep < currentStep - 1)
			{
				for (int i = startingStep + 1 > 0 ? startingStep + 1 : 1; i <= currentStep; i++)
					writeNumber (newNumberHistory [i], i);
			}
			else if (startingStep > currentStep + 1)
			{
				for (int i = startingStep; i >= (currentStep > 0 ? currentStep : 1); i--)
					writeNumber (newNumberHistory [i], i);
			}
			else
			{
				writeNumber (newNumberHistory [step], step);
			}
		}
	}
	else if (currentStep == maxStep)
	{
		startingStep = previousStep;

		for (int i = previousStep + 1; i <= currentStep; i++)
			numbers [newNumberHistory [i]] = getNumber (numbersInternHistory [i][newNumberHistory [i]]);

		previousStep = currentStep;

		stepForward = true;
	}
	else if (currentStep == 0)
	{
		startingStep = previousStep;

		if (startingStep < 0)
			startingStep = 0;
		if (startingStep > maxStep)
			startingStep = maxStep;

		for (int i = previousStep; i >= currentStep; i--)
			numbers [newNumberHistory [i]] = '0';

		previousStep = currentStep;

		stepForward = false;
	}

	return currentStep;
}

void Sudoku::writeNumbers ()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			char temp [2];
			temp [0] = numbers [9 * i + j];
			temp [1] = '\0';

			if (temp [0] != '0')
				verdana30.write (topLeftCorner.x + j * 0.5f / 10 + 0.0165f, topLeftCorner.y - i * .5f / 10 - 0.035f, temp);
		}
}

void Sudoku::writeNumber (int pos, int step)
{
	char temp [2];
	temp [0] = getNumber (numbersInternHistory [step][pos]);
	temp [1] = '\0';

	verdana30.write (topLeftCorner.x + (pos % 9) * 0.5f / 10 + 0.0165f, topLeftCorner.y - (pos / 9) * .5f / 10 - 0.035f, temp);
}

void Sudoku::convertNumbers ()
{
	for (int i = 0; i < 81; i++)
	{
		numbersIntern [i] = (unsigned int) numbers [i] - '0';
		if (numbersIntern [i] != 0)
		{
			if (1 <= numbersIntern [i] && numbersIntern [i] <= 9)
			    numbersIntern [i] = ~(1 << (numbersIntern [i] - 1));
	    }
	}
}

int Sudoku::numberZeros (unsigned int numbersIntern_element)
{
	int n = 0;

	for (int j = 512; j > 1; j >>= 1)
		if (((j >> 1) & numbersIntern_element) == 0)
			n++;

	return n;
}

bool Sudoku::numberIsSolved (unsigned int numbersIntern_element)
{
	if (numberZeros (numbersIntern_element) == 1)
		return true;
	else
		return false;
}

char Sudoku::getNumber (unsigned int numbersIntern_element)
{
	unsigned int j = 512;
	char temp = '9';

	while (j > 1)
	{
		j >>= 1;
        if (((numbersIntern_element & 511) | j) == 511)
			j = 1;
        else
			temp--;
	}

	return temp;
}

void Sudoku::copyNumbersHistory (int step)
{
	for (int i = 0; i < 81; i++)
		numbersInternHistory [step][i] = numbersIntern [i];
}

void Sudoku::solve ()
{
	int step = 0;

	checked = false;

	convertNumbers ();
	newNumberHistory [step] = initialNumbers;
	copyNumbersHistory (step);

	while (step < 80 && newNumberHistory [step] != noNewNumber && newNumberHistory [step] != unsolvable)
	{
		step++;
		newNumberHistory [step] = solveStep ();
		copyNumbersHistory (step);
	}

	maxStep = step - 1;
}

int Sudoku::solveStep ()
{
	unsigned int i;
	unsigned int j;
	unsigned int k;
	unsigned int n;
	unsigned int z;
	unsigned int test [9];
	unsigned int qupos;
	unsigned int mask;
	unsigned int field [3];

	for (i = 0; i < 81; i++)
	{
		numbersIntern [i] &= 511;
		if (numbersIntern [i] == 0)
			marked [i] = false;
	    else
		{
			marked [i] = true;
			checked = true;
		}
	}

	while (checked)
	{
		checked = false;
		for (i = 0; i < 81; i++)
		{
			if (marked [i])
			{
				marked [i] = false;
				n = numberZeros (numbersIntern [i]);
				mask = (~numbersIntern [i]) & 511;

				for (z = 0; z <= 2; z++)
				{
					if (z == 0)
					{
						for (j = 0; j <= 8; j++)
						{
							test [j] = i / 9;
							test [j] *= 9;
							test [j] += j;
						}
					}
					else if (z == 1)
					{
						for (j = 0; j <= 8; j++)
							test [j] = 9 * j + i % 9;
					}
					else
					{
						qupos = i / 9;
						qupos /= 3;
						qupos *= 27;
						test [0] = (i % 9) / 3;
						test [0] *= 3;
						qupos += test [0];
						for (j = 0; j <= 2; j++)
						{
							test [3 * j]     = qupos + 9 * j;
							test [3 * j + 1] = qupos + 9 * j + 1;
							test [3 * j + 2] = qupos + 9 * j + 2;
						}
					}

					k = 0;
					for (j = 0; j <= 8; j++)
						if (numbersIntern [i] == numbersIntern [test[j]])
							k++;
					if (k > n)
						return unsolvable;
					if (k == n)
					{
						for (j = 0; j <= 8; j++)
						{
							if (numbersIntern [i] != numbersIntern [test[j]])
							{
								if ((numbersIntern [test[j]] | mask) != numbersIntern [test[j]])
								{
									numbersIntern [test[j]] = numbersIntern [test[j]] | mask;
									if (numbersIntern [test[j]] == 511)
										return unsolvable;
									marked [test[j]] = true;
									checked = true;
									if (numberIsSolved (numbersIntern [test [j]]))
										return test [j];
								}
							}
						}
					}
				}

				for (j = 0; j <= 8; j++)
				{
					n = 0;
					field [0] = field [1] = field [2] = 0;
					k = 0;
					while (k < 9)
					{
						if ((numbersIntern [test [k]] & (1 << j)) == 0)
						{
							if (n < 3)
							{
								field [n] = test [k];
								n++;
							}
							else
							{
								k = 10;
								n = 10;
				            }
						}
						k++;
					}
					if (n == 1)
					{
						if ((numbersIntern [field [0]] & 511) != (unsigned int)(~(1 << j) & 511))
						{
							numbersIntern [field [0]] = ~(1 << j) & 511;
							marked [field [0]] = true;
							checked = true;
							if (numberIsSolved (numbersIntern [field [0]]))
								return field [0];
						}
					}
					else if (n == 2)
						field [2] = field [1];
					if ((n == 3) || (n == 2))
					{
						if ( ((field [0] + 1) == field [1]) && ((n == 2) || ((n == 3) && (field [1] + 1) == field [2])) )
						{
							z = field [0] / 9;
							z *= 9;
							for (k = 0; k <= 8; k++)
							{
								if (((z + k) != field [0]) && ((z + k) != field [1]) && ((z + k) != field [2]))
								{
									if ((numbersIntern [z + k] | (1 << j)) != numbersIntern [z + k])
									{
										numbersIntern [z + k] = numbersIntern [z + k] | (1 << j);
										if (numbersIntern [z + k] == 511)
											return unsolvable;
										marked [z + k] = true;
										checked = true;
										if (numberIsSolved (numbersIntern [z + k]))
											return z + k;
									}
								}
							}
						}
						else if ( ((field [0] + 9) == field [1]) && ((n == 2) || ((n == 3) && (field [1] + 9) == field [2])) )
						{
							z = field [0] % 9;
							for (k = 0; k <= 8 * 9; k += 9)
							{
								if (((z + k) != field [0]) && ((z + k) != field [1]) && ((z + k) != field [2]))
								{
									if ((numbersIntern [z + k] | (1 << j)) != numbersIntern [z + k])
									{
										numbersIntern [z + k] = numbersIntern [z + k] | (1 << j);
										if (numbersIntern [z + k] == 511)
											return unsolvable;
										marked [z + k] = true;
										checked = true;
										if (numberIsSolved (numbersIntern [z + k]))
											return z + k;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return noNewNumber;
}

int Sudoku::getMaxStep ()
{
	return maxStep;
}

void Sudoku::setNumber (int pos, char number)
{
	if (pos >= 0 && pos <= 80)
		numbers [pos] = number;
}