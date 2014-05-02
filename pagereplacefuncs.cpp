#include "prog4.h"

vector< vector<int> > *pointframetable;
vector<int> *pointreftable;
int *pointpagefaults;
void guiprintframetable (void);

/*****************************************************************************/
void pagereplace( )
{
  int choice = 0, pages = 0, frames = 0, references = 0;
  while ( true)
    {
      printf( " Which Page Replacement Algorithm would you like to run?\n"
	      "  1. FIFO\n  2. Optimal\n  3. LRU\n  4. LFU\n"  
	      "  5. Second Chance\n  6. Clock\n  7. Back\nChoice? ");
      if (scanf ( "%d", &choice ) == 0 )
	{
	  printf( "Pick an INTEGER!\n" );
	  return;
	}

      if ( choice == 7 )
	return;
       
      if ( choice < 1 || choice > 7 )
	{
	  printf( "Pick a number between 1 and 7!\n" );
	  continue;
	}

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
	  printf ( "You shouldn't have gotten here...\n" );
	  break;
	}
    }
  return;
}

/*****************************************************************************/
int minrefnum ( vector<struct virtpage> &frametable )
{
  int framenum, minnum, i, minnum2;
  minnum = frametable[0].refnum;
  minnum2 = frametable[0].refnum2;
  framenum = 0;
  for (i = 0; i < frametable.size(); i++ )
    {
      if ( (frametable[i].refnum < minnum)&&(frametable[i].refnum2 <= minnum2) )
	{
	  minnum = frametable[i].refnum;
	  minnum2 = frametable[i].refnum2;
	  framenum = i;
	}	
    }

  return framenum;
}

/*****************************************************************************/	
int minrefnum2 ( vector <struct virtpage> &frametable )
{
  int i, framenum, minnum;
  minnum = frametable[0].refnum;
  framenum = 0;
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
void fillpagetable ( vector<struct virtpage> &pagetable, int numpages )
{
  virtpage temp;
  int p = 0;
  for ( p = 0; p < numpages ; p++ )
    {
      temp.page = p;
      pagetable.push_back (temp);

    }
}

/*****************************************************************************/
void fillreftable ( vector<int> &reftable, int numref, int numpages )
{
  srand( time(NULL) );
  int n = 0, randnum = 7;
  for ( n = 0; n < numref ; n++ )
    {
      randnum = rand()%numpages;
      reftable.push_back( randnum );
    }
  return;
}


/*****************************************************************************/
void guiprintframetable ( void )
{
  glClear( GL_COLOR_BUFFER_BIT );
  int i = 0, j = 0, k = 0, ay, ax, bx, by, topy;
  int w = 0;
  string t, q;
  char *f;
  stringstream out;

  topy = ScreenHeight;
  bx = 20;
  by = ScreenHeight-60;
  ay = ScreenHeight-36; //then increment down by 20 
  ax = 23; //then inrement over by 20


  DrawTextString( (char*)"Reference Table:", 4, topy-16, Black );
  DrawTextString( (char*)"Frame Table:", 4, topy-56, Black );
  
  for ( i = 0; i < pointreftable->size(); i++ )
    {
      w = (*pointreftable)[i];
   
      out << w;
      t = out.str();
      f = (char*)t.c_str();
      out.str("");

      DrawTextString( f, ax, ay, Black );
      ax += 20;
    
      for ( j = 0; j < (*pointframetable)[i].size(); j++ )
	{
	  if ( (*pointframetable)[i][j] == (*pointreftable)[i] )
	    {
	      DrawFilledRectangle( bx+1, by-18, bx+19, by-1, Green); 
	    }
	  DrawRectangle( bx+1, by-18, bx+19, by-1, Black );
	  
	  w = (*pointframetable)[i][j];
          out << w;
	  t = out.str();
	  f = (char*)t.c_str();
	  out.str("");
	  
	  DrawTextString ( f, bx+3, by-16, Black);
	  by-=20;
	}

      bx += 20;
      if ( bx+20 > ScreenWidth )
	{
	  topy -= ( 70 + (20 * ((*pointframetable)[i].size())));
	  DrawTextString( (char*)"Reference Table:", 4, topy-16, Black );
	  DrawTextString( (char*)"Frame Table:", 4, topy-56, Black );
	  bx = 20;
	  ay = topy-36;
	  ax = 23;
	}
      by = topy-60;
    }
   w = (*pointpagefaults);
          out << w;
	  t = out.str();
  q = "Page Faults: " + t;
  f = (char*)q.c_str();
  topy -= ( 70 + (20 * ((*pointframetable)[i-1].size())));
  DrawTextString( f, 4, topy-16, Black );

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
  pointpagefaults = &pagefaults;

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

  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;
  
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
  pointpagefaults = &pagefaults;

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
	      temp.refnum2 = pagetable[(reftable[i])].refnum2 + 1;
	      temp.refnum = i;
	      frametable.push_back( temp );
	      pagetable[(reftable[i])].refnum2+= 1;
	      pagefaults++;
	    }
	  else
	    {
	      int j = minrefnum(frametable);
	      frametable[j].page = reftable[i];
	      frametable[j].refnum = i;
	      frametable[j].refnum2 = pagetable[(reftable[i])].refnum2 + 1;
	      pagetable[(reftable[i])].refnum2 += 1;
	      pagefaults++;
	    }
	}
      else
	{
	frametable[q].refnum += 1;
	pagetable[(frametable[q].page)].refnum +=1;
	}


      for ( d = 0; d < frametable.size() ; d++ )
	framevector.push_back( frametable[d].page );
      pageframetable.push_back(framevector);
      framevector.clear();

    }

  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;

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
	      temp.page = reftable[i];
	      temp.refnum2 = i;
	      temp.refnum = 1;
	      frametable.push_back( temp );
	      pagefaults++;
	    }
	  else
	    {
	      int j = minrefnum2(frametable);
	      frametable[j].page = reftable[i];
	      frametable[j].refnum = 1;
	      frametable[j].refnum2 = i;
	      pagefaults++;
	    }
	}

      for ( d = 0; d < frametable.size() ; d++ )
	{
	  frametable[d].refnum <<= 1;
	framevector.push_back( frametable[d].page );
	}
      pageframetable.push_back(framevector);
      framevector.clear();

    }

  pointframetable = &pageframetable;
  pointreftable = &reftable;
  pointpagefaults = &pagefaults;

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
	      temp.refnum = i;
	      temp.refnum2 = 1;//flip the ref bit so min func works
	      frametable.push_back( temp );
	      pagefaults++;
	    }
	  else
	    {
	      int j = minrefnum(frametable);
	      while ( frametable[j].refnum2!= 1)
		{
		  frametable[j].refnum = i;
		  frametable[j].refnum2 = 1;
		  j = minrefnum(frametable);
		}
	      frametable[j].page = reftable[i];
	      frametable[j].refnum2 = 0;
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
  pointpagefaults = &pagefaults;

  glutInit ( &(*superargc),(*superargv) );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL(); 
  glutDisplayFunc( guiprintframetable );
  glutMainLoop();

  frametable.clear();
  pagetable.clear();
  reftable.clear();
}
	
