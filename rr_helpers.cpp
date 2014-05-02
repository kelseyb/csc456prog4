#include "prog4.h"


process rand_proc()
{
  //id gets set after this call.
  process p;

  p.state = 0;
  p.priority = rand()%10+1;
  p.burst = (rand()%10+1)*100;
  p.time_left = p.burst;
  p.start_time = (rand()%5+1)*100;

  return p;
}

void gui_draw_stuff (int current_process)
{
  //glClear( GL_COLOR_BUFFER_BIT );

  int i;

  for(i=0; i<proc_count; i++)
  {
    if(p[i].state == 0) //not done yet, and also started.
    {
      if(i==current_process)
      {
        DrawRectangle(current_x, y_vals[i], time_interval/2, y_vals[i]+10, Green);
      }
      else
      {
        DrawLine(current_x, y_vals[i], time_interval/2, y_vals[i], Blue);
      }
    }
  }

  current_x += time_interval/2;

  //printf ( "\n" );
  //glFlush();
} //are we using this anymore? //yes.

//real draw stuff function
void draw_everything(int choice)
{
  glutInit ( &(*superargc),(*superargv) );
    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL();
  switch(choice)
  {
  case 1:
    glutDisplayFunc( round_robin );
    //round_robin(p, proc_count, time_segment);
    //return 0;
    break;
  case 2:
    glutDisplayFunc( priority );
    //priority(p, proc_count, time_segment);
    //return 0;
    break;
  case 3:
    glutDisplayFunc( shortest_job_first );
    //shortest_job_first(p, proc_count, time_segment); 
    //return 0;
    break;
  }

  glutMainLoop();
}//end draw_everything


