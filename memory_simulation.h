#ifndef MEMORY_SIMULATION_H
#define MEMORY_SIMULATION_H

//includes/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
#include <queue>
#include <cmath>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <bitset>


using namespace std;


//defines//////////////////////////////////////////////////////////////////////
//MEM_SIZE must equal pow(2, 8*sizeof(address)), otherwise there will
//be seg faults or unused memory addresses; address is a
//typedef listed below
#define MEM_SIZE 256 //number of memory addresses in mem_unit
#define MAX_PROCESS_SIZE 8 //in bytes
#define TLB_SIZE 8 //number of key-value pairs in TLB
#define ERROR -1 //can change error in case 'address' typedef is changed
#define PAGE_TABLE_BASE_REGISTER 0
#define PAGE_SIZE 3 //in bits



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

//note:
//the above typdefs should stay unsigned, as the TLB uses a logical
//bit shift right; in other words, if it isn't unsigned, it will shift
//in ones instead of zeros when the right bit shift operator (>>) is used,
//which will cause issues


//classes//////////////////////////////////////////////////////////////////////

extern vector < pair< page, frame > > tlbvec;
extern vector < address > ptvec;
extern vector < address > memvec;

///////////////////////////////////////////////////////////////////////////////
//
// tlb class
//
// author: Shaun Gruenig
//
// description: a translation look-aside buffer class. The class allows
//      new (page, frame) pairs to be inserted, and uses a FIFO
//      replacement scheme. The class also retrieves frames for a
//      given page.
//
///////////////////////////////////////////////////////////////////////////////
class tlb
{
    public:
        tlb();
        ~tlb() {};
        bool insert ( pair<page, frame> new_entry );
        frame retrieve ( page p );
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

///////////////////////////////////////////////////////////////////////////////
//
// mem_unit class
//
// author: Shaun Gruenig
//
// description: a simple block of memory (of size MEM_SIZE) that can be
//      written to and read from. It can also print its contents.
//
///////////////////////////////////////////////////////////////////////////////
class mem_unit
{

    public:
        mem_unit();
        ~mem_unit() {};

        bool write ( data d, address addr );
        data read ( address addr );
        void print();

    private:
        //simple array used to hold memory contents
        data mem[MEM_SIZE];

};

///////////////////////////////////////////////////////////////////////////////
//
// mmu class
//
// author: Shaun Gruenig
//
// description: a memory management unit class. This class translates
//      logical addresses into physical addresses contained in the
//      memory unit instantiated within this class. It also has
//      a translation look-aside buffer which it keep the TLB_SIZE
//      most recent pages/frames for quick access. It contains many
//      print functions for the purpose of implementing the
//      simulation.
//
///////////////////////////////////////////////////////////////////////////////
class mmu
{
    public:
        mmu();
        ~mmu() {};

        address retrieve ( address logical_addr );
        void print_memory()
        {
            memory.print();
        };
        void print_tlb()
        {
            tlbuffer.print();
        };
        void random_mem_fill();
        void print_page_table();



    private:
        tlb tlbuffer; //translation look-aside buffer
        address ptbr; //page-tabe base register
        int num_of_pages;
        //note: needs to be changed to a pointer if overloaded
        //constructor is required
        mem_unit memory;


};


#endif
