#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <stdlib.h>
#include "Binary.h"
#include "Buffer.h"
#include <memory.h>
#include "Debug.h"

struct FILE_INFO
{
	uint32_t value1;
	uint64_t value2;
};

struct Data
{
	uint32_t value1;
	uint32_t value2;
	uint32_t value3;
	uint32_t value4;
	uint32_t value5;
	uint32_t value6;
	uint32_t value7;
	uint32_t value8;
	uint32_t value9;
	uint32_t value10;
};


void print_log(BUFFER* buffer)
{
	BINget_bytes_string(buffer);
	BUFbind(buffer); 
	const char* log_buffer = BUFget_ptr();
	fputs(log_buffer , stdout);
}
void print_address(const char* discription , void* address)
{
	printf("%s : %d\n",discription, address); 
}

void at__exit() { puts("Successfully exited"); }

int main()
{ 
	
	atexit(at__exit); 

	return 0;
}