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


//ehh. figure out waiting later?
struct process
{
  int id, burst, time_left, start_time;//, start; //do we need start?
  int state; //0 not started, 1 finished? or make a 'in process'?
  int priority;
};

void priority(process p[], int proc_count, int time_segment)
{
int i, best, next;
bool done = false;
int done_count = 0;
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
} //weeeell. it makes sure best is a real value?

for(i=0; i<proc_count; i++)
{
if(p[i].state == 0) //is not done
{
  if(p[i].priority < p[best].priority)
    best = i;
}
}

//go do highest priority job
p[best].time_left = 0;
p[best].state = 1; //done.
time_passed += p[i].burst;
done_count++;
cout << "process " << best << " is done. priority: " << p[best].priority << endl; 

if(done_count == proc_count) done = true;
}//end while
}//end priority

//think we might have to import time_segment and basically
//rewrite this almost entire code
//yeah
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
  }//end while
}//end sjf

//no need for reference
void round_robin(process p[], int proc_count, int time_segment) //dont remember again.
{
int i = 0;
int j, k;
bool done = false;
int done_count = 0;
int total_time=0;
//do while? i forget where the check happens.
while(!done)
{
//run if not done
int time_taken = 0;

//cout << "on process " << i << endl;
if(p[i].state == 0 && p[i].start_time <= total_time)
{ 
  cout << "starting process " << i << "...\n";
  //cout << "start time: " << p[i].start_time
}
while(p[i].state == 0 && time_taken != time_segment && p[i].start_time <= total_time)
{
  p[i].time_left = p[i].time_left - 1;
  time_taken = time_taken + 1; //this seems sound?
  if(p[i].time_left == 0)
  {
     p[i].state = 1;
     cout << "process " << i << " is done!\n";
     done_count++;
  }
}
if(time_taken!=0)
{
  cout << "process " << i << " went for " << time_taken << "," << p[i].time_left << " left to go.";
  total_time += time_taken;
  cout << " system time: " << total_time << endl;
}
//this logic seems fuzzy
if(i==(proc_count-1)) i = 0;
else i = i+1;

//check if done
if(done_count == (proc_count)) done = true;

}
cout << endl;
}


int process_scheduling()
{
  int proc_count;
  int time_segment = 100; //or what ever you want to call that.
  int total_time = 0;


  //get data from user (about processes)
  //or fake that data.
  proc_count = 5;
  process p[5]; //can we do this? or does it have to be dynamic blah?

  for(int i=0; i<proc_count; i++)
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


  //output data stats
  for(int i=0; i<proc_count; i++)
  {
    cout << "process: " << i << "   burst time: " << p[i].burst;
    cout << "   start time: " << p[i].start_time << endl;
  }
  cout << "time segment: " << time_segment << endl;

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


