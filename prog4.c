#include "prog4.h"

int main()
{
int choice, success;

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

  break;
case 3:

  break;
case 4:
  cout << "Exiting.\n\n";
  return 0;
  break;
default:
  cout << "Invalid choice; valid choices are numbers 1, 2, 3, and 4.\n";
  break;
}

}

return 0;
}
