#include "Buffer.h"
#include "Debug.h"
#include "Binary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STACK_ALLOCATED_BYTES 	4
#define HEAP_ALLOCATED_BYTES 	8
#define HEAP_ALLOCATED_OBJECT   16
#define STACK_ALLOCATED_OBJECT 	32

struct __BINARY 
{
	uint8_t* bytes;
	uint32_t count;
	uint8_t info;
};

struct __BINARY* binded_binary = NULL;

uint8_t* get_byte_at(uint32_t index) { return binded_binary->bytes + binded_binary->count - index - 1; }
static void rev_memset(uint8_t* buffer, int value , int size)
{
	while(size)
	{
		*buffer = value;
		--buffer;
		--size;
	}
}

static void rev_mempcpy(uint8_t* dst, uint8_t* src, int size)
{
	while(size)
	{
		*dst = *src;
		--dst;
		--src;
		--size;
	}
}

void BINset_bytes(uint8_t* bytes) { binded_binary->bytes = bytes; }
void BINset_byte_count(uint32_t count) { binded_binary->count = count; }
void BINset_endianess(uint8_t endianess)
{
	if((binded_binary->info & BIN_BIG_ENDIAN) && (endianess != BIN_BIG_ENDIAN))
	{
		binded_binary->info &= (~BIN_BIG_ENDIAN);
		binded_binary->info |= BIN_LITTLE_ENDIAN;
	}
	else
	{
		binded_binary->info &= (~BIN_LITTLE_ENDIAN);
		binded_binary->info |= BIN_BIG_ENDIAN;
	}
}
uint8_t* BINget_bytes() { return binded_binary->bytes; }
uint32_t BINget_byte_count() { return binded_binary->count; } 
uint8_t BINget_endianess()
{
	if(binded_binary->info & BIN_BIG_ENDIAN) return BIN_BIG_ENDIAN;
	else if(binded_binary->info & BIN_LITTLE_ENDIAN) return BIN_LITTLE_ENDIAN;
}

void BINprint_info()
{	
	printf("nBytes : %u\n" , binded_binary->count);
}
void BINlittle_endian(BINlayout* __layout__, uint32_t count)
{
	if(BINget_endianess() == BIN_LITTLE_ENDIAN) return;
	BINreverse_byte_order(__layout__, count); 
	BINdelete_bit_mask8(binded_binary->info , BIN_BIG_ENDIAN); 
	BINadd_bit_mask8(binded_binary->info , BIN_LITTLE_ENDIAN);
}

void BINbig_endian(BINlayout* __layout__ , uint32_t count)
{
	if(BINget_endianess() == BIN_BIG_ENDIAN) return;
	BINreverse_byte_order(__layout__, count); 
	BINdelete_bit_mask8(binded_binary->info , BIN_LITTLE_ENDIAN);
	BINadd_bit_mask8(binded_binary->info , BIN_BIG_ENDIAN);
}

void BINreverse_bit_order(BINlayout* __layout__ , uint32_t count)
{
	if(__layout__ == NULL)
	{
		BINreverse_byte_order(NULL , 0); 
		uint32_t byte_count = binded_binary->count;
		while(byte_count)
		{
			--byte_count;
			BINreverse_bits8(*(binded_binary->bytes + byte_count)); 
		}
	}
	else
	{
		int x = 5;
	}
}
void BINreverse_bit_orderb(uint8_t* __bytes__ , uint32_t count , uint32_t __lower_b__ , uint32_t __upper_b__) 
{
	int g = 10;
}
void BINreverse_byte_orderb(uint8_t* __bytes__ , uint32_t count)
{
	if(count < 2) return; 
	int32_t mid_count = (count >> 1) - 1;
	uint8_t hold_byte;
	--count;
	while(mid_count >= 0)
	{
		hold_byte = *(__bytes__ + mid_count);
		*(__bytes__ + mid_count) = *(__bytes__ + (count - mid_count));
		*(__bytes__ + (count - mid_count)) = hold_byte;
		--mid_count;
	}
}
void BINreverse_byte_order(BINlayout* __layout__ , uint32_t count)
{
	if(__layout__ != NULL)
	{
		uint8_t* __bytes__ = binded_binary->bytes + binded_binary->count;
		while(count > 0)
		{
			--count;
			uint8_t value = *(*__layout__ + count) ; 
			__bytes__ -= (*(*__layout__ + count));
			BINreverse_byte_orderb(__bytes__ , *(*__layout__ + count));
		}
	}
	else
	{
		if(binded_binary->count < 2) return; 
		int32_t mid_count = (binded_binary->count >> 1);
		--mid_count;
		uint8_t hold_byte;
		--(binded_binary->count);
		while(mid_count >= 0)
		{
			hold_byte = *(binded_binary->bytes + mid_count);
			*(binded_binary->bytes + mid_count) = *(binded_binary->bytes + (binded_binary->count - mid_count));
			*(binded_binary->bytes + (binded_binary->count - mid_count)) = hold_byte;
			--mid_count;
		}
		++(binded_binary->count) ; 
	}
}

