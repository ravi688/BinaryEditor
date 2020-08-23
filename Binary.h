#ifndef _BINARY_H_
#define _BINARY_H_

#include <stdint.h>
#include "Buffer.h"

/*
 | BIN_BINARY_OBJECT_SIZE equals sizeof(BINARY)
 | Useful if you want to allocate memory for BINARY object explicity
 | To allocate memory for BINARY object explicitly use this : 
 | BINARY* binary = BINcreate(0, BIN_LITTLE_ENDIAN or BIN_BIG_ENDIAN); //allocated in the heap
 | NOTE : you must pair the functions BINcreate and BINserialize with BINfree()
 | or if you want to allocate in the stack memory, use this : 
 | char object_bytes[BIN_BINARY_OBJECT_SIZE];
 | BINARY* binary = BINcreate_object(object_bytes);
 | NOTE : unlike the heap allocation you don't have to free this memory
 */
#define BIN_BINARY_OBJECT_SIZE 	16 /*bytes*/
#define BIN_LITTLE_ENDIAN 		1
#define BIN_BIG_ENDIAN 			2


/*
 | Usage Instructions : 
 | Every BINARY object must be binded after the creating them to work with them
 | 
 | Example : 
 | 
 | BINARY* binary = BINcreate(4, BIN_LITTLE_ENDIAN);  //creates a buffer of 4 bytes in the heap and allocates memory of BIN_BINARY_OBJECT_SIZE in the heap
 | BINbind(binary); 								  //bind it to work with it
 | //some manipulations
 | BINfree(); 										  //free up the most recent binded binary if it was not unbinded, otherwise you would have to bind it first.
 */


/*NOTE : all the macros suffixed with 8 will work for only a single byte */
/*
 | Returns the setted bits in range (__lower_b__, __upper_b__)
 | Ex : 
 | uint8_t byte = BINget_set_bit_mask_in_range(0, 3);
 | would results 
 | byte = 0 0 0 0 0 1 1 1
 */
#define BINget_set_bit_mask_in_range8(__lower_b__ , __upper_b__) ((0xff >> (7 - (__upper_b__))) & (0xff << (__lower_b__)))
/*
 | Returns the unsetted bits in range (__lower_b__, __upper_b__)
 | Ex : 
 | uint8_t byte = BINget_unset_bit_mask_in_range8(0, 5);
 | would results
 | byte = 1 1 1 0 0 0 0 0
 */
#define BINget_unset_bit_mask_in_range8(__lower_b__ , __upper_b__) (~BINget_set_bit_mask_in_range8(__lower_b__, __upper_b__))
/*
 | Sets number of bits in range (__lower_b__, __upper_b__)
 | example : 
 | uint8_t byte = 0x00;
 | BINset_bits_in_range8(byte, 2, 4);
 | would results : 
 | byte = 0 0 0 1  1 1 0 0
 */
#define BINset_bits_in_range8(__byte__ , __lower_b__, __upper_b__) (__byte__) |= BINget_set_bit_mask_in_range8(__lower_b__ , __upper_b__)
/*
 | Unsets number of bits in range (__lower_b__, __upper_b__);
 | example : 
 | uint8_t byte = 0xff;
 | BINunset_bits_in_range8(byte, 2, 4);
 | would results : 
 | byte = 1 1 0 0  0 0 1 1
 */
#define BINunset_bits_in_range8(__byte__ , __lower_b__ , __upper_b__) (__byte__) &= BINget_unset_bit_mask_in_range8(__lower_b__ , __upper_b__)
/*Obvious to you*/
#define BINapply_bit_mask8(__byte__, __mask__) (__byte__) &= (__mask__)
/*Obvious to you*/
#define BINdelete_bit_mask8(__byte__ , __mask__) (__byte__) &= (~(__mask__))
/*Obvious to you*/
#define BINadd_bit_mask8(__byte__, __mask__) (__byte__) |= (__mask__)
/*Obvious to you*/
#define BINset_all_bits8(__byte__) (__byte__) |= 0xff
/*Obvious to you*/
#define BINunset_all_bits8(__byte__) (__byte__) &= 0x00
/*
 | Sets a particular bit 
 | Example : 
 | uint8_t byte = 0x00;
 | BINset_bit8(byte, 2); 
 | would results : 
 | byte = 0 0 0 0  0 1 0 0 
 */
