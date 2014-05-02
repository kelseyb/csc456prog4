#include "prog4.h"

//global pointers for accessing inside GUI callbacks
vector< vector<int> > *pointframetable;
vector<int> *pointreftable;
int *pointpagefaults;
//GUI callback function for display
void guiprintframetable (void);

 /************************************************************************
   Function: pagereplace
   Author: Hannah Aker
   Description: gives the user a menu, calls the appropiate function
   Parameters: none
 ************************************************************************/
void pagereplace( )
{
  int choice = 0, pages = 0, frames = 0, references = 0;
  while ( true)
    {
      printf( " Which Page Replacement Algorithm would you like to run?\n"
	      "  1. FIFO\n  2. Optimal\n  3. LRU\n  4. LFU\n"  
	      "  5. Second Chance\n  6. Clock\n  7. Back\nChoice? ");
      //if scanf fails, return
      	if (scanf ( "%d", &choice ) == 0 )
	{
	  printf( "Pick an INTEGER!\n" );
	  return;
	}
	
	//go back if 7
      if ( choice == 7 )
	return;
       
      //check for valid choice
      if ( choice < 1 || choice > 7 )
	{
	  printf( "Pick a number between 1 and 7!\n" );
	  continue;
	}
	
	//get data
      printf ( "Number of pages? " );
      scanf ( "%d", &pages );
      printf ( "Number of frames? " );
      scanf ( "%d", &frames );
      printf ( "Number of page references? " );
      scanf ( "%d", &references );

	//process choice
      switch (choice)
	{
	case 1: fifo( pages, frames, references );
	  break;
	case 2: optimal( pages, frames, references );
	  break;
	case 3: lru( pages, frames, references );
	  break; 
	case 4: lfu( pages, frames, references );
	  break;
	case 5: secondchance( pages, frames, references );
	  break;
	case 6: superclock( pages, frames, references );
	  break;
	default: 
	  printf ( "You shouldn't have gotten here...\n" );
	  break;
	}
    }
  return;
}

 /************************************************************************
   Function: minrefnum
   Author: Hannah Aker
   Description: fins the smallest refnum and refnum2 in the frametable
   Parameters: frametable-vector of structs, returns location in vector
 ************************************************************************/
int minrefnum ( vector<struct virtpage> &frametable )
{
  int framenum, minnum, i, minnum2;	//variables
  //initilize min numbers
  minnum = frametable[0].refnum;
  minnum2 = frametable[0].refnum2;
  framenum = 0;
  //loop thru the array
  for (i = 0; i < frametable.size(); i++ )
    {
    	//if the current one is less than the current mins,
      if ( (frametable[i].refnum < minnum)&&(frametable[i].refnum2 <= minnum2) )
	{
		//update variables
	  minnum = frametable[i].refnum;
	  minnum2 = frametable[i].refnum2;
	  framenum = i;
	}	
    }
//return the location
  return framenum;
}

 /************************************************************************
   Function: minrefnum2
   Author: Hannah Aker
   Description: finds min in frametable based on only refnum
   Parameters: frametable, returns location in vector
 ************************************************************************/
int minrefnum2 ( vector <struct virtpage> &frametable )
{
  int i, framenum, minnum;	//variables
  //initilize stuff
  minnum = frametable[0].refnum;
  framenum = 0;
  //loop vector, update min refnum if needed
  for ( i = 0; i < frametable.size(); i++ )
    {
      if ( frametable[i].refnum < minnum )
	{
	  minnum = frametable[i].refnum;
	  framenum = i;
	}	
    }
  return framenum;
}

 /************************************************************************
   Function:inframetable
   Author: Hannah Aker
   Description: checks if a page is in the frametable
   Parameters: page to be found, frametable, returns location if found, -1 if not found
 ************************************************************************/
int inframetable ( int findpage, vector<struct virtpage> &frametable )
{
  int m = 0;
  //return if empty
  if( frametable.empty() )
    return -1;
    //loop vector, check pages
  for ( m = 0; m < frametable.size(); m++ )
    {
    	//return if not found
      if ( frametable[m].page == findpage )
	return m;
    }
    //return if not found
  return -1;
}

 /************************************************************************
   Function: fillpagetable
   Author: Hannah Aker
   Description: fills hte pagetable with virtpage structs
   Parameters: pagetable vector, number of pages, returns filled pagetable
 ************************************************************************/
void fillpagetable ( vector<struct virtpage> &pagetable, int numpages )
{	
  virtpage temp;	//variables
  int p = 0;
  //loop until max page is reached
  for ( p = 0; p < numpages ; p++ )
    {
    	//add page to vector, leave rest as defaults
      temp.page = p;
      pagetable.push_back (temp);

    }
}

 /************************************************************************
   Function: fillreftable
   Author: Hannah Aker
   Description: fills a vector with a random sequence of pages to be referenced
   Parameters: reftable, number of references, number of pages, returns filled reftable
 ************************************************************************/
