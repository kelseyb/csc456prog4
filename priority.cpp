#include "prog4.h"

//void priority(process p[], int proc_count, int time_segment)
void priority()
{
int i, best, next;
bool done = false;
int done_count = 0;
//do we need this v?
int time_passed = 0;
int total_time = 0; 


while(!done) //or next == -1 or something.
{
best = -1;
next = 0; //maybe set all this at the bottom?
for(i=0; i<proc_count; i++) //this seems really stupid. really. stupid.
{
  if(next==0)
  {
    if(p[i].state == 0)
    {
      best = i;
      next = 1;
    }
  }
} 

if(best != -1) //valid choice exists
{
for(i=0; i<proc_count; i++)
{
if(p[i].state == 0 && p[i].start_time <= total_time) //is not done and has started
{
  if(p[i].priority < p[best].priority)
    best = i;
}
}

//go do highest priority job
p[best].time_left = p[best].time_left - time_segment;

total_time += time_segment;
cout << "process running: " << best << " priority: " << p[best].priority << " time left: " << p[best].time_left << " sys time: " << total_time << endl;

if(p[best].time_left <= 0)
{
  p[best].state = 1; //proc is done
  done_count++;
  cout << "process " << best << " is done. priority: " << p[best].priority;
}


if(done_count == proc_count) done = true;
}//end if valid
else //no valid 
{
  total_time += time_segment;
  cout << "nothing ran this cycle; sys time: " << total_time << endl;
}
    current_process = best;
    //update screen
    //update_screen(p, proc_count, time_interval, best);

}//end while
}//end priority