#define BINset_bit8(__byte__, __index__) (__byte__) |= (0x01 << ((__index__)))
/*
 | Unsets a particular bit
 | Example : 
 | uint8_t byte = 0xff;
 | BINunset_bit8(byte, 2); 
 | would results : 
 | byte = 1 1 1 1  1 0 1 1
 */
#define BINunset_bit8(__byte__ , __index__) (__byte__) &= (~(0x01 << (__index__)))
/*
 | Returns 0x01 or 0x00 if the bit at index is set or unset repectively
 | Example : 
 | unit8_t byte = 0x08; // 0 0 0 0  0 1 0 0
 | uint8_t result = BINget_bit8(byte, 3);
 | would results : 
 | result = 0 0 0 0  0 0 0 1
 | In contrast : 
 | result = BINget_bit8(byte, 7); 
 | would results : 
 | result = 0 0 0 0  0 0 0 0
 */
#define BINget_bit8(__byte__, __index__) (((__byte__) & (0x01 << (__index__))) >> (__index__))
/*
 | Returns bits in range (__lower_b__, __upper_b__)
 | Example 1 : 
 | uint8_t byte = 0x00;
 | BINset_bit(byte, 1);
 | BINset_bit(byte, 3);
 | BINset_bit(byte, 4);    // byte = 0 0 0 1  1 0 1 0
 | uint8_t retrieved_bits = BINget_bits_in_range8(byte, 0, 3);
 | would results : 
 | retrieved_bits = 0 0 0 0  1 0 1 0
 |
 | Example 2 : 
 | uint8_t retrieved_bits = BINget_bits_in_range8(byte, 3, 5); 
 | would result : 
 | retrieved_bits = 0 0 0 0  0 0 1 1;
 */
#define BINget_bits_in_range8(__byte__, __lower_b__, __upper_b__) ((__byte__ & BINget_set_bit_mask_in_range8(__lower_b__, __upper_b__)) >> (__lower_b__))
/*
 | Inverts a bit at an index '__index__'
 | Example : 
 | uint8_t byte = 0x00;
 | BINinvert_bit8(byte, 2); 
 | would results : 
 | byte = 0 0 0 0  0 1 0 0;
 */
#define BINinvert_bit8(__byte__ , __index__)\
		if(BINget_bit8(__byte__ , __index__)) BINunset_bit8(__byte__, __index__);\
		else 								  BINset_bit8(__byte__ , __index__);
/*
 | Inverts all the bits in the byte
 */
#define BINinvert_all_bits8(__byte__) (__byte__) =  (~__byte__)
/*
 | Inverts all bits in range (__lower_b__, __upper_b__)
 | Example :  
 | uint8_t byte = 0x00;
 | BINinvert_bits_in_range8(byte, 4 , 6);
 | would results : 
 | byte = 0 1 1 1  0 0 0 0
 */
#define BINinvert_bits_in_range8(__byte__, __lower_b__, __upper_b__)\
		{\
			uint8_t mask = BINget_set_bit_mask_in_range8(__lower_b__, __upper_b__);\
			uint8_t filtered_bits = ((__byte__) & mask);\
			BINinvert_all_bits8(filtered_bits);\
			BINapply_bit_mask8(filtered_bits, mask);\
			BINdelete_bit_mask8(__byte__, mask);\
			(__byte__) |= filtered_bits;\
		}
/*
 | Copies __num_bits__ from source byte to destination byte
 | __dst_byte__ : the destination byte
 | __src_byte__ : the source byte
 | __num_bits__ : number of bits to copy
 | __dst_offset__ : how many bits to offset the copies bits from the right most in the destination byte
 | __src_offset__ : how many bits to offset the bits to be copied from the right most in the source byte
 | Example : 
 | uint8_t src_byte = 0x00;
 | BINset_bit(0); BINset_bit(1); BINset_bit(3); BINset_bit(5); // src_byte = 0 0 1 0  1 0 1 1
 | uint8_t dst_byte = 0x00;
 | BINcpy_bits8(dst_byte, src_byte, 3, 0 , 0);
 | would results : 
 | dst_byte = 0 0 0 0  0 0 1 1; 
 |
 | And :  
 | dst_byte = 0x00;
 | BINcpy_bits8(dst_byte, src_byte, 3, 0, 3); 
 | would results : 
 | dst_byte = 0 0 0 1  1 0 0 0;
 |
 | And : 
 | dst_byte = 0x00; 
 | BINcpy_bits8(dst_byte, src_byte, 3, 3, 3);
 | would results : 
 | dst_byte = 0 0 1 0  1 0 0 0
 */