void fillreftable ( vector<int> &reftable, int numref, int numpages )
{
	//seed rand
  srand( time(NULL) );
  int n = 0, randnum = 7;	//varialbes
  //loop until number of references is reached
  for ( n = 0; n < numref ; n++ )
    {
    	//get a radon page, put it in the vector
      randnum = rand()%numpages;
      reftable.push_back( randnum );
    }
  return;
}


 /************************************************************************
   Function: guiprintframetable
   Author: Hannah Aker
   Description: gui callback to draw ref table, frametable, and print number of pagefaults
   Parameters: none, references global pointers to frametable, reftable, and pagefaults
 ************************************************************************/
void guiprintframetable ( void )
{
  glClear( GL_COLOR_BUFFER_BIT );	//clear the screen
  //variables
  int i = 0, j = 0, k = 0, ay, ax, bx, by, topy;
  int w = 0;
  string t, q;
  char *f;
  stringstream out;

	//set initial coordinates for reftable and frametable
  topy = ScreenHeight;
  bx = 20;
  by = ScreenHeight-60;
  ay = ScreenHeight-36; //then increment down by 20 
  ax = 23; //then inrement over by 20

//draw table headers
  DrawTextString( (char*)"Reference Table:", 4, topy-16, Black );
  DrawTextString( (char*)"Frame Table:", 4, topy-56, Black );
  
  //loop through the reftable
  for ( i = 0; i < pointreftable->size(); i++ )
    {
    	//make the page into a c string for gui printing
      w = (*pointreftable)[i];
      out << w;
      t = out.str();
      f = (char*)t.c_str();
      out.str("");

	//put the page on the screen
      DrawTextString( f, ax, ay, Black );
      
      //update ref table location
      ax += 20;
    
    //loop through the frametable for each reference
      for ( j = 0; j < (*pointframetable)[i].size(); j++ )
	{
		//if the current location in frame table is the referenced page
	  if ( (*pointframetable)[i][j] == (*pointreftable)[i] )
	    {	//give it a green box
	      DrawFilledRectangle( bx+1, by-18, bx+19, by-1, Green); 
	    }
	    //give it a plain box
	  DrawRectangle( bx+1, by-18, bx+19, by-1, Black );
	  
	  //make the page into a c string for gui printing
	  w = (*pointframetable)[i][j];
          out << w;
	  t = out.str();
	  f = (char*)t.c_str();
	  out.str("");
	  
	  //put the page number on the screen
	  DrawTextString ( f, bx+3, by-16, Black);
	  //unpdate frame table ocation
	  by-=20;
	}
	//update frame table location
      bx += 20;
      //if the table goes past the window width, wrap to next line
      if ( bx+20 > ScreenWidth )
	{
	  topy -= ( 70 + (20 * ((*pointframetable)[i].size())));
	  DrawTextString( (char*)"Reference Table:", 4, topy-16, Black );
	  DrawTextString( (char*)"Frame Table:", 4, topy-56, Black );
	  bx = 20;
	  ay = topy-36;
	  ax = 23;
	}
	//reset y location for frame table
      by = topy-60;
    }
    //make pagefaults into string
   w = (*pointpagefaults);
          out << w;
	  t = out.str();
  q = "Page Faults: " + t;
  f = (char*)q.c_str();
  
  //calculate location for pagefaults
  topy -= ( 70 + (20 * ((*pointframetable)[i-1].size())));
  //put the text on the screen
  DrawTextString( f, 4, topy-16, Black );
//flush gui output
  glFlush();
}	

 /************************************************************************
   Function: fifo
   Author: Hannah Aker
   Description: runs the first in first out algorith for page replacement
   Parameters: number of pages, frames and references
 ************************************************************************/	
void fifo( int numpages, int numframes, int numref )
{
	//variables
  vector<struct virtpage> pagetable;
  vector<struct virtpage> frametable;
  vector< vector<int> > pageframetable;
  vector<int> framevector;
  vector<int> reftable;
  virtpage temp;
  int pagefaults = 0, i = 0, q = 0, d = 0;
  
//fill page and ref tables
  fillpagetable( pagetable, numpages );
  fillreftable ( reftable, numref, numpages );
  
 //loop thru the reference table
  for ( i = 0; i < numref ; i++ )
    {
    	//find the page in the frame tble
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0) //if not in frame table
	{
		//if the table is not full yet
	  if ( frametable.size() < numframes )
	    {
	    	//fill temp structure, add to frame table
	      temp.page = reftable[i];
	      temp.refnum = i;
	      frametable.push_back( temp );
	      //increment pagefaults
	      pagefaults++;
	    }
	  else	//otherwise, replace a page
	    {
	    	//find one to replace, replace it
	      int j = minrefnum(frametable);
	      frametable[j].page = reftable[i];
	      frametable[j].refnum = i;
	      //increment pagefaults
	      pagefaults++;
	    }
	}
	//add the pages in the frame table to the master frame table vector
      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      //clear vector for next use
      framevector.clear();
    }
