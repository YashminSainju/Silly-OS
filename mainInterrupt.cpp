#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define nFlags 32

/* Define flag types. */
#define monitorPrompt 0

/* Define various character codes. */
#define null 0
#define backspace 8
#define tab 9
#define lineFeed 10
#define carraigeReturn 13
#define esc 27
#define upArrow 72

/* Define screen attributes. */
#define nLines 24
#define nCols 80

/*Define max characters for buffer*/
#define MAX_CHAR 81

/* Functions called. */
void bootStrap(void);
void bootMe(char *initfile);

void keyboardMe(void);
void screenMessage(char *screenString);
int monitorMe(char *commandLine);
void screenEdit(char *file);
bool showFile(char *file);
void showPrompt(void);

void GotoXY(HANDLE StdOut, SHORT x, SHORT y);

typedef struct node {
	char content[MAX_CHAR];
	int val;
	node *next;
	node *previous;
}node;

typedef struct list {
	node *Head;
	node *Tail;
	int lineNum;
}list;

/*Recently Added*/
void readFile(list *fileBuffer, char *readLine);
void saveFile(list *fileBuffer, char *fileName);
void addLine(list *fileBuffer, int atLine, char *line);
void addChar(list *fileBuffer, int atLine, char c, int x);
void printBuffer(list *fileBuffer);
void actionHistory(list * weezyBuffer, char * actionEntry);

/* Global variables. */
char keyBuffer[128];
list fileBuffer;
char strTmp[MAX_CHAR];
bool flags[nFlags];
list weezyBuffer;

void main(void)
{
	bool running;

	printf("Hello TV Land! \n");
	printf(">");

	/* Boot Strap. */
	bootStrap();

	/* Boot. */
	bootMe("os.ini");

	/* Interrupt Loop. */
	running = true;
	while (running) {
		/* Handle interrupts. */
		if (_kbhit()) {
			/* Call keyboard handler routine. */
			keyboardMe();
		}

		/* Process */
		if (flags[monitorPrompt]) {
			flags[monitorPrompt] = false;

			/* Send string to monitor process. */
			monitorMe(keyBuffer);
		}

		Sleep(25);
	}
}


void keyboardMe(void)  {
	char c;
	static int buffPtr = 0;

	/*weezyBuffer.Head = NULL;
	weezyBuffer.Tail = NULL;
	weezyBuffer.lineNum = 0;
	*/

	/* Get the key. */
	c = _getch();

	/* If user hits return key, flag monitor prompt. */
	if (c == carraigeReturn) {
		actionHistory(&weezyBuffer, keyBuffer);
		showPrompt();

		keyBuffer[buffPtr] = null;
		buffPtr = 0;

		flags[monitorPrompt] = true;
	}

	/*Here i will add code to traverse my newly and sexy list containing previous entries.*/
	/*else if (c == upArrow) {


	}
	*/

	else if (c == backspace) {
		printf("%c", c);
		printf(" ");
		printf("%c", c);

		buffPtr--;
	}
	else if ((c >= ' ') && (c <= '~')) {
		printf("%c", c);

		/* Load keyboard buffer. */
		if (buffPtr < 128) {
			keyBuffer[buffPtr] = c;
			buffPtr++;
		}

	}
}

void actionHistory(list * weezyBuffer, char * actionEntry) {
	int x = 0;
	//THE LIST IS EMPTY
	//so I 
	//created a new UNLINKED node.	
	node *newNode;
	newNode = (node *)malloc(sizeof(node));
	// populated content with the Command Line Entry. 
	strcpy(newNode->content, actionEntry);
	
	// if new node is the 1st node of the list 
	if (weezyBuffer->Head == NULL) {
		// Add new node to the list. Setting both the head and the tail of the list equal to the new node.
		weezyBuffer->Head = newNode;
		weezyBuffer->Tail = newNode;
		weezyBuffer->Tail->next = weezyBuffer->Head;
		//weezyBuffer->Tail->previous = weezyBuffer ->Head;

		x = 1;
		printf("%d",x);
		
	}
	// If this is not the first node then we need to set the lists tail equal to the 
	else {
		newNode->previous = weezyBuffer->Tail;
		weezyBuffer->Tail->next = newNode;
		weezyBuffer->Tail = newNode;
		weezyBuffer->Tail->next = weezyBuffer->Head;
		weezyBuffer->Head->previous = weezyBuffer->Tail;
		x = 2;
		printf("%d", x);
		
		
		

	}

}