#define BINcpy_bits8(__dst_byte__, __src_byte__ , __num_bits__ , __dst_offset__ , __src_offset__)\
		{\
			uint8_t cpy_bits = BINget_bits_in_range8(__src_byte__, __src_offset__, __src_offset__ + __num_bits__ - 1);\
			cpy_bits << __dst_offset__;\
			BINunset_bits_in_range8(__dst_byte__, __dst_offset__, __dst_offset__ + __num_bits__ - 1);\
			BINadd_bit_mask8(__dst_byte__, cpy_bits);\
		}
/*
 | Swaps the bits at indices '__bit1_index__' and '__bit2_index__'
 | Example : 
 | uint8_t byte = 0x00; BINset_bit(1); BINset_bit(0); BINset_bit(3); BINset_bit(7); // byte = 1 0 0 0  1 0 1 1 
 | BINswap_bits8(byte, 4, 7); 
 | would results : 
 | byte = 0 0 0 1  1 0 1 1
 | NOTE : __bit2_index__ must be greater than __bit1_index__
 */
#define BINswap_bits8(__byte__, __bit1_index__, __bit2_index__)\
		{\
			uint8_t hold_bit = BINget_bit8(__byte__, __bit1_index__);\
			BINunset_bit8(__byte__, __bit1_index__);\
			__byte__ |= (BINget_bit8(__byte__, __bit2_index__) >> ((__bit2_index__) - (__bit1_index__)));\
			BINunset_bit8(__byte__, __bit2_index__);\
			__byte__ |= (hold_bit << (__bit2_index__));\
		}
/*
 | Rotates left the bits in a bytes by __bit_count__
 | NOTE : it is different from shifting, it does destroy the overflowed bits , instead it rotates them
 | Example : 
 | uint8_t byte = 0xAF; // byte = 1 0 1 0  1 1 1 1
 | BINcircular_shift_left8(byte, 1); 
 | would results : 
 | byte = 0 1 0 1  1 1 1 1
 |
 | or
 | byte = 0xAF;
 | BINcircular_shift_left8(byte , 2);
 | would results : 
 | byte = 1 0 1 1  1 1 1 0
 */
#define BINcircular_shift_left8(__byte__, __bit_count__)\
		{\
			signed char bit_counter = 0;\
			uint8_t hold_bit;\
			while(bit_counter < __bit_count__)\
			{\
				++bit_counter;\
				hold_bit = (__byte__ & 0x80);\
				hold_bit >>= 7;\
				__byte__ <<= 1;\
				__byte__ |= hold_bit;\
			}\
		}
/*
 | Rotates left the bits in a bytes by __bit_count__
 | NOTE : it is different from shifting, it does destroy the overflowed bits , instead it rotates them
 | Example : 
 | uint8_t byte = 0xAF; // byte = 1 0 1 0  1 1 1 1
 | BINcircular_shift_right8(byte, 1); 
 | would results : 
 | byte = 1 1 0 1  0 1 1 1
 |
 | or
 | byte = 0xAF;
 | BINcircular_shift_left8(byte , 2);
 | would results : 
 | byte = 1 1 1 0  1 0 1 1
 */
#define BINcircular_shift_right8(__byte__ , __bit_count__)\
		{\
			signed char bit_counter = 0;\
			uint8_t hold_bit;\
			while(bit_counter < __bit_count__)\
			{\
				++bit_counter;\
				hold_bit = (__byte__ & 0x01);\
				hold_bit <<= 7;\
				__byte__ >>= 1;\
				__byte__ |= hold_bit;\
			}\
		}
/*
 | Rotates left the bits in range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__ by __bit_count__
 | Example : 
 | uint8_t byte = 0xAF ; //byte = 1 0 1 0  1 1 1 1
 | BINcircular_shift_left_in_range8(byte, 1, 2, 4);
 | would results : 
 | byte = 1 0 1 1  1 0 1 1
 */
