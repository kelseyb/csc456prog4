#include "memory_simulation.h"



///////////////////////////////////////////////////////////////////////////////
//
// mem_unit::mem_unit function
//
// author: Shaun Gruenig
//
// description: This constructor simply initializes all elements in the
//      block of memory to 0.
//
///////////////////////////////////////////////////////////////////////////////
mem_unit::mem_unit()
{
    //initialize block of memory to all zeros
    for ( int i = 0; i < MEM_SIZE; i++ )
        mem[i] = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// mem_unit::write function
//
// author: Shaun Gruenig
//
// description: This constructor simply initializes all elements in the
//      block of memory to 0.
//
// parameters:  d - data to be written
//      addr - address to write to
//
///////////////////////////////////////////////////////////////////////////////
bool mem_unit::write ( data d, address addr )
{
    mem[addr] = d;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// mem_unit::write function
//
// author: Shaun Gruenig
//
// description: This constructor simply initializes all elements in the
//      block of memory to 0.
//
// parameters:  addr - address to write to
//
///////////////////////////////////////////////////////////////////////////////
data mem_unit::read ( address addr )
{
    return mem[addr];
}

///////////////////////////////////////////////////////////////////////////////
//
// mem_unit::write function
//
// author: Shaun Gruenig
//
// description: This constructor simply initializes all elements in the
//      block of memory to 0.
//
///////////////////////////////////////////////////////////////////////////////
void mem_unit::print()
{
    for ( int i = 0; i < MEM_SIZE; i++ )
        memvec.push_back ( mem[i] );

}
