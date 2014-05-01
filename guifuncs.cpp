#include "prog4.h"

// global vars
int ScreenWidth = 600;
int ScreenHeight = 300;

// various commands to initialize OpenGL and GLUT
void initOpenGL( void )
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );	// 32-bit graphics and single buffering

    glutInitWindowSize( ScreenWidth, ScreenHeight );	// initial window size
    glutInitWindowPosition( 100, 50 );			// initial window  position
    glutCreateWindow( "prog4 GUI output" );			// window title

    glClearColor( 1.0, 1.0, 1.0, 1.0 );			// use white for glClear command

    // callback routines
    glutDisplayFunc( display );				// how to redisplay window
    glutReshapeFunc( reshape );				// how to resize window
    glutKeyboardFunc( NULL );			// how to handle key presses
    glutMouseFunc( NULL );			// how to handle mouse events
}

/******************************************************************************/
/*                          OpenGL callback functions                         */
/******************************************************************************/

// callback function that tells OpenGL how to redraw window
void display( void )
{
    // clear the display
  glClear( GL_COLOR_BUFFER_BIT );

    // flush graphical output
    glFlush();
}

/******************************************************************************/

// callback function that tells OpenGL how to resize window
// note that this is called when the window is first created
void reshape( int w, int h )
{
    // store new window dimensions globally
    ScreenWidth = w;
    ScreenHeight = h;

    // how to project 3-D scene onto 2-D
    glMatrixMode( GL_PROJECTION );		// use an orthographic projection
    glLoadIdentity();				// initialize transformation matrix
    gluOrtho2D( 0.0, w, 0.0, h );		// make OpenGL coordinates
    glViewport( 0, 0, w, h );			// the same as the screen coordinates
}


/******************************************************************************/
/*                          useful graphics routines                          */
/******************************************************************************/

// draw a line using the given RGB color value
void DrawLine( float x1, float y1, float x2, float y2, const float color[] )
{
    // glLineWidth( 10 );
    glColor3fv( color );
    glBegin( GL_LINES );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y2 );
    glEnd();
    glFlush();
}

/******************************************************************************/

// draw a rectangle using the given RGB color value
void DrawRectangle( float x1, float y1, float x2, float y2, const float color[] )
{
    glColor3fv( color );
    glBegin( GL_LINE_LOOP );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y1 );
        glVertex2f( x2, y2 );
        glVertex2f( x1, y2 );
    glEnd();
    glFlush();
}

/******************************************************************************/

// draw a filled rectangle using the given RGB color value
void DrawFilledRectangle( float x1, float y1, float x2, float y2, const float color[] )
{
    glColor3fv( color );
    glBegin( GL_POLYGON );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y1 );
        glVertex2f( x2, y2 );
        glVertex2f( x1, y2 );
    glEnd();
    glFlush();
}

/******************************************************************************/

// write a text string at a specified position using the given RGB color value
void DrawTextString( char *string, int x, int y, const float color[] )
{
    glColor3fv( color );
    glRasterPos2i( x, y );
    while ( *string )
	glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *string++ );
}
