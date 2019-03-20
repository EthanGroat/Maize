/*

March 2018

	    - Ethan Groat's -
	   -------------------
	+++++ MAIZE PROJECT +++++
	   -------------------
	  CS M20 - Prof. Alnaji

				Data Struct & Alg

*/

// Program to display a maze which proceeds to solve itself

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "CrossPlatform.h"  // REMEMBER THE QUOTES!!!!!!!!!!!!! POS

using namespace std;



// global variables to use across definitions:
ifstream inFile;	// input file stream
string maze;		// string to contain entire unsolved maze

struct coordinates
{
	int x;
	int y;
	void coordinates::operator =(const coordinates &c)
	{
		x = c.x;
		y = c.y;
	}
};

// Data type to hold an operable version of the maze for the solving algorithm
// (It uses a two-dimensional vector, in case different sized mazes are used.)
class MazeMatrix
{
	// vect holds all the walls, free spaces, visited locations, and 
	// start and finish points of the maze, whereas bread crumbs 
	// will be stored onscreen and as a stack of coordinates.
	// Note that first coordinate is row and second is column.
	vector< vector<char> > vect;

	coordinates start;	// for quick access at the beginning of solve()
	coordinates current; // for current position in matrix/onscreen
	stack<coordinates> crumbs;	//stack of coordinates for backtracking

	// parses a given string into 2-D vector char by char, line by line:
	void parse(string maze)
	{
		int i = 0;
		int row = 0;
		vect.push_back(vector<char>()); // push back initial row
		while (i < maze.size())
		{
			if (maze[i] != char(10))
			{
				vect[row].push_back(maze[i]); // add characters to the row
				
				// Assign start coordinate:
				if (maze[i] == 'S')
				{
					start.x = vect[row].size()-1;
					start.y = row;
				}
			}
			else
			{
				vect.push_back(vector<char>()); // push back another row
				row++;	// iterate row index
			}
			i++;	// iterate string index
		}
	} // result should be 2-D vector of chars with all characters except newline

	// checks available moves and advances to one of them
	void advance()
	{
		int x = current.x;
		int y = current.y;
		
		// Note: left and right must be 2 spaces the way the maze is formatted.
		
		// check up
		if (vect[y - 1][x] == ' ' || vect[y - 1][x] == 'E')
		{
			current.y -= 1;		// advance up
			crumbs.push(current);
		}
		// check left
		else if (vect[y][x - 2] == ' ' || vect[y][x - 2] == 'E')
		{
			current.x -= 2;		// advance left
			crumbs.push(current);
		}
		// check down
		else if (vect[y + 1][x] == ' ' || vect[y + 1][x] == 'E')
		{
			current.y += 1;		// advance down
			crumbs.push(current);
		}
		// check right
		else if (vect[y][x + 2] == ' ' || vect[y][x + 2] == 'E')
		{
			current.x += 2;		// advance right
			crumbs.push(current);
		}
		else		// nothing available and unvisited
		{
			crumbs.pop();	// the order of these is critical!
			cursorPos(current.x, current.y);
			putchar(' ');
			current = crumbs.top();	// advance to previous visited position
		}
	}

public:
	void display()
	{
		clearScreen();
		for (int row = 0; row < vect.size(); row++)
		{
			for (int c = 0; c < vect[row].size(); c++)
			{
				cout << vect[row][c];
			}
			cout << endl;
		}
	}

	void solve(int time)
	{
	  // Outline of Algorithm:
	  //
		// begin at 'S';
		// crumbs.push(start);
		// while (!'E' at current position in MazeArray M):
			// put '-' onscreen;  sidenote: put a face like " '-' "
			// record 'v' at current position in M;
			// time delay;
			// erase face, if used;
			//
			// advance:
			//	(check available routes in M & proceed to first condition)
				// 1: up == ' '||'E'	-> current = up;	crumbs.push(current)
				// 2: left == ' '||'E'	-> current = left;	crumbs.push(current)
				// 3: down == ' '||'E'	-> current = down;	crumbs.push(current)
				// 4: right == ' '||'E'	-> current = right;	crumbs.push(current)
				// 5: nothing is ' '||'E'	-> backtrack
										//	crumbs.pop();
										//	put ' ';
										//	current = crumbs.top();
		// solved -> mark the endpoint
		

		// debugging notes:

		// test to find S:  verified
		// cout << vect[start.y][start.x];

		// boundary & parsing test:  verified
		// cout << "x: " << start.x << endl;
		// cout << "y: " << start.y << endl;
		// cout << "looking for char " << int(maze[110]) << endl; (newline char)
			// it's char (10)


		// Algorithm! See Outline of Algorithm above.
		cursorPos(start.x, start.y);
		current = start;
		crumbs.push(start);
		delay(1000);
		while (vect[current.y][current.x] != 'E')
		// for (int i=0; i<50; i++)  // loop for testing small portions
		{
			cursorPos(current.x,current.y);
			putchar('-');
			vect[current.y][current.x]='v';
			delay(time);		// speed of solver
			advance();
		}
		cursorPos(current.x, current.y);
		putchar('x'); // x marks the spot

	}

public:
	// default constructor
	MazeMatrix() {}

	// initialized with parse(string maze)
	MazeMatrix(string maze)
	{
		parse(maze);
	}
};


// reads through the text file char by char & returns the resultant string
string readMaze()
{
	string m;			// string to hold entire file text temporarily
	char x = ' ';	// character used to iterate through the file stream
	while (x != EOF)
	{
		x = inFile.get();	// acquire a single character, INCLUDING whitespace!
		m += x;				// add it to the maze string
	}

	inFile.close();		// frees up maze file for other programs

	return m;
}

// simply displays the string of the unsolved maze in memory
void displayUnsolvedMaze()
{
	clearScreen();
	cout << maze;
}

// for fancy text
void animate(string s, int usecond_delay)
{
	for (int i = 0; i < s.size(); i++)
	{
		delay(usecond_delay);
		cout << char(s[i]);
	}
}


int main()
{
	setConsoleSize(960,576); // good for font size up to 18
	// setConsoleSize(1280, 720); // good for font 24

	// open maze text:
	inFile.open("maze0.txt");
	// use maze with similar formatting: # # # or + + + or = = = for wall, 
	// INCLUDING SPACES, 
	// S for start, E for end,
	// enclosed in a layer of walls to show boundaries.

	if (!inFile) cerr << "Oops!";	// check for file read errors

	maze = readMaze();
	MazeMatrix M = MazeMatrix(maze);

	M.display();	// equivalent to displayUnsolvedMaze(); at this point
	delay(700);
	animate("Ethan's 'MAIZE PROJECT'", 150); // fancy titlescreen
	delay(1000);
	cout << endl << endl;

	// check window size and wait for user to press ENTER:
#ifdef _WIN32 // Windows console size will work if font is decently sized
	animate("Press ENTER to solve the maze! ", 70);
#else // POSIX may require manual resizing of terminal window.
	cout << "Resize terminal if needed ";
	animate("\n\tand press ENTER to solve the maze!", 70);
#endif
	shortPause();

	M.display(); // clear the previous lines by refreshing the screen


	M.solve(100);  // calls main algorithm with step delay parameter


	shortPause();
	//cout << endl;
	//pause();	// instead of system("pause"); see CrossPlatform.h
	return 0;
}