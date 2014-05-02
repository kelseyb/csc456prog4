#include "prog4.h"

//void shortest_job_first(process p[], int proc_count, int time_segment)
void shortest_job_first()
{
int i, best, next;
bool done = false;
int done_count = 0;
int total_time = 0;

glClear(GL_COLOR_BUFFER_BIT);

while(!done)
{
  best = -1;
  next = 0; //maybe set all this at the bottom?
  for(i=0; i<proc_count; i++) //this seems really stupid. really. stupid.
  {
    if(next==0)
    {
      if(p[i].state == 0 && p[i].start_time <= total_time)
      {
        best = i;
        next = 1;
      }
    }
  } //weeeell. it makes sure best is a real value?

  if(best != -1) //if valid choice exists
  {
    for(i=0; i<proc_count; i++)//find best choice
    {
      if(p[i].state == 0 && p[i].start_time <= total_time) //is not done
      {
        if(p[i].burst < p[best].burst)
          best = i;
      }
    }

    //go do highest priority job
    p[best].time_left = p[best].time_left - time_segment;
 
    total_time += time_segment;//p[best].burst;
    cout << "process running: " << best << " time left: " <<p[best].time_left << " sys time: " << total_time << endl;

    if(p[best].time_left <= 0)
    {
      p[best].state = 1; //done.
      done_count++;
      cout << "process " << best << " is done. burst: " << p[best].burst << endl; 
    }
 
    if(done_count == proc_count) done = true;
    }//end if (valid) (best!=-1
    else //if best == -1
    {
      total_time += time_segment;
      cout << "nothing ran this cycle; sys time: " << total_time << endl;
    }
    
    //update screen
    current_process = best;
    //update_screen(p, proc_count, time_interval, best);
    gui_draw_stuff(best);    

  }//end while

  glFlush();
}//end sjf



