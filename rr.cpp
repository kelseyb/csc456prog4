#include "prog4.h"

//file specific globals
int proc_count;
vector<int> points_to_draw;

//structure to hold the information for a process
struct process
{
  int id, burst, time_left, start_time;
  int state;
  int priority;
};

 /************************************************************************
   Function: rand_proc
   Author: Kelsey Bellew
   Description: Generates a single randomized process
   Parameters: N/A
 ************************************************************************/
process rand_proc()
{
  //id gets set after this call.
  process p;

  p.state = 0;
  p.priority = rand()%10+1;
  p.burst = (rand()%10+1)*100;
  p.time_left = p.burst;
  p.start_time = (rand()%5)*100;

  return p;
}



 /************************************************************************
   Function: gui_draw_stuff
   Author: Kelsey Bellew
   Description: gets called after the GUI screen opens; formats words 
       and draws out a gantt chart using information from a global vector.
   Parameters: N/A
 ************************************************************************/
void gui_draw_stuff ( void )
{
  glClear( GL_COLOR_BUFFER_BIT );

  int i, j, yval;
  int buffer = 100;
  //proc_count = 5;
  int div = 4;
  int time_interval = 100;

  string t;
  stringstream out;
  char *f;

  for(i=0; i<proc_count; i++)
  {
	//make the page into a c string for gui printing
      yval = ScreenHeight-(i*20+20);

      out << i;
      t = "Process " + out.str();
      f = (char*)t.c_str();
      out.str("");

	//put the page on the screen
      DrawTextString(f, 5, yval, Black);
  }

  for(i=0; i<points_to_draw.size(); i++)
  {
    for(j=0; j<proc_count; j++)
    {
      yval = ScreenHeight-(j*20+20);
      if(points_to_draw[i] == j)
      {
        
        DrawRectangle((i*100)/div+buffer,yval, (i*100+time_interval)/div+buffer, yval+10, Green);
      }
      else
      {
        DrawLine((i*100)/div+buffer, yval, (i*100+time_interval)/div+buffer, yval, Blue);
      }
    }
  }

 glFlush();
}


 /************************************************************************
   Function: update_screen
   Author: Kelsey Bellew
   Description: GGets called every time a scheduler algorithm is complete 
       and has filled the clobal vector. Sets up the environment for a 
       Glut window to open up and calles the function to draw everything. 
       It then clears the global vector.
   Parameters: N/A
 ************************************************************************/
void update_screen()
{
  glutInit ( &(*superargc),(*superargv) );
    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL();
  glutDisplayFunc( gui_draw_stuff );
  glutMainLoop();

  points_to_draw.clear();
}//end updateScreen


 /************************************************************************
   Function: round_robin
   Author: Kelsey Bellew
   Description: Round Robin process scheduling algorithm.
   Parameters: p[] - array of randomized processes
               proc_count - number of elements in the array of processes
               time_segment - turn around time.
 ************************************************************************/
void round_robin(process p[], int proc_count, int time_segment)
{
int i = 0;
int j, k;
bool done = false;
int done_count = 0;
int total_time=0;

while(!done)
{
//glClear(GL_COLOR_BUFFER_BIT);
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
      //cout << "starting process " << i << "...\n";
      p[i].time_left = p[i].time_left - time_segment;
      total_time += time_segment;

      points_to_draw.push_back(i);

      if(p[i].time_left <= 0)
      {
        p[i].state = 1; 
        //cout << i << " is done!\n";
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
    //cout << "no processes ran- sys time: " << total_time << endl;

    points_to_draw.push_back(-1);
  }


}//end not done loop
  cout << endl;
update_screen();
}//end round robin


 /************************************************************************
   Function: shortest_job_first
   Author: Kelsey Bellew
   Description: Shortest Job First process scheduling algorithm.
   Parameters: p[] - array of randomized processes
               proc_count - number of elements in the array of processes
               time_segment - turn around time.
 ************************************************************************/
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
    } 

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
      //cout << "process running: " << best << " time left: " <<p[best].time_left << " sys time: " << total_time << endl;

      if(p[best].time_left <= 0)
      {
        p[best].state = 1; //done.
        done_count++;
        //cout << "process " << best << " is done. burst: " << p[best].burst << endl; 
      }
  
      if(done_count == proc_count) done = true;
    }//end if (valid) (best!=-1
    else //if best == -1
    {
      total_time += time_segment;
      //cout << "nothing ran this cycle; sys time: " << total_time << endl;
    }
   
    points_to_draw.push_back(best);

  }//end while

  update_screen();
}//end sjf


 /************************************************************************
   Function: priority
   Author: Kelsey Bellew
   Description: Priority process scheduling algorithm.
   Parameters: p[] - array of randomized processes
               proc_count - number of elements in the array of processes
               time_segment - turn around time.
 ************************************************************************/
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
//cout << "process running: " << best << " priority: " << p[best].priority << " time left: " << p[best].time_left << " sys time: " << total_time << endl;

if(p[best].time_left <= 0)
{
  p[best].state = 1; //proc is done
  done_count++;
  //cout << "process " << best << " is done. priority: " << p[best].priority;
}


if(done_count == proc_count) done = true;
}//end if valid
else //no valid 
{
  total_time += time_segment;
  //cout << "nothing ran this cycle; sys time: " << total_time << endl;
}

    points_to_draw.push_back(best);

}//end while

update_screen();
}//end priority



 /************************************************************************
   Function: process_scheduling
   Author: Kelsey Bellew
   Description: Acting main of rr.cpp. Gets called by prog4.cpp main when 
       user chooses to simulate process scheduling. This function prints 
       out a secondary menu asking how many processes the user would like 
       to simulate, prints out the information reguarding those processes, 
       and then which process simulator they would like to run.
   Parameters: N/A
 ************************************************************************/
int process_scheduling()
{
  int time_segment = 100; 
  int total_time = 0;
  int choice;

  srand(time(NULL));

  cout << "How many processes would you like to schedule? ";
  cin >> choice;
  if(choice > 100)
  {
    cout << "Choice is too large; cap is at 100. Choice has been set to 100.\n";
    choice = 99;
  }
  else if (choice < 0)
  {
    cout << "Choice must be greater than zero; choice has been set to 1.\n";
    choice = 1;
  }

  proc_count = choice;
  process p[110]; //can we do this? or does it have to be dynamic blah?

  //randomize processes
  for(int i=0; i<proc_count; i++)
  {
    p[i] = rand_proc();
    p[i].id = i;
    cout << i << " with burst " << p[i].burst << ", priority " << p[i].priority;
    cout << ", and start time " << p[i].start_time << endl;
  }

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

    }//end switch
  }//end whiletrue
}//end process_scheduling

