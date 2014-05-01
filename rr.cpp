//to be round robin code
//aha, nix that
//to be rr, shortest job first, and priority
//to display- 
//-gantt chart of some form?
//-order went, when done, something.
//-average waiting time.
//include start times

//#include <iostream>

//using namespace std;

#include "prog4.h"

//file specific globals
int current_x;
int y_vals[] = {100, 80, 60, 40, 20}; //or something.


struct process
{
  int id, burst, time_left, start_time;//, start; //do we need start?
  int state; //0 not started, 1 finished? or make a 'in process'?
  int priority;
};

//also probably need a generate random process function.
process rand_proc()
{
  //id gets set after this call.
  process p;

  p.state = 0;
  p.priority = rand()%10+1;
  p.burst = (rand()%10+1)*100;
  p.time_left = p.burst;
  p.start_time = (rand()%10+1)*100;

  return p;
}

//draw stuff function
void update_screen(process p[], int proc_count, int time_interval, int current_process)
{
int i;

for(i=0; i<proc_count; i++)
{
  if(p[i].state == 0) //not done yet, and also started.
  {
    if(i==current_process)
    {
      DrawRectangle(current_x, y_vals[i], time_interval, y_vals[i], Green);
    }
    else
    {
      DrawLine(current_x, y_vals[i], time_interval, y_vals[i], Blue);
    }
  }
}

}//end updateScreen


void priority(process p[], int proc_count, int time_segment)
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

    //update screen
    //update_screen(p[], proc_count, time_interval, best);

}//end while
}//end priority




void shortest_job_first(process p[], int proc_count, int time_segment)
{
int i, best, next;
bool done = false;
int done_count = 0;
int total_time = 0;

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
    //update_screen(p[], proc_count, time_interval, best);
    
  }//end while
}//end sjf




//no need for reference
void round_robin(process p[], int proc_count, int time_segment)
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
      //update_screen(p[], proc_count, time_interval, i);

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
    //update_screen(p[], proc_count, time_interval, -1);
  }

}//end not done loop
  cout << endl;
}//end round robin





int process_scheduling()
{
  int proc_count;
  int time_segment = 100; //or what ever you want to call that.
  int total_time = 0;

  srand(time(NULL));

  current_x = 0; //or whatever initial value is.

  //get data from user (about processes)
  //or fake that data.
  proc_count = 5;
  process p[5]; //can we do this? or does it have to be dynamic blah?

  //randomize processes
  for(int i=0; i<proc_count; i++)
  {
    p[i] = rand_proc();
    p[i].id = i;
    cout << i << " with burst " << p[i].burst << ", priority " << p[i].priority;
    cout << ", and start time " << p[i].start_time << endl;
  }


/*  for(int i=0; i<proc_count; i++)
  {
    p[i].id = i;
    p[i].state = 0; //v 200, 300, 400, b/c ts=100
    p[i].burst = i*100+200; //easier to look at numbers in hundreds.
    p[i].time_left = p[i].burst;
    total_time += p[i].burst;
    p[i].priority = proc_count - i;
    if(i%2==0) p[i].start_time = 0;
    else p[i].start_time = i*100;
  }
  //faker data
  p[1].priority = -1;
  p[3].priority = 9;
//  p[0].burst = 700;
  //p[2].burst = 100;
  p[1].start_time = 400;

  //end fake data
*/

  //output data stats
  // for(int i=0; i<proc_count; i++)
  // {
  //   cout << "process: " << i << "   burst time: " << p[i].burst;
  //   cout << "   start time: " << p[i].start_time << endl;
  // }
  // cout << "time segment: " << time_segment << endl;

int choice;
  while(true)
  {
    cout << "Which Process Scheduler would you like to run?\n";
    cout << "1. Round Robin\n";
    cout << "2. Priority\n";
    cout << "3. Shortest Job First\n";
    cout << "4. I changed my mind- go back.\n";
    cin >> choice;
switch(choice)
{
case 1:
  round_robin(p, proc_count, time_segment);
  return 0;
  break;
case 2:
  priority(p, proc_count, time_segment);
  return 0;
  break;
case 3:
  shortest_job_first(p, proc_count, time_segment); 
  return 0;
  break;
case 4:
  return 0;
  break;
default:
  cout << "Invalid choice; Valid choices are 1, 2, 3, and 4.\n";
  break;
  //round_robin(p, proc_count, time_segment);
  //priority(p, proc_count, time_segment);
  //shortest_job_first(p, proc_count, time_segment);

}//end switch

  }//end whiletrue
}

