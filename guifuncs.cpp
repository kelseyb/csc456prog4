// various commands to initialize OpenGL and GLUT
void initOpenGL( void )
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );	// 32-bit graphics and single buffering

    glutInitWindowSize( ScreenWidth, ScreenHeight );	// initial window size
    glutInitWindowPosition( 100, 50 );			// initial window  position
    glutCreateWindow( "OpenGL Demo" );			// window title

    glClearColor( 0.0, 0.0, 0.0, 0.0 );			// use black for glClear command

    // callback routines
    glutDisplayFunc( display );				// how to redisplay window
    glutReshapeFunc( reshape );				// how to resize window
    glutKeyboardFunc( keyboard );			// how to handle key presses
    glutMouseFunc( mouseclick );			// how to handle mouse events
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

// callback function that tells OpenGL how to handle keystrokes
void keyboard( unsigned char key, int x, int y )
{
    // correct for upside-down screen coordinates
    y = ScreenHeight - y;
    cerr << "keypress: " << key << " (" << int( key ) << ") at (" << x << "," << y << ")\n";

}

/******************************************************************************/

// callback function for mouse button click events
void mouseclick( int button, int state, int x, int y )
{
    // correct for upside-down screen coordinates
    y = ScreenHeight - y;

    // handle mouse click events
    switch ( button )
    {
	case GLUT_LEFT_BUTTON:				// left button: should create objects
	    if ( state == GLUT_DOWN )			// press
		cerr << "mouse click: left press at    (" << x << "," << y << ")\n";
	    else if ( state == GLUT_UP )		// release
		cerr << "mouse click: left release at  (" << x << "," << y << ")\n";
	    break;

	case GLUT_RIGHT_BUTTON:				// right button: should move objects
	    if ( state == GLUT_DOWN )			// press
		cerr << "mouse click: right press at   (" << x << "," << y << ")\n";
	    else if ( state == GLUT_UP )		// release
		cerr << "mouse click: right release at (" << x << "," << y << ")\n";
	    break;
    }
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