void bootStrap(void)
{
	int j;

	/* Clear all flags. */
	for (j = 0; j<nFlags; j++) {
		flags[j] = false;
	}

	/* Clear all memory. */

}


void bootMe(char *initFile)
{
	/* Set type of job scheduling. */

	/* Set type of memory management. */

	/* Set type of disk management. */

	/* Set up shared memory for OS graphic monitor. */
	/* Start graphic monitor. */
}


void screenMessage(char *screenString)
{
	printf("%s \n", screenString);
	showPrompt();
}

int monitorMe(char *commandLine)
{
	int status = 0;
	char string[128];
	char eScreen[80][80];

	if (strncmp(commandLine, "edit", 4) == 0) {

		/* Call editor with filename. */
		strcpy(string, commandLine + 5);
		screenEdit(string);
	}

	/* File handling commands. */
	else if (strcmp(commandLine, "ls") == 0) {
		/* List files in current directory. */


	}
	else if (strcmp(commandLine, "cd") == 0) {
		/* Change directory. */
	}
	else if (strcmp(commandLine, "mkdir") == 0) {
		/* Change directory. */
	}
	else if (strncmp(commandLine, "type", 4) == 0) {
		/* Show the file named after the type command. */
		/* Call showFile with filename. */
		strcpy(string, commandLine + 5);
		showFile(string);
		//showPrompt();
	}
	else if (strncmp(commandLine, "clear", 5) == 0) {
		for (int j = 0; j<75; j++) {
			for (int k = 0; k<80; k++) {
				eScreen[j][k] = ' ';
			}
		}
		GotoXY(GetStdHandle(STD_OUTPUT_HANDLE), 0, 0);
		showPrompt();
	}

	/* Job execution commands. */

	/* Set compiler. */

	/* Set Jobs directory. */

	/* Run 1 job. */
	/*
	Create the process.
	Load it to memory
	Put pcb into readyQ
	Let scheduler take over the running of job.
	When complete, release any resources
	*/

	/* Run multiple jobs from directory. */
	/*
	As memory is available:
	Create the process.
	Load it to memory
	Put pcb into readyQ
	Let scheduler take over the running of job.
	When complete, release any resources.

	Keep loading jobs until user memory is full.

	If memory is full, the job load process must wait until jobs
	complete befor loading another job.

	Keep doing this until all jobs in the directory have been completed.

	*/

	return status;
}



