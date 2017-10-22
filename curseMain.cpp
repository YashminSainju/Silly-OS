#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;

/* Constants. */
const int NUM_LINES = 22;
const int NUM_COLS = 80;
const int MANUAL = 1;
const int AUTO = 2;

/* Globals. */
/* The screen variable holds the enviornment map for us. */
char screen[NUM_LINES][NUM_COLS];


/* Prototypes. */
void GotoXY( HANDLE StdOut, SHORT x, SHORT y );
void PrintTime(void);
void loadScreenFromFile(char *fname);
void showScreen(int score, HANDLE hStdout);
void addFood2Level(void);
void showGameData(int score, HANDLE hStdout);
void updateScore(int score, HANDLE hStdout);
void drawCharOnScreen(char c, HANDLE hStdout);
char pacDudeBrain(int x, int y, char ateSome);
int findSmellyArea(int x, int y, char dir);


// Set current cursor position.
void GotoXY( HANDLE StdOut, SHORT x, SHORT y )
{
    // Set the cursor position.
    COORD Cord;
    Cord.X = x;
    Cord.Y = y;
    SetConsoleCursorPosition( StdOut, Cord );
}

// Print time at the upper right corner of console.
void PrintTime(void)
{
    // Get handle to console output buffer.
    HANDLE hStdout = GetStdHandle( STD_OUTPUT_HANDLE );

    // Get current screen information.
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo = { 0 };
    GetConsoleScreenBufferInfo( hStdout, &ScreenBufferInfo );

    // Set the cursor position to upper-right of console.
    GotoXY( hStdout, 54, 23 );

    // Get time and display it.
    time_t tim=time(NULL);
    char *s=ctime(&tim);
    cout << s;

    // Reset cursor back to position.
    GotoXY( hStdout,
            ScreenBufferInfo.dwCursorPosition.X,
            ScreenBufferInfo.dwCursorPosition.Y );
}

void main(int argc, char* argv[])
{
	char looping, ch, mode;
	char ateSome;
	int x, y, x0, y0;
	int score;


	HANDLE hStdout;

	/* Set up the random number generator. */
	srand( (unsigned)time( NULL ) );
	
	/* Initialize postion of pac dude. */
	x = 40;
	y = 10;
	x0 = x;
	y0 = y;
	score = 0;
	mode = MANUAL;

	/* Get a handle for the standard output so we can control the cursor. */
	hStdout = GetStdHandle( STD_OUTPUT_HANDLE );
	
	/* Load the screen configuration from the file. */
	loadScreenFromFile("screen test.txt");
	/* Populate the world with food. */
	addFood2Level();
	/* Display the screen. */
	showScreen(score, hStdout);

	/* Set the position under Pac-dude to be foodless so the score count
	   correctly. */
	screen[y][x] = ' ';

	/* Show pac-dude for the first time. */
	GotoXY(hStdout, x, y);
	drawCharOnScreen((char)1, hStdout);

	looping = true;
	while(looping) {

		if (kbhit()) {
			ch = getch();
		
			switch(ch)
			{
			/* Key board controls. */
			case 'i':
				if (y > 0) {
					y--;
				}
				break;

			case 'j':
				if (x > 0) {
					x--;
				}
				break;
			case 'k':
				if (x < 79) {
					x++;
				}
				break;
			case 'm':
				if (y < 24) {
					y++;
				}
				break;
			/* Toggle between auto and manual mode. */
			case 'a':
				if (mode == MANUAL) {
					mode = AUTO;
				}
				else {
					mode = MANUAL;
				}
				break;

			/* Arrow key controls. */
			case 'K':
				if (x > 0) {
					x--;
				}
				break;
			case 'M':
				if (x < 79) {
					x++;
				}
				break;
			case 'P':
				if (y < 24) {
					y++;
				}
				break;
			case 'H':
				if (y > 0) {
					y--;
				}
				break;
			/* Game ending controls. */
			case 27:
				looping = false;
				break;
			}
		}
		else if (mode == AUTO) {
			ch = pacDudeBrain(x, y, ateSome);
			switch (ch) {
					case 'K':
				if (x > 0) {
					x--;
				}
				break;
			case 'M':
				if (x < 79) {
					x++;
				}
				break;
			case 'P':
				if (y < 24) {
					y++;
				}
				break;
			case 'H':
				if (y > 0) {
					y--;
				}
				break;
			}
		}


		/* We check the previous postion for these,
		   so the screen can be properly refreshed. */
		
		/* Check to see if he hit the ghost door. */
		if (screen[y0][x0] == '#') {
			GotoXY(hStdout, x0, y0);
			drawCharOnScreen('#', hStdout);
		}
		else {
			GotoXY(hStdout, x0, y0);
			printf("%c", ' ');
		}


		/* We check the current position for these so
		   pac-dude can be moved properly. */
		/* Check to see if he is try to go through a wall. */
		if (screen[y][x] == '*') {
			x = x0;
			y = y0;
		}
		/* Check for a transport to the other side of the screen. */
		else if (screen[y][x] == '>') {
			x = 1;
		}
		else if (screen[y][x] == '<') {
			x = 77;
		}
		/* Check to see if Pac-dude ate a dot. */
		if (screen[y][x] == '.') {
			score = score + 1;
			ateSome = TRUE;
			screen[y][x] = ' ';
		}
		else {
			ateSome = FALSE;
		}

		/* Update previous postion. */
		x0 = x;
		y0 = y;
		
		/* Draw the pac-dude. */
		GotoXY(hStdout, x, y);
		drawCharOnScreen((char)1, hStdout);

		/* Update the game score. */
		updateScore(score, hStdout);
		
		Sleep(10);
		PrintTime();
	}

	/* Go to bottom of screen to write the shutdown message. */
	GotoXY(hStdout, 0, 24);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


void loadScreenFromFile(char *fname)
{
	FILE *in;
	char line[100];
	int j, k;

	if (NULL == (in = fopen(fname, "r"))) {
		cout << "File ", fname, " not found";
		exit(0);
	}
	
	j=0;
	while(NULL != (fgets(line, 100, in))) {
		for(k=0;k<NUM_COLS;k++) {
			screen[j][k] = line[k];
		}
		screen[j][NUM_COLS - 1] = 0;
		j = j+1;
	}
}


void showScreen(int score, HANDLE hStdout) 
{
	int j, k;

	/* Draw the map in green. */
	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN|FOREGROUND_INTENSITY);

	/* Start at upper left corner of screen. */
	GotoXY(hStdout, 0, 0);
	/* Print the screen. */
	for(j=0;j<NUM_LINES;j++) {
		for(k=0;k<NUM_COLS;k++) {
			drawCharOnScreen(screen[j][k], hStdout);
		}
	}

	/* Reset the color to white. */
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	showGameData(score, hStdout);
}


