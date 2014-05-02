#include "prog4.h"
#include "memory_simulation.h"

int* superargc = NULL;
char*** superargv = NULL;

int main(int argc, char **argv)
{
int choice, success;
  superargv = &argv;
  superargc = &argc;



while(true)
{
cout << "What would you like to simulate?\n";
cout << "1. Process Scheduling\n";
cout << "2. Memory Management\n";
cout << "3. Page Replacement\n";
cout << "4. Quit\n";

cin >> choice;

switch(choice)
{
case 1:
  success = process_scheduling(); 
  break;
case 2:
  memory_simulation();
  break;
case 3:
  pagereplace();
  break;
case 4:
  cout << "Exiting.\n\n";
  return 0;
  break;
default:
  cout << "Invalid choice; valid choices are numbers 1, 2, 3, and 4.\n";
  break;
}

//clears buffer so that loop doesn't go unstable 
//when invalid input is entered
cin.clear();
cin.ignore(10000, '\n');
}

return 0;
}