/*
Here is a good start on an editor.


It needs the following at a minimum:


It needs to read in an input file, if it exists.
It needs a "save as" function.

It needs to handle tabs correctly.

backspace does not work quite right at the end of a line.

Right now the way you exit the editor is with the '~' key.  That
needs to change.  I think ctrl 's' should save, ctrl 'x' should exit.

Scrolling is not implemented, it needs to do that.

Inserting lines between lines is not implemented, nor is deleting of lines.
It should probably do that to.

Coloring of key programming words would be awesome, check the curseMain.cpp
program for examples of how to do that stuff.

etc.


*/
void screenEdit(char *file)
{
	/* Screen/cursor variables. */

	HANDLE hStdout;

	int j, k;

	int cx, cy;

	bool editing;

	char eScreen[80][80];
	unsigned char c;

	/* File input and output. */
	FILE  *in, *out;

	/* Get a handle for the standard output so we can control the cursor. */
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	/* Clear screen buffer. */
	for (j = 0; j<75; j++) {
		for (k = 0; k<80; k++) {
			eScreen[j][k] = ' ';
		}
	}

	/* Set cursor position. */
	cx = 0;
	cy = 0;
	GotoXY(hStdout, cx, cy);

	/*File Buffer variables*/

	fileBuffer.Head = NULL;
	fileBuffer.Tail = NULL;
	fileBuffer.lineNum = 0;


	/* Here we would load the screen from the file, if the file exists. */

	//read an input line in till it is at the end of the file
	in = fopen(file, "r");
	if (in != NULL) {
		while (fgets(strTmp, MAX_CHAR, in)) {
			readFile(&fileBuffer, strTmp);
			printf(strTmp);
			fileBuffer.lineNum = fileBuffer.lineNum + 1;
		}
		fclose(in);
	}
	else {
		/* Display data from array to screen, place cursor upper left. */
		for (j = 0; j<nLines; j++) {
			for (k = 0; k<nCols; k++) {
				putchar(eScreen[j][k]);
			}
		}
	}

	/* Go into edit loop */
	editing = true;
	/* Set cursor position. */
	cx = 0;
	cy = 0;
	GotoXY(hStdout, cx, cy);
	while (editing) {
		/* Edit functions. */
		if (_kbhit()) {
			/* Get the key. */
			c = _getch();

			/* Special keys, arrows, page up, etc. */
			if (c == 224) {

				c = _getch();

				switch (c)       
				{
					/* Arrow keys. */
				case 72:
					/* Code for up arrow handling */
					if (cy > 0) cy--;
					break;

				case 80:
					/* Code for down arrow handling */
					if (cy < (nLines - 1)) cy++;
					break;

				case 75:
					/* Code for left arrow handling */
					if (cx > 0) cx--;
					break;
				case 77:
					/* Code for right arrow handling */
					if (cx < (nCols - 1)) cx++;
					break;

					/* Page Up, Page Down, Home, End, Insert, Delete */
					/* 73		81			71	 79	  82	  83 */
				case 73:
					/* Code for Page Up */
					if (cy = 0) cx++;
					break;
				case 81:
					/* code for Page Down*/
					if (cy = (nLines - 1)) cy++;
					break;

				case 71:
				case 79:
					/* codes to bring cursor at the end of the line*/


				case 82:
					/* Insert the letter in place of replacing them*/
				case 83:
					/*deleting by letters*/
					break;

					/* ... etc ... */
				}
				GotoXY(hStdout, cx, cy);
			}
			else if (c == carraigeReturn) {
				/* Place char in buffer. */
				eScreen[cy][cx] = c;
				addChar(&fileBuffer, cy, c, cx);

				/* Handle cursor. */
				if (cy < (nLines - 1)) {
					cy++;
				}
				cx = 0;
				GotoXY(hStdout, cx, cy);
			}
			else if (c == backspace) {
				/* Place char in buffer. */
				eScreen[cy][cx] = ' ';
				addChar(&fileBuffer, cy, ' ', cx);

				/* Handle cursor. */
				printf("%c", c);
				printf(" ");
				printf("%c", c);
				if (cx > 0) cx--;
				GotoXY(hStdout, cx, cy);
			}
			else if (c == tab) {
				/* Handle position of cursor on screen. */
				cx = cx + 4;

				if (cx >= 80) {
					/* Load screen buffer. */
					eScreen[cy][cx] = carraigeReturn;
					addChar(&fileBuffer, cy, carraigeReturn, cx);
					cx = 0;
					cy++;
					if (cy == 80) {
						cy = 79;
					}
				}
				else {
					/* Load screen buffer. */
					for (j = cx; j<cx + 4; j++) {
						eScreen[cy][cx] = ' ';
						addChar(&fileBuffer, cy, ' ', cx);
					}
				}
				GotoXY(hStdout, cx, cy);

			}
			/* Common printed characters. */
			else if ((c >= ' ') && (c <= '}')) {
				printf("%c", c);


				/* Put char into screen buffer. */
				eScreen[cy][cx] = c;
				addChar(&fileBuffer, cy, c, cx);

				/* Handle position of cursor on screen. */
				cx++;

				if (cx == MAX_CHAR) {
					cx = 0;
					cy++;
					if (cy == 80) {
						cy = 79;
					}
				}
			}
			// Save file
			else if (c == 4) {
				saveFile(&fileBuffer, file);
				printf("file Saved");
			}
			/* Exit editor. */
			else if (c == 24) {
				fileBuffer.Head = NULL;
				fileBuffer.Tail = NULL;
				fileBuffer.lineNum = 0;
				printBuffer(&fileBuffer);
				editing = false;

				/* Clear screen. */

				for (j = 0; j<nLines; j++) {
					for (k = 0; k<nCols; k++) {
						putchar(' ');
					}
				}

				/* Set cursor position. */
				cx = 0;
				cy = 0;
				GotoXY(hStdout, cx, cy);
				showPrompt();
			}
		} /* End if _kbhit() */

		  /* Service system interrupts. */
		else {

		}

		Sleep(50);
	} /* End while editing. */

	  /* If needed, save contents. */
}