void drawCharOnScreen(char c, HANDLE hStdout) 
{
	if (c == '*') {
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	}
	else if (c == '.') {
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	else if ((c == '#')||(c == '>')||(c == '<')) {
		SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
	}
	else if (c == (char)1) {
		SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	}

	cout << c;
}



void showGameData(int score, HANDLE hStdout)
{
	GotoXY(hStdout, 0, 23);
	cout<<"                                                            ";
	GotoXY(hStdout, 0, 23);
	cout<<"Press 'a' to toggle auto/manual. Pac-Dude score: " << score;
}

void updateScore(int score, HANDLE hStdout)
{
	GotoXY(hStdout, 49, 23);
	cout<<"    ";
	GotoXY(hStdout, 49, 23);
	cout<<score;
}




void addFood2Level(void)
{
	int j,k;

	for(j=0;j<NUM_LINES;j++) {
		for(k=0;k<NUM_COLS;k++) {
			if (screen[j][k] == ' ') {
				screen[j][k] = '.';
			}
		}
	}
}

char pacDudeBrain(int x, int y, char ateSome)
{
	char dirs[4] = {'K', 'M', 'P', 'H'};
	static char currDir = 'K';
	static int notAte = 0;
	char dir;
	

	dir = currDir;
	if (!ateSome) {
		notAte = notAte + 1;
		/* Get a random direction. */
		dir = dirs[rand()%4];
		currDir = dir;
	}
	else {
		notAte = 0;
	}

	return dir;
}


char smellDir(int x, int y)
{
	char dir;

	int nDist, sDist, eDist, wDist;

	nDist = findSmellyArea(x, y, 'n');
	sDist = findSmellyArea(x, y, 's');
	eDist = findSmellyArea(x, y, 'e');
	wDist = findSmellyArea(x, y, 'w');

	if ((nDist < sDist)&&(nDist < eDist)&&(nDist < wDist)) {
		dir = 'H';
	}
	else if ((sDist < nDist)&&(sDist < eDist)&&(sDist < wDist)) {
		dir = 'P';
	}
	else if ((eDist < nDist)&&(eDist < sDist)&&(eDist < wDist)) {
		dir = 'M';
	}
	else {
		dir = 'K';
	}

	return dir;
}


int findSmellyArea(int x, int y, char dir)
{
	int dist;
	bool looping, found;

	looping = true;
	found = false;
	dist = 0;

	switch (dir) {
	case 'n':
		while (looping) {
			if (screen[x][y] == '*') {
				looping = false;
			}
			else if (screen[x][y] == '.') {
				looping = false;
				found = true;
			}
			else {
				y = y-1;
				dist = dist+1;
			}
		}
		break;
	case 's':
		while (looping) {
			if ((screen[x][y] == '*')||(screen[x][y] == '<')||(screen[x][y] == '>')) {
				looping = false;
			}
			else if (screen[x][y] == '.') {
				looping = false;
				found = true;
			}
			else {
				y = y+1;
				dist = dist+1;
			}
		}
		break;
	case 'e':
		while (looping) {
			if ((screen[x][y] == '*')||(screen[x][y] == '<')||(screen[x][y] == '>')) {
				looping = false;
			}
			else if (screen[x][y] == '.') {
				looping = false;
				found = true;
			}
			else {
				x = x+1;
				dist = dist+1;
			}
		}
		break;
	case 'w':
		while (looping) {
			if ((screen[x][y] == '*')||(screen[x][y] == '<')||(screen[x][y] == '>')) {
				looping = false;
			}
			else if (screen[x][y] == '.') {
				looping = false;
				found = true;
			}
			else {
				x = x-1;
				dist = dist+1;
			}
		}
		break;
	}

	if (found == false) {
		dist = 999;
	}
	return dist;
}
	


