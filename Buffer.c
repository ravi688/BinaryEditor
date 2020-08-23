#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Buffer.h"
#include "Debug.h"

#define STACK_ALLOCATED_OBJECT  0x1
#define HEAP_ALLOCATED_OBJECT 	 0x2

struct __BUFFER 
{
	uint8_t* bytes;
	uint32_t info; 
	uint32_t capacity;
	uint32_t element_count;		
	uint32_t element_size;
};
struct __BUFFER* binded_buffer;

uint32_t BUFget_capacity() { return binded_buffer->capacity; }
uint32_t BUFget_element_count() { return binded_buffer->element_count; } 
uint32_t BUFget_element_size() { return binded_buffer->element_size;  } 
uint8_t* BUFget_ptr() { return binded_buffer->bytes; } 

void BUFset_capacity(uint32_t capacity) { binded_buffer->capacity = capacity; } 
void BUFset_element_count(uint32_t element_count) { binded_buffer->element_count = element_count; } 
void BUFset_element_size(uint32_t element_size) { binded_buffer->element_size = element_size; } 
void BUFset_ptr(uint8_t* ptr) { binded_buffer->bytes = ptr; } 

void BUFbind(BUFFER* buffer) { binded_buffer = buffer; } 
void BUFunbind() { binded_buffer = NULL; } 

void BUFlog()
{
	printf("ElementCount :\t%u\n"
		   "Capacity :\t%u\n"
		   "ElementSize :\t%u\n",
		    binded_buffer->element_count,
		    binded_buffer->capacity,
		    binded_buffer->element_size
			);
}

void BUFfree()
{
	if(binded_buffer == NULL) return;
	if(binded_buffer->bytes != NULL)
	{
		free(binded_buffer->bytes);  
		binded_buffer->bytes == NULL;
	}

    if(binded_buffer->info & HEAP_ALLOCATED_OBJECT)
    { binded_buffer->info = 0x00; free(binded_buffer) ; } 
	binded_buffer = NULL;
}

BUFFER* BUFcreate_object(uint8_t* bytes)
{
	struct __BUFFER* buffer = (struct __BUFFER*)(bytes);
	buffer->bytes = NULL;
	buffer->info = 0x00;
	buffer->info |= STACK_ALLOCATED_OBJECT; 
	buffer->element_size = 0;
	buffer->element_count = 0;
	buffer->capacity = 0;
	return buffer;
}

BUFFER* BUFcreate(BUFFER* __buffer, uint32_t element_size, uint32_t capacity, uint32_t offset)
{
	struct __BUFFER* buffer; 
	if(__buffer == NULL)
	{
		buffer = malloc(sizeof(struct __BUFFER));
		buffer->info = 0x00;
		buffer->info |= HEAP_ALLOCATED_OBJECT;
	}
	else 
		buffer = __buffer; 

	if(buffer == NULL) return NULL;
	buffer->bytes = (uint8_t*)malloc(element_size * capacity + offset); 
	buffer->capacity = capacity;
	buffer->element_count = 0;
	buffer->element_size = element_size;
	return buffer;
}

void BUFget_at(uint32_t index ,  void* out_value)
{	
	memcpy(out_value , binded_buffer->bytes + index * binded_buffer->element_size, binded_buffer->element_size); 
}
void BUFset_at(uint32_t index , void* in_value)
{
	memcpy(binded_buffer->bytes + index * binded_buffer->element_size, in_value , binded_buffer->element_size) ; 
}

void BUFclear(void* clear_value)
{
	if(clear_value == NULL)
		memset(binded_buffer->bytes , 0 , binded_buffer->element_count * binded_buffer->element_size) ; 
	else
	{
		uint32_t element_count = binded_buffer->element_count;
		while(element_count > 0)
		{
			--element_count;
			memcpy(binded_buffer->bytes + element_count * binded_buffer->element_size, clear_value , binded_buffer->element_size); 
		}
	} 
}

void BUFinsert_at_noalloc(uint32_t index , void* in_value , void* out_value)
{
	if(out_value != NULL)
		memcpy(out_value , binded_buffer->bytes + index * binded_buffer->element_size , binded_buffer->element_size) ; 
	memcpy(binded_buffer->bytes + index * binded_buffer->element_size, in_value , binded_buffer->element_size) ;  
}

void BUFinsert_at(uint32_t index , void* in_value)
{
	++(binded_buffer->element_count); 
	if((binded_buffer->element_count) > binded_buffer->capacity)
		binded_buffer->bytes = realloc(binded_buffer->bytes , binded_buffer->element_size * binded_buffer->element_count) ; 

	uint8_t* dst_ptr = binded_buffer->bytes + (binded_buffer->element_count - 1) * binded_buffer->element_size;
	uint32_t num_shift_elements = (binded_buffer->element_count - index - 1); 
	while(num_shift_elements)
	{
		memcpy(dst_ptr , (dst_ptr - binded_buffer->element_size) , binded_buffer->element_size) ; 
	    dst_ptr -= binded_buffer->element_size;
		--num_shift_elements;
	}
	memcpy(dst_ptr , in_value , binded_buffer->element_size) ; 
}

void BUFremove_at_noshift(uint32_t index , void* out_value)
{
	if(out_value != NULL)
		memcpy(out_value , binded_buffer->bytes + index * binded_buffer->element_size , binded_buffer->element_size) ;
	memset(binded_buffer->bytes + index * binded_buffer->element_size , 0 , binded_buffer->element_size); 
}

void BUFremove_at(uint32_t index , void* out_value)
{
	--(binded_buffer->element_count); 
	if(out_value != NULL)
		mempcpy(out_value , binded_buffer->bytes + index * binded_buffer->element_size , binded_buffer->element_size); 

	uint8_t* dst_ptr = binded_buffer->bytes + index * binded_buffer->element_size;
	if(index <= binded_buffer->element_count)
	{
		uint32_t num_shift_elements = (binded_buffer->element_count - index) ;
		while(num_shift_elements > 0)
		{
			memcpy(dst_ptr , dst_ptr + binded_buffer->element_size, binded_buffer->element_size); 
			dst_ptr += binded_buffer->element_size;
			--num_shift_elements;
		}
	}
	memset(dst_ptr , 0 , binded_buffer->element_size); 
}
void BUFfit()
{
	binded_buffer->bytes = (uint8_t*)realloc(binded_buffer->bytes , binded_buffer->element_count * binded_buffer->element_size); 
	binded_buffer->capacity = binded_buffer->element_count;
}
void BUFpeek(void* out_value)
{
	memcpy(out_value , binded_buffer->bytes + (binded_buffer->element_count - 1) * binded_buffer->element_size , binded_buffer->element_size) ;   
}
void BUFpop(void* out_value)
{
	--(binded_buffer->element_count);
	memcpy(out_value , binded_buffer->bytes + binded_buffer->element_count * binded_buffer->element_size , binded_buffer->element_size) ; 
}

void BUFpush(void* in_value)
{
	++(binded_buffer->element_count); 
	uint32_t previous_capacity = binded_buffer->capacity;
	while(binded_buffer->capacity < binded_buffer->element_count)
		binded_buffer->capacity *= 2;
	if(previous_capacity != binded_buffer->capacity)
		binded_buffer->bytes = realloc(binded_buffer->bytes , binded_buffer->capacity * binded_buffer->element_size); 
	memcpy(binded_buffer->bytes + (binded_buffer->element_count - 1) * binded_buffer->element_size , in_value , binded_buffer->element_size); 
}