#define BINcircular_shift_left_in_range8(__byte__, __bit_count__, __lower_b__, __upper_b__)\
		{\
			signed char bit_count  = (__bit_count__);\
			uint8_t filtered_bits;\
			uint8_t mask = BINget_set_bit_mask_in_range8(__lower_b__, __upper_b__);\
			uint8_t hold_bit;\
			while(bit_count)\
			{\
				filtered_bits = ((__byte__) & mask);\
				hold_bit = BINget_bit8(filtered_bits, __upper_b__);\
				hold_bit <<= (__lower_b__);\
				filtered_bits >>= 1;\
				filtered_bits |= hold_bit;\
				BINapply_bit_mask8(filtered_bits, mask);\
				BINdelete_bit_mask8(__byte__, mask);\
				(__byte__) |= filtered_bits;\
				--bit_count;\
			}\
		}
/*
 | Rotates right the bits in range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__ by __bit_count__
 | Example : 
 | uint8_t byte = 0xAF ; //byte = 1 0 1 0  1 1 1 1
 | BINcircular_shift_right_in_range8(byte, 1, 2, 4);
 | would results : 
 | byte = 1 0 1 1  0 1 1 1
 */
#define BINcircular_shift_right_in_range8(__byte__, __bit_count__, __lower_b__, __upper_b__)\
		{\
			signed char bit_count  = (__bit_count__);\
			uint8_t filtered_bits;\
			uint8_t mask = BINget_set_bit_mask_in_range8(__lower_b__, __upper_b__);\
			uint8_t hold_bit;\
			while(bit_count)\
			{\
				filtered_bits = ((__byte__) & mask);\
				hold_bit = BINget_bit8(filtered_bits, __lower_b__);\
				hold_bit <<= (__upper_b__);\
				filtered_bits >>= 1;\
				filtered_bits |= hold_bit;\
				BINapply_bit_mask8(filtered_bits, mask);\
				BINdelete_bit_mask8(__byte__, mask);\
				__byte__ |= filtered_bits;\
				--bit_count;\
			}\
		}
/*
 | Reverses all the bits in the __byte__
 | Example : 
 | uint8_t byte = 0xAF;			// byte = 1 0 1 0  1 1 1 1
 | BINreverse_bits8(byte);
 | would results : 
 | byte = 1 1 1 1  0 1 0 1
 */
#define BINreverse_bits8(__byte__)\
		{\
			uint8_t bit_counter = 0;\
			uint8_t hold_bit;\
			while(bit_counter < 4)\
			{\
				hold_bit = BINget_bit8(__byte__, 7 - bit_counter);\
				if(BINget_bit8(__byte__ , bit_counter)) BINset_bit8(__byte__ , 7 - bit_counter);\
				else BINunset_bit8(__byte__ , 7 - bit_counter);\
				if(hold_bit) BINset_bit8(__byte__ , bit_counter);\
				else BINunset_bit8(__byte__, bit_counter);\
				++bit_counter;\
			}\
		}
/*
 | Reverses all the bits in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__
 | Example : 
 | uint8_t byte = 0xAF; //byte =  1 0 1 0  1 1 1 1
 | BINreverse_bits_in_range8(byte, 3, 6);
 | would results : 
 | byte = 1 1 0 1  0 1 1 1
 */
#define BINreverse_bits_in_range8(__byte__ , __lower_b__ , __upper_b__)\
		{\
			uint8_t hold_bit;\
			uint8_t bit_count = ((__upper_b__) - (__lower_b__));\
			++bit_count;\
			bit_count >>= 1;\
			while(bit_count)\
			{\
				--bit_count;\
				hold_bit = BINget_bit8(__byte__ , (__upper_b__) - bit_count);\
				BINunset_bit8(__byte__, __upper_b__ - bit_count);\
				__byte__ |= (BINget_bit8(__byte__ , (__lower_b__) + bit_count) << ((__upper_b__) - bit_count));\
				BINunset_bit8(__byte__, __lower_b__ + bit_count);\
				__byte__ |= (hold_bit << ((__lower_b__) + bit_count));\
			}\
		}
