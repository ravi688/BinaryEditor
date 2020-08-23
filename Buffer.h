#ifndef _BUFFER_H_
#define _BUFFER_H_

#define BUF_8BIT   /*1 byte*/
#define BUF_16BIT  /*2 byte*/
#define BUF_32BIT  /*4 byte*/
#define BUF_64BIT  /*8 byte*/
#define BUF_128BIT /*16 byte*/

#define BUF_BUFFER_OBJECT_SIZE 24 /*bytes*/

#include <stdint.h>

typedef struct __BUFFER BUFFER;

#ifdef __cplusplus
extern "C" { 
#endif

void BUFreverseb(void* ptr_to_buffer , uint32_t element_size, uint32_t element_count); 

/* Binds the buffer to the binded buffer*/
void BUFbind(BUFFER* buffer) ; 
/* Unbinds the current binded buffer*/
void BUFunbind();  
/* Logs the details of buffer*/
void BUFlog();
/* Frees all the memory blocks allocated in the buffer as well as buffer object*/
void BUFfree();

BUFFER* BUFcreate_object(uint8_t* bytes); 
/* Creates a new buffer , allocates buffer object and with initial element capacity of 'capacity' and each element would have size 'element size' in bytes*/
BUFFER* BUFcreate(BUFFER* __buffer, uint32_t element_size, uint32_t capacity, uint32_t offset);
/* Fills the out_value memory block with the content at index 'index'*/
void BUFget_at(uint32_t index , void* out_value);
/* Fills the memory block referenced by index with value 'in_value' having size buffer's element size in bytes*/
void BUFset_at(uint32_t index , void* in_value) ;  
/* When buffer becomes too large, since the buffer doubles the element capacity when it i s about to overflow, 
 * When you have done , and you have anticepted that the buffer will not increase further then you can fit the buffer and
 * there will be no free space in the buffer for new element*/
void BUFfit();
/* Fills out_value with the top element but doesn't remove it*/
void BUFpeek(void* out_value);  
/* Fills the out_value memeory block with the top element in the buffer with size of binded buffer's element size in bytes*/
void BUFpop(void* out_value); 
/* Pushes the in_value with size of bined buffer's element size in bytes on the top of the buffer 
 * When the buffer is about to overflow then it doubles it element capacity*/
void BUFpush(void* in_value); 
/* Fills all the memory blocks having size of binded buffer 's element size in bytes with clear_value */
void BUFclear(void* clear_value) ; 
/* Removes an element with size binded buffer's element size, (actually it sets each byte in the memory block referenced by index to zero)
 * out_value : filled with the removed value
 * index  : the index at which it is requested to remove the element
 * NOTE : There will be no shifting of elements , that means  you can reinsert some value again in that slot without any furthur shifting of elements
 *        Use this if you now that you are removing an element at index 'index' and you would reinsert an element at that index 'index' again.
 * Important note : There will be no effect on element count in the bined buffer
 */ 
void BUFremove_at_noshift(uint32_t index , void* out_value) ;  
/* Removes an element with size binded buffers' e lemetn size , (actually it sets each byte int the top most memory block referenced by index to zero)
 * and shifts all the elements  succeeding the block referenced by index to left by one memory block
 * index : at the element to be removed
 * out_value : Filled with the removed value
 */
void BUFremove_at(uint32_t index , void* out_value) ;  
/* Inserts an element of size binded buffer's element size at the index 'index'
 * index : at which the value must be inserted
 * in_vaue : pointer to the value
 * NOTE : index must be less than element_count
   And it will alloc extra memory of size element_size if the there will be no space left in the buffer
   And it will shift all the elements beyound the index to right by one element
 */
void BUFinsert_at(uint32_t index , void* in_value) ;  
/* Inserts an element of size binded buffer's element size at the index 'index'
 * index : at which the value must be inserted
 * in_value : pointer to the value
 * out_value : filled with the existing element at index 'index'
 * NOTE : index must be less than element_size
   And actually it simply replaces the value at index with the value in_value
 */
void BUFinsert_at_noalloc(uint32_t index , void* in_value , void* out_value) ; 

/* Returns the max count of elements that the buffer can contain*/
uint32_t BUFget_capacity();  
/* Returns the Current element count in the buffer*/
uint32_t BUFget_element_count(); 
/* Returns the element size of the elements residing in the buffer*/
uint32_t BUFget_element_size(); 
/* Returns the unsigned char pointer to the buffer*/
uint8_t* BUFget_ptr(); 

/* Sets the max count of elements that the buffer can contain*/
void BUFset_capacity(uint32_t capacity);  
/* Sets the Current element count in the buffer*/
void BUFset_element_count(uint32_t element_count); 
/* Sets the element size of the elements residing in the buffer*/
void BUFset_element_size(uint32_t element_size); 
/* Sets the unsigned char pointer to the buffer*/
void BUFset_ptr(uint8_t* ptr); 
#ifdef __cplusplus
}
#endif

#endif