#include "memory_simulation.h"




mem_unit::mem_unit()
{
	//initialize block of memory to all zeros
	for(int i = 0; i < MEM_SIZE; i++)
		mem[i] = ERROR;
}

mem_unit::~mem_unit()
{
}

bool mem_unit::write(data d, address addr)
{
	mem[addr] = d;
	return true;
}

data mem_unit::read(address addr)
{
	return mem[addr];
}

void mem_unit::print()
{
	for(int i = 0; i < MEM_SIZE; i++)
		cout << "addr " << i << ": " << (bitset<sizeof(data)*8>) mem[i] << endl;

}
