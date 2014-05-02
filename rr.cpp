#include "prog4.h"

//file specific globals
//vector< vector<int> > *pointframetable;
//vector<int> *pointreftable;

struct process
{
  int id, burst, time_left, start_time;
  int state;
  int priority;
};

/*
struct process_list
{
  int current;
  bool done;
}
*/
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

//process scheduling globals
//int current_x;
//int y_vals[] = {ScreenHeight-20, ScreenHeight-40, ScreenHeight-60, ScreenHeight-80, ScreenHeight-100}; //or something.
//globals that shouldn't be globals
//process p[5];
int proc_count;
//int current_process;
//int time_interval = 100;
//int time_segment = 100;

//vector<vector<int>> points_to_draw;
//vector<process_list>
vector<int> points_to_draw;


void gui_draw_stuff ( void )
{
  glClear( GL_COLOR_BUFFER_BIT );

  //int y_vals[] = {ScreenHeight-20, ScreenHeight-40, ScreenHeight-60, ScreenHeight-80, ScreenHeight-100};

  int i, j, yval;
  int buffer = 100;
  //int proc_count = 5;
  int div = 4;
  int time_interval = 100;

  string t;
  stringstream out;
  char *f;

  for(i=0; i<proc_count; i++)
  {
	//make the page into a c string for gui printing
      yval = i*20+20;
cout << yval;
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
      yval = j*20+20;
      if(points_to_draw[i] == j)
      {
        
        DrawRectangle((i*100)/div+buffer,yval, (i*100+time_interval)/div+buffer, yval, Green);
      }
      else
      {
        DrawLine((i*100)/div+buffer, yval, (i*100+time_interval)/div+buffer, yval, Blue);
      }
    }
  }

 glFlush();
}



//draw stuff function
void update_screen()
{
  glutInit ( &(*superargc),(*superargv) );
    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  initOpenGL();
  glutDisplayFunc( gui_draw_stuff );
  glutMainLoop();

  points_to_draw.clear();
}//end updateScreen



void round_robin(process p[], int proc_count, int time_segment)
//void round_robin()
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
glClear(GL_COLOR_BUFFER_BIT);
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
      //gui_draw_stuff(i, total_time);
      points_to_draw.push_back(i);

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
    //gui_draw_stuff(-1, total_time);
    points_to_draw.push_back(-1);
  }


}//end not done loop
  cout << endl;
update_screen();
}//end round robin

void shortest_job_first(process p[], int proc_count, int time_segment)
//void shortest_job_first()
{
  int i, best, next;
  bool done = false;
  int done_count = 0;
  int total_time = 0;

  //randomize processes
  /*for(int i=0; i<proc_count; i++)
  {
    p[i] = rand_proc();
    p[i].id = i;
    cout << i << " with burst " << p[i].burst << ", priority " << p[i].priority;
    cout << ", and start time " << p[i].start_time << endl;
  }*/

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
    //current_process = best;
    //update_screen(p, proc_count, time_interval, best);
    //gui_draw_stuff(best, total_time);    
  
    points_to_draw.push_back(best);

  }//end while

  update_screen();
}//end sjf

void priority(process p[], int proc_count, int time_segment)
//void priority()
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
    //current_process = best;
    //update screen
    //update_screen(p, proc_count, time_interval, best);
    points_to_draw.push_back(best);

}//end while

update_screen();
}//end priority


/*
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
}//end draw_everything*/



int process_scheduling()
{
  int proc_count;
  int time_segment = 100; //or what ever you want to call that.
  int total_time = 0;
  int choice;

  srand(time(NULL));

  //current_x = 0; //or whatever initial value is.

  //get data from user (about processes)
  //or fake that data.

  cout << "How many processes would you like to schedule?";
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

  //p[2].start_time = 0;


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

    /*if(choice == 1 || choice == 2 || choice == 3)
    {
      draw_everything(choice);
      return 0;
    }
    else if(choice == 4)
    {
      return 0;
    }
    else
    {
      cout << "Invalid choice; Valid choices are 1, 2, 3, and 4.\n";
    }*/
  }//end whiletrue
}

