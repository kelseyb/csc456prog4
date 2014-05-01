#include "prog4.h"

vector< vector<int> > *pointframetable;
vector<int> *pointreftable;
void guiprintframetable (void);

/*****************************************************************************/
void pagereplace( )
{
  int choice = 0, pages = 0, frames = 0, references = 0;
  while ( true)
    {
      printf( "  1. FIFO\n  2. Optimal\n  3. LRU\n  4. LFU\n"  
	      "  5. Second Chance\n  6. Clock\n  7. Back\nChoice?");
      scanf ( "%d", &choice );
  
      if ( choice == 7 )
	return;

      printf ( "Number of pages? " );
      scanf ( "%d", &pages );
      printf ( "Number of frames? " );
      scanf ( "%d", &frames );
      printf ( "Number of page references? " );
      scanf ( "%d", &references );

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
	  printf ( "Invalid Choice." );
	  return;
	  break;
	}
    }
  return;
}

/*****************************************************************************/
int minrefnum ( vector<struct virtpage> &frametable )
{
  int framenum, minnum, i;
  minnum = frametable[0].refnum;
  framenum = 0;
  for (i = 0; i < frametable.size(); i++ )
    {
      if ( frametable[i].refnum < minnum )
	{
	  minnum = frametable[i].refnum;
	  framenum = i;
	}	
    }

  return framenum;
}

/*****************************************************************************/	
int maxrefnum ( vector <struct virtpage> &frametable )
{
  int i, framenum, maxnum;
  maxnum = frametable[0].refnum;
  framenum = 0;
  for ( i = 0; i < frametable.size(); i++ )
    {
      if ( frametable[i].refnum < maxnum )
	{
	  maxnum = frametable[i].refnum;
	  framenum = i;
	}	
    }
  return framenum;
}

/*****************************************************************************/	
int inframetable ( int findpage, vector<struct virtpage> &frametable )
{
  int m = 0;
  if( frametable.empty() )
    return -1;
  for ( m = 0; m < frametable.size(); m++ )
    {
      if ( frametable[m].page == findpage )
	return m;
    }
  return -1;
}

/*****************************************************************************/
void printframetable ( vector <struct virtpage> &frametable )
{
  int k = 0;
  for ( k = 0; k < frametable.size(); k++)
    printf ( " %d", frametable[k].page );
  printf ( "\n" );
}

/*****************************************************************************/
void fillpagetable ( vector<struct virtpage> &pagetable, int numpages )
{
  virtpage temp;
  int p = 0;
  for ( p = 0; p < numpages ; p++ )
    {
      temp.page = p;
      temp.refnum = 0;
      temp.frame = 0;
      pagetable.push_back (temp);

    }
}

/*****************************************************************************/
void fillreftable ( vector<int> &reftable, int numref, int numpages )
{
  srand( time (NULL) );
  int n = 0, randnum = 7;
  printf( "Reference Table:");
  for ( n = 0; n < numref ; n++ )
    {
      randnum = rand()%numpages;
      reftable.push_back( randnum );
      printf ( " %d", reftable[n] );
    }
  printf ( "\n" );
  return;
}

/*****************************************************************************/
void guiprintframetable ( void )
{
  glClear( GL_COLOR_BUFFER_BIT );
  int i = 0, j = 0, k = 0, text_y, text_x;
  text_y = ScreenHeight-16; //then increment down by 20 
  text_x = 4; //then inrement over by 20
  char *pagenum;
  string pagenumstring;
  stringstream out;
  printf( "Reference Table:" );

  for ( i = 0; i < pointreftable->size(); i++ )
  {
    out << (*pointreftable)[i];
    pagenumstring = out.str();
    pagenum = (char*)pagenumstring.c_str();
    out.str("");
    printf( " %s", pagenum );
    DrawTextString ( pagenum, text_x, text_y, Black );
    text_x += 20;

  }

  printf ( "\n" );
  glFlush();
}	

/*****************************************************************************/	
void fifo( int numpages, int numframes, int numref )
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
      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();
    }

  pointframetable = &pageframetable;
  pointreftable = &reftable;

  glutInit ( &(*superargc),(*superargv) );
    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();

  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

/*****************************************************************************/
void optimal( int numpages, int numframes, int numref )
{
  vector<struct virtpage> pagetable;
  vector<struct virtpage> frametable;
  vector< vector<int> > pageframetable;
  vector<int> framevector;
  vector<int> reftable;
  virtpage supertemp;
  int pagefaults = 0, i = 0, q = 0, d = 0;

  fillpagetable( pagetable, numpages );
  fillreftable ( reftable, numref, numpages );
   
  for ( i = 0; i < numref; i++ )
    {
      pagetable[(reftable[i])].refnum+=1; 
    }

  for ( i = 0; i < numref ; i++ )
    { 
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0)
	{

	  if ( frametable.size() < numframes )
	    {
	      supertemp.page = reftable[i];
	      supertemp.frame = 0;
	      supertemp.refnum = pagetable[(reftable[i])].refnum ;
	      frametable.push_back( supertemp );
	     
	      pagefaults++;
	    }
	  else
	    {
	      int j = minrefnum(frametable);
	      frametable[j].page = reftable[i];
	      frametable[j].refnum -= 1;
	      pagetable[(frametable[j].page)].refnum -= 1;
	      pagefaults++;
	    }
	}
      else
	{
	  
	  frametable[q].refnum-=1;
	  pagetable[(frametable[q].page)].refnum-=1;
	 
	}
      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();
    }

  *pointframetable = pageframetable;
  *pointreftable = reftable;

  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();


  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

/*****************************************************************************/
void lru( int numpages, int numframes, int numref )
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

  printf ( "Frame Table: \n" );
  
  for ( i = 0; i < numref ; i++ )
    {
      q =  inframetable ( reftable[i] , frametable );
      if ( q < 0)
	{
	
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
      else
	frametable[q].refnum = i;

      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();

    }

  pointframetable = &pageframetable;
  pointreftable = &reftable;

  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();


  frametable.clear();
  pagetable.clear();
  reftable.clear();
}

/*****************************************************************************/
void lfu( int numpages, int numframes, int numref )
{

}

/*****************************************************************************/
void secondchance( int numpages, int numframes, int numref )
{

}

/*****************************************************************************/
void superclock( int numpages, int numframes, int numref )
{

}
	
