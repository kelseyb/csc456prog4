#include "prog4.h"

//void round_robin(process p[], int proc_count, int time_segment)
void round_robin()
{
int i = 0;
int j, k;
bool done = false;
int done_count = 0;
int total_time=0;
//do while? i forget where the check happens.
cout << "made it to rr f(x)\n";
while(!done)
{
  //find valid start
  bool valid = false;
  for(j=0; j<proc_count; j++)
  {
    if(p[j].state == 0 && p[j].start_time <= total_time)
      valid = true;
  }
  if(valid == true)
  {
    //this is messy- could combine.
    if(p[i].state == 0 && p[i].start_time <= total_time)
    { 
      cout << "starting process " << i << "...\n";
      p[i].time_left = p[i].time_left - time_segment;
      total_time += time_segment;

      //update screen
      //update_screen(p, proc_count, time_interval, i);

      if(p[i].time_left <= 0)
      {
        p[i].state = 1; 
        cout << i << " is done!\n";
        done_count++;
      }
    }
    
    //loop through processes. (basically, find ones that are valid, in order.)
    if(i==(proc_count-1)) i = 0;
    else i = i+1;

    //check if done
    if(done_count == (proc_count)) done = true;
  }//end if valid
  else //no valid
  {
    total_time += time_segment;
    cout << "no processes ran- sys time: " << total_time << endl;
    
    //update screen
    //update_screen(p, proc_count, time_interval, -1);
  }

}//end not done loop
  cout << endl;
}//end round robin