//update pointer references
  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;

//do the gui stuff
  glutInit ( &(*superargc),(*superargv) );  //init
  //make it so hitting x on the gui screen continues execution
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); //more init
  glutDisplayFunc( guiprintframetable ); //display the frame table
  glutMainLoop(); //start the gui

//clear tables, just in case
  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

 /************************************************************************
   Function: optimal
   Author: Hannah Aker
   Description: runs the optimal algorithm
   Parameters: numpages, numframes, numref
 ************************************************************************/
void optimal( int numpages, int numframes, int numref )
{
	//variables
  vector<struct virtpage> pagetable;
  vector<struct virtpage> frametable;
  vector< vector<int> > pageframetable;
  vector<int> framevector;
  vector<int> reftable;
  virtpage supertemp;
  int pagefaults = 0, i = 0, q = 0, d = 0;
//fill stuff
  fillpagetable( pagetable, numpages );
  fillreftable ( reftable, numref, numpages );
   
   //loop thru ref table, calculate num of references for each page
  for ( i = 0; i < numref; i++ )
    {
      pagetable[(reftable[i])].refnum+=1; 
    }

//loop thru ref table
  for ( i = 0; i < numref ; i++ )
    { 
    	//find the page
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0)
	{
	//if frame table not ful, yada yada yada
	  if ( frametable.size() < numframes )
	    {
	      supertemp.page = reftable[i];
	      supertemp.frame = 0;
	      //use the refnum from the pagetable, update both
	      supertemp.refnum = pagetable[(reftable[i])].refnum - 1;
	      pagetable[(reftable[i])].refnum -= 1;
	      frametable.push_back( supertemp );
	     //increment pagefaults
	      pagefaults++;
	    }
	  else
	    {
	    	//replace a page
	      int j = minrefnum(frametable);
	      frametable[j].page = reftable[i];
	      //update refnum in both places
	      frametable[j].refnum -= 1;
	      pagetable[(frametable[j].page)].refnum -= 1;
	      pagefaults++;
	    }
	}
      else
	{
	  //update refnumbers
	  frametable[q].refnum-=1;
	  pagetable[(frametable[q].page)].refnum-=1;
	 
	}
	//add frametable to master frame vector
      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();
    }

//update global pointers
  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;
  //gui stuff
  glutInit ( &(*superargc),(*superargv) );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();
//clear stuff
  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

 /************************************************************************
   Function:lru
   Author: Hannah Aker
   Description: implment sthe least recently used algorithm
   Parameters: numpages, numframes, numref
 ************************************************************************/
void lru( int numpages, int numframes, int numref )
{
	//variables
  vector<struct virtpage> pagetable;
  vector<struct virtpage> frametable;
  vector< vector<int> > pageframetable;
  vector<int> framevector;
  vector<int> reftable;
  virtpage temp;
  int pagefaults = 0, i = 0, q = 0, d = 0;
//fill stuff
  fillpagetable( pagetable, numpages );
  fillreftable ( reftable, numref, numpages );
 //loop thru pagetable
  for ( i = 0; i < numref ; i++ )
    {
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0)
	{
	//same as fifo, except update when there's not a pagefault
	  if ( frametable.size() < numframes )
	    {
	      temp.page = reftable[i];
	      temp.refnum = i;
	      frametable.push_back( temp );
	      pagefaults++;
	    }
	  else
	    {
	      int j = minrefnum(frametable);
	      frametable[j].page = reftable[i];
	      frametable[j].refnum = i;
	      pagefaults++;
	    }
	}
	//update location if no pagefault
      else
	frametable[q].refnum = i;

//fill master frame table
      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();

    }
//fill the pointers to stuff
  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;
//gui stuff
  glutInit ( &(*superargc),(*superargv) );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();
//clear vectors
  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

 /************************************************************************
   Function: lfu
   Author: Hannah Aker
   Description:
   Parameters:
 ************************************************************************/
