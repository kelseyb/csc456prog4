#include "memory_simulation.h"
#include "prog4.h"

//used to store data for GUI functions
vector < address > ptvec;
vector < address > memvec;
vector < pair < page, frame > > tlbvec;


///////////////////////////////////////////////////////////////////////////////
//
// draw_tlb function
//
// author: Shaun Gruenig, Hannah Aker, Kelsey Bellew
//
// description: This function call the appropriate GUI functions to create
//      a graphical representation of the translation look-aside
//      buffer.
//
//
///////////////////////////////////////////////////////////////////////////////
void draw_tlb ( void )
{
    glClear ( GL_COLOR_BUFFER_BIT );
    glutSetWindowTitle ( "TLB" );
    int i, yval;
    string t;
    stringstream out;
    char *f;
    int size = tlbvec.size();

    for ( i = 0; i < size; i++ )
    {
        yval = ScreenHeight - 30 * i - 20;

        out << ( bitset < sizeof ( data ) * 8 - PAGE_SIZE > ) tlbvec[i].first;
        t = out.str();
        f = ( char* ) t.c_str();
        DrawTextString ( f, 25, yval, Black );
        out.str ( "" );


        out << ( bitset < sizeof ( data ) * 8 - PAGE_SIZE > ) tlbvec[i].second;
        t = out.str();
        f = ( char* ) t.c_str();
        DrawTextString ( f, 125, yval, Black );
        out.str ( "" );


    }

    for ( i = 0; i < TLB_SIZE; i++ )
    {
        yval = ScreenHeight - 30 * i;
        DrawRectangle ( 0, yval, 100, yval - 30, Black );
        DrawRectangle ( 100, yval, 200, yval - 30, Black );
    }


    glFlush();

}

///////////////////////////////////////////////////////////////////////////////
//
// draw_pt function
//
// author: Shaun Gruenig, Hannah Aker, Kelsey Bellew
//
// description: This function call the appropriate GUI functions to create
//      a graphical representation of the page table.
//
//
///////////////////////////////////////////////////////////////////////////////
void draw_pt ( void )
{
    glClear ( GL_COLOR_BUFFER_BIT );
    glutSetWindowTitle ( "Page Table" );
    int i, yval;

    string t;
    stringstream out;
    char temp;
    char *f;
    int size = ptvec.size();

    for ( i = 0; i < size / 2; i++ )
    {
        yval = ScreenHeight - 30 * i - 20;
        out << i << "   ";
        out << ( bitset < sizeof ( data ) * 8 - PAGE_SIZE > ) ptvec[i];
        t = out.str();
        f = ( char* ) t.c_str();
        DrawTextString ( f, 2, yval, Black );
        out.str ( "" );
    }

    for ( i = size / 2; i < size; i++ )
    {
        yval = ScreenHeight - 30 * ( i - size / 2 ) - 20;
        out << i << "   ";
        out << ( bitset < sizeof ( data ) * 8 - PAGE_SIZE > ) ptvec[i];
        t = out.str();
        f = ( char* ) t.c_str();
        DrawTextString ( f, 122, yval, Black );
        out.str ( "" );
    }

    for ( i = 0; i < size; i++ )
    {
        yval = ScreenHeight - 30 * i;
        DrawRectangle ( 20, yval, 120, yval - 30, Black );
        DrawRectangle ( 140, yval, 240, yval - 30, Black );
    }


    glFlush();

}


///////////////////////////////////////////////////////////////////////////////
//
// draw_mem function
//
// author: Shaun Gruenig, Hannah Aker, Kelsey Bellew
//
// description: This function call the appropriate GUI functions to create
//      a graphical representation of the physical memory.
//
//
///////////////////////////////////////////////////////////////////////////////
void draw_mem ( void )
{
    glClear ( GL_COLOR_BUFFER_BIT );
    glutSetWindowTitle ( "Memory Contents" );
    int i, j, yval;
    int address_num = 0;

    string t;
    stringstream out;
    char temp;
    char *f;
    int size = memvec.size();

    for ( j = 0; j < 8; j++ )
    {
        for ( i = 0; i < 32; i++ )
        {
            yval = ScreenHeight - 25 * i;

            out << address_num << "   ";
            out << ( bitset<sizeof ( address ) * 8> ) memvec[address_num++];
            t = out.str();
            f = ( char* ) t.c_str();
            DrawTextString ( f, j * 130 + 3, yval - 20, Black );
            out.str ( "" );
            DrawRectangle ( ( j * 130 ) + 30, yval, ( j + 1 ) * 130, 
					yval - 25, Black );
        }
    }

    glFlush();

}

