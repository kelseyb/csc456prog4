#include "memory_simulation.h"

///////////////////////////////////////////////////////////////////////////////
//
// mmu::mmu function
//
// author: Shaun Gruenig
//
// description: This constructor sets the page table base register and
//      initializes all the entries to invalid.
//
///////////////////////////////////////////////////////////////////////////////
mmu::mmu()
{
    ptbr = PAGE_TABLE_BASE_REGISTER;

    //calculate number of pages/page table entries:
    //2^(number of bits in memory unit - page size)
    num_of_pages = pow ( 2, 8 * sizeof ( address ) - PAGE_SIZE );

    //initialize page table in memory
    //cout << num_of_pages << endl;
    for ( int i = 0 + ptbr; i < num_of_pages; i++ )
    {
        memory.write ( ERROR, i );
    }

}

///////////////////////////////////////////////////////////////////////////////
//
// mmu::retrieve function
//
// author: Shaun Gruenig
//
// description: This function translates logical address into a physical
//      address. It first breaks the logical address into a page #
//      and offset. It then searches the TLB for the page. If it
//      not found, it then checks the page table for the entry
//      and inserts the found entry into the TLB before returning
//      the physical address that is constructed from the offset
//      and the frame that corresponds to the given page.
//
// parameters:  logical_addr - the logical address to translate into a
//              physical address
//
///////////////////////////////////////////////////////////////////////////////
address mmu::retrieve ( address logical_addr )
{
    address physical_addr;
    page p;
    frame f;
    offset o;
    address mask;

    //extract page from logical address
    p = logical_addr >> PAGE_SIZE;

    //create bitmask to extract address offset
    for ( int i = 0; i < PAGE_SIZE; i++ )
        mask |= 1 << i;

    //extract offset
    o = logical_addr & mask;

    //first check TLB for logical address and return
    //corresponding physical address if found
    f = tlbuffer.retrieve ( p );

    //if the frame was found in the TLB, return
    //the appropriate physical address
    if ( f != ( frame ) ERROR )
    {
        cout << "TLB hit!" << endl;
        //combine frame and offset to get address
        physical_addr = ( f << PAGE_SIZE ) | o;
        return physical_addr;
    }
    else
        cout << "TLB miss!" << endl;

    //otherwise, check the page table
    f = memory.read ( ptbr + p );

    //combine frame and offset to get address
    physical_addr = ( f << PAGE_SIZE ) | o;

    //return phys addr or error if phys addr doesn't exist
    if ( f != ( frame ) ERROR )
    {
        //add page table entry to TLB for quicker lookup next time
        tlbuffer.insert ( make_pair ( p, f ) );
        return physical_addr;
    }
    else
        return ERROR;

}

///////////////////////////////////////////////////////////////////////////////
//
// mmu::random_mem_fill function
//
// author: Shaun Gruenig
//
// description: This function random fills the attached memory block
//      with random values (except the page table which is filled
//      to ensure no repeated values occur).
//
///////////////////////////////////////////////////////////////////////////////
void mmu::random_mem_fill()
{
    int i;
    int temp;

    //calculates number of frames
    int end_of_page_table = pow ( 2, 8 * sizeof ( frame ) - PAGE_SIZE );

    bool used[MEM_SIZE];

    //initialize used array
    for ( i = 0; i < MEM_SIZE; i++ )
        used[i] = false;

    //fill the page table while ensuring there are no
    //duplicate entries
    for ( i = 0; i < end_of_page_table; i++ )
    {
        do
        {
            temp = rand() % end_of_page_table;
        }
        while ( used[temp] );

        memory.write ( temp, i );
        used[temp] = true;
    }

    //fills memory with random data from right after page table to
    //end of memory
    for ( i = end_of_page_table; i < MEM_SIZE; i++ )
    {
        memory.write ( rand() % ( int ) pow ( 2, 8 * sizeof ( data ) ), i );
    }

}

///////////////////////////////////////////////////////////////////////////////
//
// mmu::print_page_table function
//
// author: Shaun Gruenig
//
// description: This function simply prints the contents of the page table.
//
///////////////////////////////////////////////////////////////////////////////
void mmu::print_page_table()
{
    int i;

    //calculates number of frames
    int end_of_page_table = pow ( 2, 8 * sizeof ( frame ) - PAGE_SIZE );

    //print page table in binary form
    for ( i = 0; i < end_of_page_table; i++ )
    {
        ptvec.push_back ( memory.read ( i ) );
    }




}