/*
 | Shifts left all the bits in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__
 | Example : 
 | uint8_t byte = 0xAF; //byte =  1 0 1 0  1 1 1 1
 | BINshift_bits_left_in_range8(byte, 3, 6);
 | would results : 
 | byte = 1 1 0 1  0 1 1 1
 */
#define BINshift_bits_left_in_range8(__byte__, __bit_count__, __lower_b__, __upper_b__)\
		{\
			uint8_t mask = BINget_set_bit_mask_in_range8(__lower_b__, __upper_b__);\
			uint8_t filtered_bits = (mask & __byte__);\
			filtered_bits <<= (__bit_count__);\
			BINapply_bit_mask8(filtered_bits, mask);\
			BINdelete_bit_mask8(__byte__, mask);\
			__byte__ |= filtered_bits;\
		}
/*
 | Shifts right all the bits in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__
 | Example : 
 | uint8_t byte = 0xAF; //byte =  1 0 1 0  1 1 1 1
 | BINshift_bits_right_in_range8(byte, 3, 6);
 | would results : 
 | byte = 1 0 0 1  0 1 1 1
 */
#define BINshift_bits_right_in_range8(__byte__ ,__bit_count__, __lower_b__, __upper_b__)\
		{\
			uint8_t mask = BINget_set_bit_mask_in_range8(__lower_b__, __upper_b__);\
			uint8_t filtered_bits = (mask & __byte__);\
			filtered_bits >>= (__bit_count__);\
			BINapply_bit_mask8(filtered_bits, mask);\
			BINdelete_bit_mask8(__byte__, mask);\
			__byte__ |= filtered_bits;\
		}
/*
 | BINlayout : pointer to the array of unsigned char
 | It represents the layout of the bytes pointed in the BINARY object 's internal pointer
 | 
 | for example : 
 | if you have struct like this : 
 | struct data
 | {
 |		uint32_t i32value; // allocates 8 bytes
 |		uint64_5 i64value; // allocates 8 bytes
 |		uint8_t  i8value;  // allocates 8 bytes
 | };
 | and in some function
 |
 | BINARY* serialized_data = BINserialize(&data, sizeof(struct data));
 |
 | then the Binary layout for this struct will be : 
 | BINlayout data_layout = {8 , 8 , 8};  
 |
 | Usage : It is used in reversing the whole bytes with following a binary layout, useful for converting from big endian to little endian or vice-versa
 */
typedef uint8_t BINlayout[];
/*
 | struct __BINARY is abstracted by BINARY typedefinintion for internal data protection reasons
 | NOTE : sizeof(struct __BINARY) equals BIN_BINARY_OBJECT_SIZE define at the top of this header file
 */
typedef struct __BINARY BINARY;

