#include "memory_simulation.h"

///////////////////////////////////////////////////////////////////////////////
//
// tlb::tlb function
//
// author: Shaun Gruenig
//
// description: This constructor simply initializes the current and max size
//		to their default values.
//
///////////////////////////////////////////////////////////////////////////////
tlb::tlb()
{
	curr_size = 0;
	max_size = TLB_SIZE;

}



///////////////////////////////////////////////////////////////////////////////
//
// tlb::insert function
//
// author: Shaun Gruenig
//
// description: This function inserts a new (page, frame) pair into the TLB.
//		it uses a FIFO replacement scheme to replace the oldest 
//		entries. 
//
// parameters:  new_entry - the new (page, frame) pair to insert into the TLB
//
///////////////////////////////////////////////////////////////////////////////
bool tlb::insert(pair<page, frame> new_entry)
{

	if(curr_size < max_size)
	{
		//add new entry to table and increment size
		tlb_table[new_entry.first] = new_entry.second;
		tlb_fifo.push(new_entry.first);
		curr_size++;
	}
	else
	{
		//remove entry at front of the queue to make 
		//room for new TLB entry
		address temp = tlb_fifo.front();
		tlb_fifo.pop();

		//remove old entry from map;
		tlb_table.erase(temp);

		//add new entry to table
		tlb_table[new_entry.first] = new_entry.second;
		tlb_fifo.push(new_entry.first);	
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// tlb::retrieve function
//
// author: Shaun Gruenig
//
// description: This function checks if an entry exist in the table and 
//		returns it if it does. Since it uses a hash map, the 
//		values are found in constant time.
//
// parameters:  p - page to look up in table
//
///////////////////////////////////////////////////////////////////////////////
frame tlb::retrieve(page p)
{

	//if a physical address corresponding to
	//the specified logical address, return it
	if(tlb_table.count(p))
	{
		return tlb_table[p];
	}

	
	//otherwise, return an error
	else
		return ERROR;
		

}

///////////////////////////////////////////////////////////////////////////////
//
// tlb::print function
//
// author: Shaun Gruenig
//
// description: This function prints the elements of the TLB.
//
//
///////////////////////////////////////////////////////////////////////////////
void tlb::print()
{
	page temp;

	cout << "--------------TLB----------------------------" << endl;

	for(int i = 0; i < curr_size; i++)
	{
		temp = tlb_fifo.front();
		cout << "page: " << (bitset<sizeof(frame)*8 - PAGE_SIZE>)temp;
		cout << "\t\t frame: " << (bitset<sizeof(frame)*8 - PAGE_SIZE>)tlb_table[temp] << endl;
		tlb_fifo.pop();
		tlb_fifo.push(temp);

	}
	
	cout << "---------------end of TLB---------------------" << endl;
}