void BINset_bits_in_range(uint32_t __lower_b__ , uint32_t __upper_b__)
{
	uint32_t lower_byte_index = (__lower_b__ >> 3);
	uint32_t upper_bytes_index = (__upper_b__ >> 3); 
	if(lower_byte_index == upper_bytes_index)
		BINset_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index) , __lower_b__ % 8, __upper_b__ % 8);
	else
	{
		BINset_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index), __lower_b__ % 8 , 7); 
		BINset_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - upper_bytes_index), 0, __upper_b__ % 8);
	    upper_bytes_index = (upper_bytes_index - lower_byte_index - 1);
		while(upper_bytes_index > 0)
		{
			++lower_byte_index;
			BINset_all_bits8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index));
			--upper_bytes_index;
		}
	}
}

void BINunset_bits_in_range(uint32_t __lower_b__ , uint32_t __upper_b__)
{
	uint32_t lower_byte_index = (__lower_b__ >> 3);
	uint32_t upper_bytes_index = (__upper_b__ >> 3); 
	if(lower_byte_index == upper_bytes_index)
		BINunset_bits_in_range8(*(binded_binary->bytes +  binded_binary->count - 1 - lower_byte_index) , __lower_b__ % 8, __upper_b__ % 8);
	else
	{
		BINunset_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index), __lower_b__ % 8 , 7); 
		BINunset_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - upper_bytes_index), 0, __upper_b__ % 8);
	    upper_bytes_index = (upper_bytes_index - lower_byte_index - 1);
		while(upper_bytes_index > 0)
		{
			++lower_byte_index;
			BINunset_all_bits8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index));
			--upper_bytes_index;
		}
	}	
}

void BINfree()
{
	if(binded_binary == NULL) return;
	if((binded_binary->info & HEAP_ALLOCATED_BYTES) && (binded_binary->bytes != NULL))
	{	
		free(binded_binary->bytes);
		binded_binary->count = 0;
		binded_binary->bytes = NULL; 
	}
	if(binded_binary->info & HEAP_ALLOCATED_OBJECT)
	 {  binded_binary->info = 0x00;	free(binded_binary); } 
	binded_binary = NULL;
}

BINARY* BINcreate_object(uint8_t* bytes)
{
	struct __BINARY* binary =  (struct __BINARY*)(bytes); 
	binary->bytes = NULL;
	binary->count = 0;
	binary->info = 0x00;
	binary->info |= STACK_ALLOCATED_OBJECT;
	return binary;
}