///////////////////////////////////////////////////////////////////////////////
//
// tlb_gui function
//
// author: Shaun Gruenig, Hannah Aker, Kelsey Bellew
//
// description: This is a helper function that initializes the GUI and then
//      calls the function to print the TLB.
//
//
///////////////////////////////////////////////////////////////////////////////
void tlb_gui()
{
    ScreenWidth = 200;
    ScreenHeight = TLB_SIZE * 30;
    glutInit ( & ( *superargc ), ( *superargv ) );
    glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, 
			GLUT_ACTION_CONTINUE_EXECUTION );
    initOpenGL();

    glutDisplayFunc ( draw_tlb );

    glutMainLoop();

}


///////////////////////////////////////////////////////////////////////////////
//
// pt_gui function
//
// author: Shaun Gruenig, Hannah Aker, Kelsey Bellew
//
// description: This is a helper function that initializes the GUI and then
//      calls the function to print the page table.
//
//
///////////////////////////////////////////////////////////////////////////////
void pt_gui()
{
    ScreenWidth = 240;
    ScreenHeight = ptvec.size() / 2 * 30;
    glutInit ( & ( *superargc ), ( *superargv ) );
    glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, 
			GLUT_ACTION_CONTINUE_EXECUTION );
    initOpenGL();

    glutDisplayFunc ( draw_pt );

    glutMainLoop();

}

///////////////////////////////////////////////////////////////////////////////
//
// mem_gui function
//
// author: Shaun Gruenig, Hannah Aker, Kelsey Bellew
//
// description: This is a helper function that initializes the GUI and then
//      calls the function to print the physical memory.
//
//
///////////////////////////////////////////////////////////////////////////////
void mem_gui()
{
    ScreenWidth = 8 * 130;
    ScreenHeight = memvec.size() / 8 * 25;
    glutInit ( & ( *superargc ), ( *superargv ) );
    glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, 
			GLUT_ACTION_CONTINUE_EXECUTION );
    initOpenGL();

    glutDisplayFunc ( draw_mem );

    glutMainLoop();

}



///////////////////////////////////////////////////////////////////////////////
//
// memory_simulation function
//
// author: Shaun Gruenig
//
// description: This is the function that runs the main memory simulation
//      portion of the program. It instantiates the appropriate
//      objects and runs through the simulation based on randomly
//      generated numbers and some user input.
//
//
///////////////////////////////////////////////////////////////////////////////
void memory_simulation()
{
    address temp;
    int choice;
    int end_of_page_table;
    mmu mmunit;

    srand ( time ( NULL ) );

    cout << "Randomly filling page table and memory contents..." << endl;
    mmunit.random_mem_fill();

    while ( true )
    {
        cout << "What would you like to do?\n";
        cout << "1. Randomly generate logical address\n";
        cout << "2. Print TLB\n";
        cout << "3. Print page table\n";
        cout << "4. Print memory contents\n";
        cout << "5. Return to main menu\n";

        cin >> choice;

        switch ( choice )
        {
            case 1:
                end_of_page_table = pow ( 2, 
				8 * sizeof ( frame ) - PAGE_SIZE );
                temp = ( rand() % ( MEM_SIZE - end_of_page_table + 1 ) ) 
					+ end_of_page_table + 1;
                cout << "logical address: " << 
			( bitset<sizeof ( address ) * 8> ) temp << endl;
                cout << "MMU returns " << 
			( bitset<sizeof ( address ) * 8> ) 
				mmunit.retrieve ( temp ) 
			<< " for the physical address." << endl;
                break;
            case 2:
                mmunit.print_tlb();
                tlb_gui();
                tlbvec.clear();
                break;
            case 3:
                mmunit.print_page_table();
                pt_gui();
                ptvec.clear();
                break;
            case 4:
                mmunit.print_memory();
                mem_gui();
                memvec.clear();
                break;
            case 5:
                cout << "Exiting.\n\n";
                tlbvec.clear();
                ScreenWidth = 600;
                ScreenHeight = 300;
                return;
                break;
            default:
                cout << "Invalid choice; valid choices are numbers 1, 2, 3, and 4.\n";
                break;
        }

        //clears buffer so that loop doesn't go unstable
        //when invalid input is entered
        cin.clear();
        cin.ignore ( 10000, '\n' );
    }
}