/* Set current cursor position. */
void GotoXY(HANDLE StdOut, SHORT x, SHORT y)
{
	// Set the cursor position.
	COORD Cord;
	Cord.X = x;
	Cord.Y = y;
	SetConsoleCursorPosition(StdOut, Cord);
}


bool showFile(char *fName)
{
	bool itWorked = true;
	char line[128];

	FILE *myFile;

	if ((myFile = fopen(fName, "r")) == null) {
		itWorked = false;
	}
	else {
		printf("\n...%s...\n", fName);
		printf("\n");
		while (fgets(line, 80, myFile) != null) {
			puts(line);
		}
	}

	showPrompt();

	return itWorked;
}


void showPrompt(void)
{
	printf("\n>");
}


/* Read files and add lines through file buffer*/
void readFile(list *fileBuffer, char *readLine) {
	node *newNode;
	newNode = (node *)malloc(sizeof(node));
	strcpy(newNode->content, readLine);
	newNode->next = NULL;
	// if 1st node
	if (fileBuffer->Head == NULL) {
		fileBuffer->Head = newNode;
		fileBuffer->Tail = newNode;
	}
	// if not 1st
	else {
		fileBuffer->Tail->next = newNode;
		fileBuffer->Tail = newNode;
	}
}

void saveFile(list *fileBuffer, char *fileName) {
	FILE *in;
	in = fopen(fileName, "w");
	if (in == NULL) {
		printf("Cannot find file with the filename.");
	}
	else {
		node *i;
		for (i = fileBuffer->Head; i != NULL; i = i->next) {
			fprintf(in, "%s", i->content);
		}
		fclose(in);
	}
}

void addLine(list *fileBuffer, int atLine, char *line) {
	node *i;
	int counter = 1;

	node *newNode;
	newNode = (node*)malloc(sizeof(node));
	strcpy(newNode->content, line);
	newNode->next = NULL;

	if (atLine == 0 && fileBuffer->Head == NULL && fileBuffer->Tail == NULL) {
		newNode->next = fileBuffer->Head;
		fileBuffer->Head = newNode;
		fileBuffer->Tail = newNode;
	}
	else if (atLine == 0) {
		newNode->next = fileBuffer->Head;
		fileBuffer->Head = newNode;
	}
	else {
		for (i = fileBuffer->Head; i != NULL; i = i->next) {
			if (counter == atLine) {
				newNode->next = i->next;
				i->next = newNode;
			}
			counter++;
		}

		if (atLine == fileBuffer->lineNum) {
			fileBuffer->Tail = newNode;
		}
	}
	fileBuffer->lineNum = fileBuffer->lineNum + 1;
}

/*Problems with addChar: Does not consider backspaces/ change of lines while typing*/

void addChar(list *fileBuffer, int atLine, char c, int x) {
	node *i;
	node *newNode;
	int counter = 0;

	newNode = (node *)malloc(sizeof(node));
	newNode->content[0] = c;
	newNode->next = NULL;

	if (fileBuffer->Head == NULL) {
		fileBuffer->Head = newNode;
		fileBuffer->Tail = newNode;
	}
	else {
		for (i = fileBuffer->Head; i != NULL; i = i->next) {
			if (counter == atLine) {
				//go to that character
				//replace the character with c
				i->content[x] = c;
				if (i->content[x + 1] < ' ' || i->content[x + 1] > '}') {
					i->content[x + 1] = '\0';
				}
			}
			counter++;
		}
	}
}

void printBuffer(list *fileBuffer) {
	node*i;
	i = fileBuffer->Head;
	while (i != null) {
		printf(i->content);
		i = i->next;
	}
}