BINARY* BINserialize(BINARY* __binary__, void* __object__ , uint32_t __size__, uint8_t endianess)
{
	struct __BINARY* binary;
	if(__binary__ == NULL)
	{
	 	binary = malloc(sizeof(struct __BINARY)); 
	 	binary->info = 0x00;
	 	binary->info |= HEAP_ALLOCATED_OBJECT;
	}
	else
		binary = __binary__;
	binary->bytes = (uint8_t*)__object__;
	binary->count = __size__;
	if((binary->info & BIN_LITTLE_ENDIAN) && (endianess == BIN_BIG_ENDIAN))
		binary->info &= (~BIN_LITTLE_ENDIAN);
	if((binary->info & BIN_BIG_ENDIAN) && (endianess == BIN_LITTLE_ENDIAN))
		binary->info &= (~BIN_BIG_ENDIAN);
	binary->info |= endianess;
	if(binary->info & HEAP_ALLOCATED_BYTES)
		binary->info &= (~HEAP_ALLOCATED_BYTES);
	binary->info |= STACK_ALLOCATED_BYTES;
	return binary;
}
BINARY* BINcreate(BINARY* __binary__, uint32_t nbytes, uint8_t endianess)
{
	struct __BINARY* binary;
	if(__binary__ == NULL)
	{
		binary = malloc(sizeof(struct __BINARY));
		binary->info = 0x00;
		binary->info |= HEAP_ALLOCATED_OBJECT;
	}
	else
		binary = __binary__;
	binary->bytes = malloc(nbytes); 
	if((binary->info & BIN_LITTLE_ENDIAN) && (endianess == BIN_BIG_ENDIAN))
		binary->info &= (~BIN_LITTLE_ENDIAN);
	if((binary->info & BIN_BIG_ENDIAN) && (endianess == BIN_LITTLE_ENDIAN))
		binary->info &= (~BIN_BIG_ENDIAN);
	binary->info |= endianess;

	if(binary->info & STACK_ALLOCATED_BYTES)
		binary->info &= (~STACK_ALLOCATED_BYTES);
	binary->info |= HEAP_ALLOCATED_BYTES;
	return binary;
}

void BINunset_all_bits()
{
	uint32_t byte_count = binded_binary->count; 
	while(byte_count > 0)
	{
		--byte_count;
		BINunset_all_bits8(*(binded_binary->bytes + byte_count));
	}
}
void BINset_all_bits()
{
	uint32_t byte_count = binded_binary->count;
	while(byte_count > 0)
	{
		--byte_count;
		BINset_all_bits8(*(binded_binary->bytes + byte_count)); 
	}
}
void BINunset_bit(uint32_t index)
{
	uint32_t byte_index = index >> 3; 
	BINunset_bit8(*(binded_binary->bytes + binded_binary->count - byte_index - 1) , index % 8); 
}
void BINset_bit(uint32_t index)
{
	uint32_t byte_index = index >> 3;
	BINset_bit8(*(binded_binary->bytes + binded_binary->count - byte_index  - 1), index % 8);
}

uint8_t BINget_bit(uint32_t index)
{
	uint32_t byte_index = index >> 3;
	return BINget_bit8(*(binded_binary->bytes + binded_binary->count - byte_index - 1), index % 8);
}

BUFFER* BINcreate_string_buffer(BUFFER* buffer)
{
	return BUFcreate(buffer,/*element size*/11, /*capacity = */binded_binary->count , /*offset = */1); 
}

void BINget_bytes_string(BUFFER* out_buffer)
{
	char temp_buffer[12];
	uint32_t byte_count = 0;
	BUFbind(out_buffer);  
	BUFset_element_count(0); 
	while(byte_count < binded_binary->count)
	{
		BINget_byte_stringb(*(binded_binary->bytes + byte_count), temp_buffer);
		BUFpush(temp_buffer);
		++byte_count;
	}
	*(BUFget_ptr() + BUFget_element_count() * BUFget_element_size()); 
	BUFunbind(); 
}

void BINget_byte_stringb(uint8_t __byte__, char* out_string_buffer)
{
	signed char count = 7;
	while(count >= 0)
	{
		out_string_buffer[8 - count] = (__byte__ & (0x01 << count)) ? '1' : '0'; 
		--count;
	}
	out_string_buffer[0] = '[';
	out_string_buffer[9] = ']';
	out_string_buffer[10] ='\n';
	out_string_buffer[11] = 0;
}

void BINbind(BINARY* binary) { binded_binary = (struct __BINARY*)binary;  } 
void BINunbind() { binded_binary = NULL; }

void BINinvert_bit(uint32_t index)
{
	uint32_t byte_index = index >> 3; 
	uint32_t local_bit_index = index - (byte_index << 3);
	BINinvert_bit8(*(binded_binary->bytes + byte_index) , local_bit_index);
}

