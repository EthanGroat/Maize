/*
						***Platform Agnosticism***

This header defines methods which work to the same effect across all operating 
systems, methods which normally would be handled by simple system commands and 
the like, allowing compiling on both Windows and Linux/Unix-based (POSIX) 
machines.
It also includes cstdlib, necessary for linux commands, and of course windows.h
*/

#pragma once
#include <cstdlib>
#ifdef _WIN32 // OS-specific header files
	#include <windows.h>
#else
	#include <unistd.h>
// put in an if/else here because Visual Studio could not find this header
#endif

#include <windows.h>
#include <iostream>
using namespace std;

/*	note to self:
implement this header in other programs too
*/

// function to clear screen defined for Windows & Linux/Unix
void clearScreen()
{
#ifdef _WIN32
	system("cls");		// Windows
#else
	system("clear");	// Linux/Unix
#endif
}


// This replaces system("pause"); although, requiring the enter key:
void pause()
{
	cout << endl << "Press ENTER to continue... ";
	cin.ignore();
}
// this is not super proffessional because it prints anything typed.
// more research on this, I must do.


// Non-explicit version of "press enter to continue":
void shortPause()
{
	cin.ignore();
}


// for console resizing:
void setConsoleSize(int width, int height)
{

#ifdef _WIN32 // this part only works in Windows and requires windows.h

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, width, height, TRUE);
	// width, height are 4th and 5th parameters

	// #else
		// put Linux equivalent here
		// (or leave as is, either will work)

#endif // _WIN32

	return;
}


// for cursor position:
void cursorPos(int x, int y)
{
#ifdef _WIN32
	// COORD pos = { x, y };
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!SetConsoleCursorPosition(output, pos))	// call & check for errors
	{
		cerr << "SetConsoleCursorPosition has failed with code ";
		cerr << GetLastError() << " - probably out of bounds.\n";
		/* use this code to check bounds:
		CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleScreenBufferInfo);
		cout << "x: " << ConsoleScreenBufferInfo.dwSize.X << endl;
		cout << "y: " << ConsoleScreenBufferInfo.dwSize.Y << endl;*/
	}
#else // Linux/UNIX (POSIX)
	printf("\033[%d;%dH", y + 1, x + 1);
	// ANSI compatible terminal only
	// (Should usually work on Linux/UNIX systems, doesn't work on Windows)
/*
used here:
----------------------
ANSI escape sequences!
----------------------
- Position the Cursor:
	\033[<L>;<C>H
		 Or
	\033[<L>;<C>f
		puts the cursor at line L and column C.
- Move the cursor up N lines:
	\033[<N>A
- Move the cursor down N lines:
	\033[<N>B
- Move the cursor forward N columns:
	\033[<N>C
- Move the cursor backward N columns:
	\033[<N>D

- Clear the screen, move to (0,0):
	\033[2J
- Erase to end of line:
	\033[K
  


The most common console escape codes are 

\a, \b, \t, \n, \r.

They are, in order:

\a	audible beep, 
\b	go back one space (but not up a line), 
\t	tab, 
\n	new line, 
\r	return [to beginning of line]. 

There’s also \v for vertical tab and \f for form feed, but these might not do anything useful.


 escape sequence reference:
	https://www.quora.com/How-can-you-move-back-up-a-line-in-c++
	http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
*/
#endif
}


// time delay:
void delay(int microseconds)
{
#ifdef _WIN32
	unsigned int u = (unsigned int)microseconds;
	Sleep(u);

#else // POSIX
	usleep(microseconds);
// requires #include <unistd.h>

#endif
}