void lfu( int numpages, int numframes, int numref )
{
  vector<struct virtpage> pagetable;
  vector<struct virtpage> frametable;
  vector< vector<int> > pageframetable;
  vector<int> framevector;
  vector<int> reftable;
  virtpage temp;
  int pagefaults = 0, i = 0, q = 0, d = 0;

  fillpagetable( pagetable, numpages );
  fillreftable ( reftable, numref, numpages );
  
  for ( i = 0; i < numref ; i++ )
    {
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0)
	{
	
	  if ( frametable.size() < numframes )
	    {
	      temp.page = reftable[i];
	      //update refnum2 for anoter use
	      temp.refnum2 = pagetable[(reftable[i])].refnum2 + 1;
	      //keep fifo info
	      temp.refnum = i;
	      frametable.push_back( temp );
	      //update pagetable ref
	      pagetable[(reftable[i])].refnum2+= 1;
	      //inc pagefaults
	      pagefaults++;
	    }
	  else
	    {
	    	//find smallest combined freq and fifo page
	      int j = minrefnum(frametable);
	      //update new entry in frame table
	      frametable[j].page = reftable[i];
	      frametable[j].refnum = i;
	      //increment frametable frequency
	      frametable[j].refnum2 = pagetable[(reftable[i])].refnum2 + 1;
	      //increment pagetable frequency
	      pagetable[(reftable[i])].refnum2 += 1;
	      pagefaults++;
	    }
	}
      else
	{
		//update frequency with every access
	frametable[q].refnum += 1;
	pagetable[(frametable[q].page)].refnum +=1;
	}

	//fill master frame vector
      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();

    }
//update globals
  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;
//gui stuff
  glutInit ( &(*superargc),(*superargv) );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();
//clear stuff
  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

 /************************************************************************
   Function:second chance
   Author: Hannah Aker
   Description: implements the secondchance algorith as explained in the book (blue word def)
   Parameters: same as all the others
 ************************************************************************/
void secondchance( int numpages, int numframes, int numref )
{

  vector<struct virtpage> pagetable;
  vector<struct virtpage> frametable;
  vector< vector<int> > pageframetable;
  vector<int> framevector;
  vector<int> reftable;
  virtpage temp;
  int pagefaults = 0, i = 0, q = 0, d = 0;

  fillpagetable( pagetable, numpages );
  fillreftable ( reftable, numref, numpages );
  
  for ( i = 0; i < numref ; i++ )
    {
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0)
	{
	
	  if ( frametable.size() < numframes )
	    {
	    	//when page is added to table
	      temp.page = reftable[i];
	      //keep fifo info
	      temp.refnum2 = i;
	      ////initilis ref variable to 1
	      temp.refnum = 1;
	      frametable.push_back( temp );
	      pagefaults++;
	    }
	  else
	    {
	    	//find the smallest ref num
	      int j = minrefnum2(frametable);
	      //replace it
	      frametable[j].page = reftable[i];
	      frametable[j].refnum = 1;
	      frametable[j].refnum2 = i;
	      pagefaults++;
	    }
	}
	//loop tru frame table, add to frame vector, also
      for ( d = 0; d < frametable.size() ; d++ )
	{
	  frametable[d].refnum <<= 1; //left shift all the ref variables in table
	framevector.push_back( frametable[d].page );
	}
      pageframetable.push_back(framevector);
      framevector.clear();

    }
//update globals
  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;
//gui stuff
  glutInit ( &(*superargc),(*superargv) );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();
//clear vectors
  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

 /************************************************************************
   Function: superclock
   Author: Hannah Aker
   Description: runs the clock algorithm (according to blue def in book)
   Parameters: same as the rest
 ************************************************************************/
void superclock( int numpages, int numframes, int numref )
{

  vector<struct virtpage> pagetable;
  vector<struct virtpage> frametable;
  vector< vector<int> > pageframetable;
  vector<int> framevector;
  vector<int> reftable;
  virtpage temp;
  int pagefaults = 0, i = 0, q = 0, d = 0;

  fillpagetable( pagetable, numpages );
  fillreftable ( reftable, numref, numpages );
  
  for ( i = 0; i < numref ; i++ )
    {
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0)
	{
	
	  if ( frametable.size() < numframes )
	    {
	      temp.page = reftable[i];
	      //initilize refs, fifo in refnum1, and clock in 2
	      temp.refnum = i;
	      temp.refnum2 = 1;//flip the ref bit so min func works
	      frametable.push_back( temp );
	      pagefaults++;
	    }
	  else
	    {
	    	//find first victim
	      int j = minrefnum(frametable);
	      //if victim has secondchance available, let them use it
	      while ( frametable[j].refnum2!= 1)
		{
			//give victim second chance
		  frametable[j].refnum = i;
		  frametable[j].refnum2 = 1;
		  //find new victim
		  j = minrefnum(frametable);
		}
		//replace victim
	      frametable[j].page = reftable[i];
	      frametable[j].refnum2 = 0;
	      frametable[j].refnum = i;
	      pagefaults++;
	    }
	}
//fill frame vector
      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();

    }
//update globals
  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;
//gui stuff
  glutInit ( &(*superargc),(*superargv) );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();
//clear stuff
  frametable.clear();
  pagetable.clear();
  reftable.clear();
}
	
