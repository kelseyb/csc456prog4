#include <iostream>
#include <string>
#include <cstdlib>
#include <GL/freeglut.h>
#include <time.h>
#include <vector>
#include <ctime>
#include <stdio.h> 
#include <stdlib.h>
#include <sstream>

using namespace std;

//Process scheduling
int process_scheduling();

//Memory Management
void memory_simulation();


//Page Replacement



//GUI/GL stuff

// symbolic constants

// keypresses
const int EscapeKey = 27;
// global vars
extern int ScreenWidth;
extern int ScreenHeight;

extern int* superargc;
extern char*** superargv;

// colors
const float Black[]	= { 0.0, 0.0, 0.0 };
const float Red[]	= { 1.0, 0.0, 0.0 };
const float Green[]	= { 0.0, 1.0, 0.0 };
const float Blue[]	= { 0.0, 0.0, 1.0 };
const float Magenta[]	= { 1.0, 0.0, 1.0 };
const float Cyan[]	= { 0.0, 1.0, 1.0 };
const float Yellow[]	= { 1.0, 1.0, 0.0 };
const float White[]	= { 1.0, 1.0, 1.0 };


// OpenGL callback function prototypes
void display( void );
void reshape( int w, int h );


// useful graphics function prototypes
void initOpenGL( void );
void DrawLine( float x1, float y1, float x2, float y2, const float color[] );
void DrawRectangle( float x1, float y1, float x2, float y2, const float color[] );
void DrawFilledRectangle( float x1, float y1, float x2, float y2, const float color[] );
void DrawTextString( char *string, int x, int y, const float color[] );


/******************************************************/

void pagereplace( );
void fifo( int numpages, int numframes, int numref );
void optimal( int numpages, int numframes, int numref );
void lru( int numpages, int numframes, int numref );
void lfu( int numpages, int numframes, int numref );
void secondchance( int numpages, int numframes, int numref );
void superclock( int numpages, int numframes, int numref );
int minrefnum ( vector<struct virtpage> &frametable );
int maxrefnum ( vector<struct virtpage> &frametable );
int inframetable ( int findpage, vector<struct virtpage> &frametable );
void printframetable ( vector<struct virtpage> &frametable );
void fillpagetable ( vector<struct virtpage> &pagetable, int numpages );
void fillreftable ( vector<int> &reftable, int numref, int numpages );


struct virtpage
{
  int page ;
  int frame ;
  int refnum ;
};




