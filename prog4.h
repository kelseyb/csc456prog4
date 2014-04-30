#include <iostream>
#include <string>
#include <GL/glut.h>

using namespace std;

//Process scheduling
//void priority(process p[], int proc_count);
//void shortest_job_first(process p[], int proc_count, int time_segment);
//void round_robin(process p[], int proc_count, int time_segment);
int process_scheduling();

//Memory Management



//Page Replacement



//GUI/GL stuff

//symbolic constants

//keypresses
const int EscapeKey = 27;

//colors
const float Black[]	= { 0.0, 0.0, 0.0 };
const float Red[]	= { 1.0, 0.0, 0.0 };
const float Green[]	= { 0.0, 1.0, 0.0 };
const float Blue[]	= { 0.0, 0.0, 1.0 };
const float Magenta[]	= { 1.0, 0.0, 1.0 };
const float Cyan[]	= { 0.0, 1.0, 1.0 };
const float Yellow[]	= { 1.0, 1.0, 0.0 };
const float White[]	= { 1.0, 1.0, 1.0 };

//global vars
int ScreenWidth  = 900;
int ScreenHeight = 600;

//OpenGL callback function prototypes
void display( void );
void reshape( int w, int h );
void keyboard( unsigned char key, int x, int y );
void mouseclick( int button, int state, int x, int y );
void mousedrag( int x, int y );

//useful graphics function prototypes
void initOpenGL( void );
void DrawLine( float x1, float y1, float x2, float y2, const float color[] );
void DrawRectangle( float x1, float y1, float x2, float y2, const float color[] );
void DrawFilledRectangle( float x1, float y1, float x2, float y2, const float color[] );
void DrawTextString( char *string, int x, int y, const float color[] );