void BINinvert_bits_in_range(uint32_t __lower_b__ , uint32_t __upper_b__)
{
	uint32_t lower_byte_index = (__lower_b__ >> 3);
	uint32_t upper_bytes_index = (__upper_b__ >> 3); 
	if(lower_byte_index == upper_bytes_index)
	    BINinvert_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index) , __lower_b__ % 8, __upper_b__ % 8) /*scoped*/
	else
	{
		BINinvert_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index), __lower_b__ % 8 , 7) /*scoped*/
		BINinvert_bits_in_range8(*(binded_binary->bytes + binded_binary->count - 1 - upper_bytes_index), 0, __upper_b__ % 8) /*scoped*/
	    upper_bytes_index = (upper_bytes_index - lower_byte_index - 1);
		while(upper_bytes_index > 0)
		{
			++lower_byte_index;
			BINinvert_all_bits8(*(binded_binary->bytes + binded_binary->count - 1 - lower_byte_index));
			--upper_bytes_index;
		}
	}
}
void BINinvert_all_bits()
{
	uint32_t byte_count = binded_binary->count;
	while(byte_count)
	{
		--byte_count;
		BINinvert_all_bits8(*(binded_binary->bytes + byte_count));
	}
}
void BINxor(BINARY* __binary__)
{
	struct __BINARY* _another_binary = (struct __BINARY*)__binary__;
	uint32_t min_byte_count = (binded_binary->count < _another_binary->count) ? binded_binary->count : _another_binary->count;
	while(min_byte_count > 0)
	{
		--min_byte_count;
		*(binded_binary->bytes + min_byte_count) ^= *(_another_binary->bytes + min_byte_count);
	}
}
void BINand(BINARY* __binary__)
{
	struct __BINARY* _another_binary = (struct __BINARY*)__binary__;
	uint32_t min_byte_count = (binded_binary->count < _another_binary->count) ? binded_binary->count : _another_binary->count;
	while(min_byte_count > 0)
	{
		--min_byte_count;
		*(binded_binary->bytes + min_byte_count) &= *(_another_binary->bytes + min_byte_count);
	}
}
void BINor(BINARY* __binary__)
{
	struct __BINARY* _another_binary = (struct __BINARY*)__binary__;
	uint32_t min_byte_count = (binded_binary->count < _another_binary->count) ? binded_binary->count : _another_binary->count;
	while(min_byte_count > 0)
	{
		--min_byte_count;
		*(binded_binary->bytes + min_byte_count) |= *(_another_binary->bytes + min_byte_count);
	}
}

