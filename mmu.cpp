#include "memory_simulation.h"

mmu::mmu()
{
	ptbr = PAGE_TABLE_BASE_REGISTER;

	//calculate number of pages/page table entries:
	//2^(number of bits in memory unit - page size)
	num_of_pages = pow(2, 8*sizeof(address) - PAGE_SIZE);

	//initialize page table in memory
	cout << num_of_pages << endl;
	for(int i = 0 + ptbr; i < num_of_pages; i++)
	{
		memory.write(-1, i);
	}

}

//implementing this may not be necessary;
//'memory' should be changed to a pointer
//to implement this if it is neccessary
/*
mmu::mmu(mem_unit preexisting_mem)
{
	ptbr = PAGE_TABLE_BASE_REGISTER;
	memory = &preexisting_mem;

}
*/

mmu::~mmu()
{

}

address mmu::retrieve(address logical_addr)
{
	address physical_addr;
	page p;
	frame f;
	offset o;
	address mask;

	//extract page from logical address
	p = logical_addr >> PAGE_SIZE;

	//create bitmask to extract address offset
	for(int i = 0; i < PAGE_SIZE; i++)
		mask |= 1 << i;

	//extract offset
	o = logical_addr & mask;


//test
	cout << "logic address: " << (bitset<sizeof(data)*8>) logical_addr << "   page: " << (bitset<sizeof(data)*8>)p << "    offset: " << (bitset<sizeof(data)*PAGE_SIZE>)o << endl;
//end test


	//first check TLB for logical address and return
	//corresponding physical address if found
	f = tlbuffer.retrieve(p);

//test
	cout << "frame: " << (bitset<sizeof(data)*32>)f << "  error: " << (bitset<sizeof(data)*32>)ERROR <<  endl;
//end test

	if(f != (frame) ERROR)
	{
		cout << "here " << endl;
		//combine frame and offset to get address
		physical_addr = (f << PAGE_SIZE) | o;
		return physical_addr;
	}

	//otherwise, check the page table
	f = memory.read(ptbr + p);

	//combine frame and offset to get address
	physical_addr = (f << PAGE_SIZE) | o;


//test
	cout << "phys. address: " << (bitset<sizeof(data)*8>) physical_addr << "   frame: " << (bitset<sizeof(data)*8>)f << "   offset: " << (bitset<sizeof(data)*PAGE_SIZE>)o << endl;
//end test


	
	//return phys addr or error if phys addr doesn't exist
	if(f != (frame) ERROR)
	{
		//add page table entry to TLB for quicker lookup next time
		tlbuffer.insert(make_pair(p, f));
		return physical_addr;
	}
	else
		return ERROR;

	

}
