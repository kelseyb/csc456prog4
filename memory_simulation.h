#ifndef MEMORY_SIMULATION_H
#define MEMORY_SIMULATION_H

//includes/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
#include <queue>
#include <cmath>

#include <ctime>
#include <stdlib.h>
#include <bitset>


using namespace std;


//defines//////////////////////////////////////////////////////////////////////
//MEM_SIZE = pow(2, 8*sizeof(address)), otherwise there will
//be seg faults or unused memory addresses; address is a 
//typedef listed below
#define MEM_SIZE 256 //number of memory addresses in mem_unit


#define TLB_SIZE 8 //number of key-value pairs in TLB
#define ERROR -1 //can change error in case 'address' typedef is changed
#define PAGE_TABLE_BASE_REGISTER 0

#define PAGE_SIZE 2 //in bits



//typedefs/////////////////////////////////////////////////////////////////////
//just in case something else should need to be used as 
//addresses; be sure to change MEM_SIZE if you change
//the data type of 'address'
typedef unsigned char address;
typedef unsigned char page;
typedef unsigned char frame;
typedef unsigned char offset;

//type of data the block of memory in mem_unit holds
typedef unsigned char data;

//the above typdefs should stay unsigned, as the TLB uses a logical 
//bit shift right; in other words, if it isn't unsigned, it will shift
//in ones instead of zeros when the right bit shift operator (>>) is used,
//which will cause issues


//classes//////////////////////////////////////////////////////////////////////
class tlb
{
	public:
		tlb();
		~tlb();
		bool insert(pair<page, frame> new_entry);
		frame retrieve(page p);
		void print();


	private:
		int curr_size;
		int max_size;		

		//this map holds the tlb; a map is used for constant time 
		//element access
		map <page, frame> tlb_table; 

		//this queue is used to determine which elements should be 
		//removed from the TLB when it gets full (I used a FIFO TLB
		//since no specific TLB replacement algorithm was asked for) 
		queue<page> tlb_fifo; 
		

};

class mem_unit
{

	public:
		mem_unit();
		~mem_unit();

		bool write(data d, address addr);
		data read(address addr);
		void print();

	private:
		data mem[MEM_SIZE];
		
};

class mmu
{
	public:
		mmu();
		mmu(mem_unit memory);
		~mmu();

		address retrieve(address logical_addr);
		void print_memory(){memory.print();};
		void print_tlb(){tlbuffer.print();};

		

	private:
		tlb tlbuffer; //translation look-aside buffer
		address ptbr; //page-tabe base register
		int num_of_pages; 
		//note: needs to be changed to a pointer if overloaded 
		//constructor is required
		mem_unit memory; 
		

};



#endif 
