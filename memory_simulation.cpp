#include "memory_simulation.h"
#include "prog4.h"



void memory_simulation()
{
	address temp; 
	int choice;
	int end_of_page_table;
	mmu mmunit;

	srand(time(NULL));

	cout << "Randomly filling page table and memory contents..." << endl;
	mmunit.random_mem_fill();
	
	while(true)
	{
		cout << "What would you like to do?\n";
		cout << "1. Randomly generate logical address\n";
		cout << "2. Print TLB\n";
		cout << "3. Print page table\n";
		cout << "4. Print memory contents\n";
		cout << "5. Return to main menu\n";

		cin >> choice;

		switch(choice)
		{
			case 1:
				end_of_page_table = pow(2, 8*sizeof(frame) - PAGE_SIZE);
				temp = (rand() % (MEM_SIZE - end_of_page_table + 1)) + end_of_page_table + 1;
				cout << "logical address: " << (bitset<sizeof(address)*8>) temp << endl;
				cout << "MMU returns " << (bitset<sizeof(address)*8>) mmunit.retrieve(temp) << " for the physical address" << endl;
			  	break;
			case 2:
			  	mmunit.print_tlb();
			  break;
			case 3:
				mmunit.print_page_table();
			  break;
			case 4:
				mmunit.print_memory();
				break;
			case 5:
			  	cout << "Exiting.\n\n";
			  	return;
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
}