#ifdef __cplusplus
extern "C" { 
#endif

/* public setter for internal byte pointer of binded binary object */
void BINset_bytes(uint8_t * bytes); 
/* public setter for internal byte count storage variable of binded binary object */
void BINset_byte_count(uint32_t count); 
/* public setter for internal multi byte representation of binded binary objecty */
void BINset_endianess(uint8_t endianess);
/* public getter for internal byte pointer of binded binary object */
uint8_t* BINget_bytes();
/* public getter for internal byte count storage variable of binded binary object */
uint32_t BINget_byte_count();
/* public getter for internal mult byte representation of binded binary object */
uint8_t BINget_endianess(); 
/*
 | out_string_buffer : this characterr buffer must be larger than 12 bytes
 | __byte__ : the byte to convert to string representation
 | example : 
 |
 | unit8_t byte = 0xAF;
 | char string_buffer[12];
 | BINget_byte_stringb(byte, string_buffer);
 | fputs(string_buffer, stdout); 
 | would output : 
 | [10101111] inclusive of newline character
 |
 | NOTE : 1. It outputs the lower order bit on the right most 
 |        2. structure of out_string_buffer : 
 |				out_string_buffer[0]  = '['
 | 				out_string_buffer[1]  = bit at index 7
 | 				out_string_buffer[2]  = bit at index 6
 | 				out_string_buffer[3]  = bit at index 5
 | 				out_string_buffer[4]  = bit at index 4
 | 				out_string_buffer[5]  = bit at index 3
 | 				out_string_buffer[6]  = bit at index 2
 | 				out_string_buffer[7]  = bit at index 1
 | 				out_string_buffer[8]  = bit at index 0
 | 				out_string_buffer[9]  = ']'
 |				out_string_buffer[10] = '\n'
 | 				out_string_buffer[11] = '\0'
 */
void BINget_byte_stringb(uint8_t __byte__, char* out_string_buffer);
/*
 | Reverses the byte order of the bytes pointed by __bytes__ pointer
 | __bytes__ : pointer to the array of bytes
 | count 	 : number of bytes 
 |
 | Example : 
 | char bytes[8];
 | bytes[0] = 0x00;
 | bytes[1] = 0xAF;
 | bytes[2] = 0xBF;
 | bytes[3] = 0xAB;
 | bytes[4] = 0x01;
 | bytes[5] = 0x0f;
 | bytes[6] = 0xff;
 | bytes[7] = 0x55;
 | BINARY* binary = BINserialize(bytes, sizeof(bytes), BIN_LITTLE_ENDIAN);
 | BINbind(binary);
 | BUFFER* string_buffer = BINcreate_string_buffer();
 | 
 | puts("Before reversal")
 | BINget_bytes_string(string_buffer);
 | BUFbind(string_buffer);
 | fputs(BUFget_ptr(), stdout); 
 | puts("");
 | 
 | BINreverse_byte_orderb(BINget_bytes(), BINget_byte_count()); //reverses all the bytes
 | // or BINreverse_byte_orderb(bytes, sizeof(bytes)); 
 |
 | puts("After reversal");
 | BINget_bytes_string(string_buffer);
 | BUFbind(string_buffer);
 | fputs(BUFget_ptr(), stdout); 
 |
 |
 | output : 
 | Before reversal
 | [0000 0000]
 | [1010 1111]
 | [1011 1111]
 | [1010 1011]
 | [0000 0001]
 | [0000 1111]
 | [1111 1111]
 | [0101 0101]
 |
 | After reversal
 | [0101 0101]
 | [1111 1111]
 | [0000 1111]
 | [0000 0001]
 | [1010 1011]
 | [1011 1111]
 | [1010 1111]
 | [0000 0000]
 */
void BINreverse_byte_orderb(uint8_t* __bytes__ , uint32_t count); 
/*
 | Reverses bits in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__ in bytes
 | Not yet Implemented
 */
void BINreverse_bit_orderb(uint8_t* __bytes__, uint32_t count, uint32_t __lower_b__, uint32_t __upper_b__);
/*
 | Reverses all the bytes in pointed by the BINARY object's internal pointer with specified LAYOUT
 |
 | Example : 
 | struct Data
 | {
 |		uint8_t byte1;						//pushed on the previous 8 bytes allocated
 | 		uint8_t byte2;						//allocates 8 bytes
 |		uint32_t byte3; 					//allocates 8 bytes
 | };
 |
 | main()
 | {
 |		struct Data data = { 1, 1, 4 };
 |		BINARY* serialized_data = BINserialize(&data, sizeof(struct Data));
 |		BINbind(serialized_data);
 |		
 |		BINlayout data_layout = { 8 , 8 };		
 |		BINreverse_byte_order(&data_layout, sizeof(data_layout)); 
 |		BINfree(); 
 | }
 |
 | Explaination : First it reverses the order of the first 8 bytes and it reversed the second 8 bytes
 |
 | NOTE : if you want to reverse all the bytes then do this : 
 |	BINreverse_byte_order(NULL, 0); 			//the second argument actually ignored if the first argument is NULL
 */
void BINreverse_byte_order(BINlayout* __layout__ , uint32_t count);
/*
 | Not implementd yet
 */
void BINreverse_bit_order(BINlayout* __layout__ , uint32_t count);  /*NOTE IMPLEMENTED*/
/*
 | Converts the binded_binary to big endian representation if it was little endian as specified in the functions BINserialize or BINcreate
 | You must pass the binary layout '__layout__' to accomplish it.
 */
void BINbig_endian(BINlayout* __layout__ , uint32_t count);
/*
 | Converts the binded_binary to little endian representation if it was big endian as specified in the functions BINserialize or BINcreate
 | You must pass the binary layout '__layout__' to accomplish it.
 */
void BINlittle_endian(BINlayout* __layout__ , uint32_t count); 
/*
 | Sets the bits lying in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__
 | Example : 
 |
 | BINARY* binary = BINcreate(4 , BIN_LITTLE_ENDIAN);
 | BINbind(binary);
 | BINunset_all_bits();
 | BINset_bits_in_range(0, 23);  
 | BINfree(); 
 */
void BINset_bits_in_range(uint32_t __lower_b__ , uint32_t __upper_b__); 
/*
 | UnSets the bits lying in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__
 | Example : 
 |
 | BINARY* binary = BINcreate(4 , BIN_LITTLE_ENDIAN);
 | BINbind(binary);
 | BINset_all_bits();
 | BINunset_bits_in_range(0, 23);  
 | BINfree(); 
 */
void BINunset_bits_in_range(uint32_t __lower_b__ , uint32_t __upper_b__); 
/*Must be Excluded from the this header file and the corresponding source file*/
void BINprint_info();
/*
 | When you call any of the functions 
 | BINserialize
 | BINcreate
 | You must to pair them with this BINfree() function to free up all the resources
 */
void BINfree();

/*
 | It serializes an object to bytes and BINARY's internal pointer points to the serialized bytes
 |
 | __binary__ : the binary object in which all the serialized meta data would be filled,
 | 			if this is NULL then BINserialize() function allocates memory for new BINARY object in the heap then returns the pointer to it
 | 			otherwise it returns the passed __binary__ pointer
 | __object__ : pointer to the object that is to be serialized
 | __size__   : size of the object in bytes
 | endianess  : endianess of the serialized bytes
 | 
 | if __binary__ is NULL then it does allocate heap memory of BIN_BINARY_OBJECT_SIZE bytes
 | but in any case, it doesn't allocate extra memory for the serialization.
 |
 | Warning : If you manipulate the serialized binary then it will reflect changes in the object that is serialized
 */
BINARY* BINserialize(BINARY* __binary__, void* __object__, uint32_t __size__, uint8_t endianess);
/*
 | __binary__ : the binary object in which all the meta data of allocated bytes must be filled,
 |			 if this is NULL then BINcreate() function alloctes memory for new BINARY object in the heap then returns the pointer to it
 | 			 otherwise it returns the passed __binary__ pointer
 | nbytes 	  : Number of bytes to allocate
 | endianess  : endianess of the created bytes
 |
 | if __binary__ is NULL then it allocates memory for the BINARY object otherwise not
 | but in any case, it will allocate  memory for specified number of bytes
 */
BINARY* BINcreate(BINARY* __binary__, uint32_t nbytes, uint8_t endianess);
/*
 | Creates a BINARY object 
 | It doesn't allocate any memory but it takes the memory from the stack, for performance reasons
 | 
 | Example :
 | 
 | char bytes[BIN_BINARY_OBJECT_SIZE]; 
 |
 | BINARY* binary = BINcreate_object(bytes);
 | //you don't have to free this binary since it is allocated in the stack
 */
BINARY* BINcreate_object(uint8_t* bytes); 
/*
 | Creates a String buffer for the visual representation of the bytes in the binded binary
 | 
 |  IF buffer == NULL then, 
 |  Internally it allocates enough memory for storing the visual character corresponding to the bits
 | 	Size of the allocated buffer  : (BUF_BUFFER_OBJECT_SIZE + 11 * BINget_byte_count() + 1) bytes
 |  ELSE
 |  Internally it allocates enough memory for storing the visual character corresponding to the bits
 |  Size of the allocated buffer : (11 * BINget_byte_count() + 1) bytes 
 | 
 | Note that in the second case BUF_BUFFER_OBJECT_SIZE is not allocated
 | 
 | To avoid the extra allocation of BUF_BUFFER_OBJECT_SIZE bytes in the heap
 | Use this : 
 | char bytes[BUF_BUFFER_OBJECT_SIZE];
 | BUFFER* buffer = BUFcreate_object(bytes);
 |
 | The following example allocates (BUF_BUFFER_OBJECT_SIZE + 11 * BINget_byte_count() + 1) bytes : 
 |
 | char bytes[BIN_BINARY_OBJECT_SIZE];
 | BINARY* binary = BINcreate_object(bytes);
 | BINbind(binary);
 | BINunset_all_bits();
 | BINset_bit(0); BINset_bit(2); BINset_bit(4); BINset_bit(7);
 |
 | BUFFER* string_buffer = BINcreate_string_buffer(NULL);
 |
 | BINget_bytes_string(string_buffer);
 | BUFbind(string_buffer);
 | fputs(BUFget_ptr(), stdout); 
 | BUFfree(); 				//free up the created string_buffer
 |
 | In contrast to the above example, below allocates (11 * BINget_byte_count() + 1) bytes only
 |
 | char buf_bytes[BUF_BUFFER_OBJECT_SIZE];
 | char bin_bytes[BIN_BUFFER_OBJECT_SIZE];
 | BINARY* binary = BINcreate_object(bin_bytes);
 | BUFFER* buffer = BUFcreate_object(buf_bytes);
 | BINcreate(binary, 2 , BIN_LITTLE_ENDIAN); 
 | BINbind(binary);
 | BUFbind(buffer);
 | BINunset_all_bits();
 | BINset_bit(0); BINset_bit(2); BINset_bit(4); BINset_bit(7);
 | BINcreate_string_buffer(buffer); 
 | BINget_bytes_string(buffer);
 | fputs(BUFget_trp(), stdout); 
 | BUFfree();
 | BINfree();
 */
BUFFER* BINcreate_string_buffer(BUFFER* buffer); 
/*
 | Unsets all the bits in the binded binary object
 | Useful when you have created a new binary object and allocated some bytes for it, contains garbage initially
 */
void BINunset_all_bits();
/*
 | Sets all the bits in the binded binary object
 */
void BINset_all_bits();
/*
 | Unsets a particular bit at an index 'index' in binded binary object
 | NOTE : In the top most byte, the right most bit will hold the index of 0 like following : 
 | 						Indices:39  38 37 36 35 34 33 32  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17  16 15 14 13 12 11 10  9  8  7   6  5  4  3  2  1  0
 | binary's internal pointer -> [0  0  0  1  0  1  0  0] [0  0  0  1  0  1  0  0] [0  0  0  1  0  1  0  0] [0  0  0  1  0  1  0  0] [0  0  0  1  0  1  0  0]
 | 								lower most byte 																					 top most byte
 */
void BINunset_bit(uint32_t index);
/*
 | Sets a particular bit at an index 'index' in bined binary
 */
void BINset_bit(uint32_t index);
/*
 | Returns 0x01 or 0x00 whether the bit at the index 'index' is set or unset repectively in binded binary object
 */
uint8_t BINget_bit(uint32_t index);
/*
 | Fills the out_buffer with the visual characters for each bit whether they are set or unset in binded binary object
 | NOTE :  Before calling this function ensure that you have called BINcreate_string_buffer() function
 */
void BINget_bytes_string(BUFFER* out_buffer);

/*
 | Binds the BINARY object 'binary' to the current working context
 */
void BINbind(BINARY* binary);
/*
 | Unbinds the current binded 'binary '
 */
void BINunbind();
/*
 | Inverts a perticular bit at the index 'index'
 */
void BINinvert_bit(uint32_t index); 
/*
 | Inverts the bits lying in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__ of binded binary object
 */
void BINinvert_bits_in_range(uint32_t __lower_b__ , uint32_t __upper_b__);
/*
 | Inverts all the bits in the binded binary object
 */
void BINinvert_all_bits();  
/*
 | Applies the OR operation in each of the bits of binded binary with the '__binary__' and the stores the result in the binded binary object
 */
void BINor(BINARY* __binary__);
/*
 | Applies the AND operation in each of the bits of binded binary with the '__binary__' and the stores the result in the binded binary object
 */
void BINand(BINARY* __binary__);
/*
 | Applies the XOR operation in each of the bits of binded binary with the '__binary__' and the stores the result in the binded binary object
 */
void BINxor(BINARY* __binary__);

/*
 | Shifts the bits lying in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__ by bit_count to the right 
 */
void BINshift_bits_right_in_range(uint32_t bit_count , uint32_t __lower_b__ , uint32_t __upper_b__);
/*
 | Shifts the bits lying in the range (__lower_b__, __upper_b__) inclusive of __lower_b__ and __upper_b__ by bit_count to the left 
 */
void BINshift_bits_left_in_range(uint32_t bit_count , uint32_t __lower_b__ , uint32_t __upper_b__);

#ifdef __cplusplus
}
#endif
#endif