void BINshift_bits_left_in_range(uint32_t bit_count , uint32_t lower_b, uint32_t upper_b)
{
	if((upper_b < lower_b) || (bit_count == 0)) return; 
	uint8_t upper_byte_bit_index = upper_b % 8;
	uint8_t lower_byte_bit_index = lower_b % 8;
	uint32_t included_bytes;
	uint8_t mask;
	uint8_t hold_bits;
	uint8_t* upper_bytes = get_byte_at(upper_b >> 3);
	if(bit_count > 0x07)
	{
		uint32_t byte_shift = (bit_count >> 3); 
		uint32_t shift_bits_bottom_index = (upper_b - (byte_shift << 3) + 1); 
		if(((upper_b + 1) <= (lower_b + (byte_shift << 3))))
		{
			BINunset_bits_in_range(lower_b, upper_b);
		} 
		else
		{
			BINunset_bits_in_range(shift_bits_bottom_index, upper_b);
			--shift_bits_bottom_index;
			uint8_t rest_bytes_top_index = (shift_bits_bottom_index >> 3);
			uint8_t* rest_bytes = get_byte_at(rest_bytes_top_index); 
			if(rest_bytes_top_index == (lower_b >> 3))
			{
				hold_bits = BINget_bits_in_range8(*rest_bytes, lower_byte_bit_index, shift_bits_bottom_index % 8);
				uint8_t included_bits = ((shift_bits_bottom_index % 8) - (lower_byte_bit_index) + 1); 
				hold_bits <<= ((upper_byte_bit_index) - included_bits  + 1);
				*upper_bytes |= hold_bits;
				BINunset_bits_in_range8(*rest_bytes, lower_byte_bit_index, shift_bits_bottom_index % 8); 
			}
			else
			{
				hold_bits = *rest_bytes;
				if((shift_bits_bottom_index % 8) > (upper_byte_bit_index))
					hold_bits >>= ((upper_byte_bit_index) - (shift_bits_bottom_index % 8));
				else
					hold_bits <<= ((upper_byte_bit_index) - (shift_bits_bottom_index % 8));
				*upper_bytes |= hold_bits;
				*rest_bytes = 0x00;

				uint32_t num_rest_bytes = (rest_bytes_top_index - (lower_b >> 3));
				if(num_rest_bytes >= 1) num_rest_bytes -= 1;

				upper_bytes++;
				rest_bytes++;
				if(num_rest_bytes > 0)
				{	
					mempcpy(upper_bytes, rest_bytes, num_rest_bytes); 
					memset(upper_bytes + num_rest_bytes, 0, rest_bytes - upper_bytes); 
				}
				rest_bytes += num_rest_bytes;
				upper_bytes += num_rest_bytes;
				*upper_bytes = (*rest_bytes & BINget_set_bit_mask_in_range8(lower_byte_bit_index, 7));
				BINunset_bits_in_range8(*rest_bytes, lower_byte_bit_index, 7); 
			}
		}
	}
	uint8_t local_bit_shift = bit_count % 8;
	if(local_bit_shift == 0) return; 
	upper_bytes = get_byte_at(upper_b >> 3); 
	included_bytes = (upper_b >> 3); 
 	included_bytes -= (lower_b >> 3);
 	if(included_bytes >= 1) included_bytes -= 1;
 	else
 	{
 		BINshift_bits_left_in_range8(*upper_bytes, local_bit_shift, lower_byte_bit_index, upper_byte_bit_index); 
 		return;
 	}

	BINshift_bits_left_in_range8(*upper_bytes, local_bit_shift, 0, upper_byte_bit_index); 
	++upper_bytes;
	hold_bits = *upper_bytes;
    if((upper_b - local_bit_shift + 1) < lower_b) /*if lower bound is just below the upper bound*/
    	hold_bits &= BINget_set_bit_mask_in_range8(lower_byte_bit_index, 7); 
	hold_bits >>= (8 - local_bit_shift);

	*(upper_bytes - 1) |= hold_bits;
	
	if(included_bytes == 0)
	{
		BINshift_bits_left_in_range8(*upper_bytes, local_bit_shift, lower_byte_bit_index, 7); 
		return;
	} 	
	if(included_bytes >= 1)
	{
		while(included_bytes > 1)
		{
			hold_bits = BINget_bits_in_range8(*(upper_bytes + 1), 8 - local_bit_shift, 7); 
			*upper_bytes <<= local_bit_shift;
			*upper_bytes |= hold_bits;
			++upper_bytes;
			--included_bytes;
		}
		*upper_bytes <<= local_bit_shift;
		if(local_bit_shift > (8 - (lower_byte_bit_index)))
		{
			hold_bits = BINget_bits_in_range8(*(upper_bytes + 1), lower_byte_bit_index, 7); 
			hold_bits <<= (local_bit_shift - (8 - (lower_byte_bit_index)));
			mask = BINget_set_bit_mask_in_range8(0, local_bit_shift - 1); 
			hold_bits &= mask;
			*upper_bytes |= hold_bits;
			++upper_bytes;
			BINunset_bits_in_range8(*upper_bytes, lower_byte_bit_index, 7); 
		}
		else
		{
			hold_bits = BINget_bits_in_range8(*(upper_bytes + 1), 8 - local_bit_shift, 7); 
			*upper_bytes |= hold_bits;
			++upper_bytes;
			BINshift_bits_left_in_range8(*upper_bytes , local_bit_shift, lower_byte_bit_index , 7);
		}
	}
}

void BINshift_bits_right_in_range(uint32_t bit_count , uint32_t lower_b, uint32_t upper_b)
{
	if((upper_b < lower_b) || (bit_count == 0)) return; 
	uint8_t upper_byte_bit_index = upper_b % 8;
	uint8_t lower_byte_bit_index = lower_b % 8;
	uint32_t included_bytes;
	uint8_t mask;
	uint8_t hold_bits;
	uint8_t* lower_bytes = get_byte_at(lower_b >> 3);
	if(bit_count > 0x07)
	{
		uint32_t byte_shift = (bit_count >> 3); 
		uint32_t shift_bits_top_index = (lower_b + (byte_shift << 3) - 1); 
		if(((lower_b + (byte_shift << 3))) >= (upper_b + 1)) 
			BINunset_bits_in_range(lower_b, upper_b);
		else
		{
			BINunset_bits_in_range(lower_b, shift_bits_top_index);
			++shift_bits_top_index;
			uint8_t rest_bytes_bottom_index = (shift_bits_top_index >> 3);
			uint8_t* rest_bytes = get_byte_at(rest_bytes_bottom_index); 
			if(rest_bytes_bottom_index == (upper_b >> 3))
			{ 
				hold_bits = BINget_bits_in_range8(*rest_bytes, shift_bits_top_index % 8, upper_byte_bit_index);
				hold_bits <<= (lower_byte_bit_index); 
				*lower_bytes |= hold_bits;
				BINunset_bits_in_range8(*rest_bytes, shift_bits_top_index % 8, upper_byte_bit_index); 
			}
			else
			{
				hold_bits = *rest_bytes;
				*lower_bytes |= hold_bits;
				*rest_bytes = 0x00;

				uint32_t num_rest_bytes = ((upper_b >> 3) - rest_bytes_bottom_index);
				if(num_rest_bytes >= 1) num_rest_bytes -= 1;

				--lower_bytes;
				--rest_bytes;
				if(num_rest_bytes > 0)
				{	
					rev_mempcpy(lower_bytes, rest_bytes, num_rest_bytes); 
					rev_memset(lower_bytes - num_rest_bytes, 0, lower_bytes - rest_bytes); 
				}
				rest_bytes -= num_rest_bytes;
				lower_bytes -= num_rest_bytes;
				*lower_bytes = (*rest_bytes & BINget_set_bit_mask_in_range8(0, upper_byte_bit_index));
				BINunset_bits_in_range8(*rest_bytes, 0, upper_byte_bit_index); 
			}
		}
 	}
	uint8_t local_bit_shift = bit_count % 8;
	if(local_bit_shift == 0) return; 
	lower_bytes = get_byte_at(lower_b >> 3); 
	included_bytes = (upper_b >> 3); 
 	included_bytes -= (lower_b >> 3);
 	if(included_bytes >= 1) included_bytes -= 1;
 	else
 	{
 		/*if lower bound and upper boudn belongs to the same byte*/
 		BINshift_bits_right_in_range8(*lower_bytes, local_bit_shift, lower_byte_bit_index, upper_byte_bit_index); 
 		return;
 	}

	BINshift_bits_right_in_range8(*lower_bytes, local_bit_shift, lower_byte_bit_index, 7); 
	--lower_bytes;
	hold_bits = *lower_bytes;
    if((lower_b + local_bit_shift - 1) > upper_b) /*When the upper bound is just above the lower bound */
    	hold_bits &= BINget_set_bit_mask_in_range8(0, upper_byte_bit_index); 
	hold_bits <<= (8 - local_bit_shift);
    *(lower_bytes + 1) |= hold_bits;

	if(included_bytes == 0)
	{
		BINshift_bits_right_in_range8(*lower_bytes, local_bit_shift, 0, upper_byte_bit_index); 
		return;
	} 	

	if(included_bytes >= 1)
	{
		while(included_bytes > 1)
		{
			hold_bits = BINget_bits_in_range8(*(lower_bytes - 1), 0, local_bit_shift - 1); 
			hold_bits <<= (8 - local_bit_shift);
			*lower_bytes >>= local_bit_shift;
			*lower_bytes |= hold_bits;
			--lower_bytes;
			--included_bytes;
		}
		*lower_bytes >>= local_bit_shift;
		if(local_bit_shift > ((upper_byte_bit_index) + 1)) /*if the local_bit_shift will clear all the range bits in upper bound byte*/
		{
			hold_bits = BINget_bits_in_range8(*(lower_bytes - 1), 0, upper_byte_bit_index); 
			hold_bits <<= (8 - local_bit_shift);
			*lower_bytes |= hold_bits;
			--lower_bytes;
			BINunset_bits_in_range8(*lower_bytes, 0, upper_byte_bit_index); 
		}
		else
		{
			hold_bits = BINget_bits_in_range8(*(lower_bytes - 1), 0, 8 - local_bit_shift); 
			hold_bits <<= (8 - local_bit_shift);
			*lower_bytes |= hold_bits;
			--lower_bytes;
			BINshift_bits_right_in_range8(*lower_bytes , local_bit_shift, 0, upper_byte_bit_index);
		}
	}
}