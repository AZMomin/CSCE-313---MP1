/* 
    File: my_allocator.h

    Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

*/

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr; 

typedef struct header{
      int MagicNumber;      //initial check to ensure correct address
      int  is_free;            // 0 = free, 1 = not free
      int binary_Index;        // 2^binary_index = size
      int side;                // left = 0; right = 1; no side = 2
    int inheritance;    // left block holds parent's side; right block holds parent's inheritance; same as size
      int size;                // size of the block 
      struct header* next;    // points to next block
      struct header* prev;    // points to previous block
} HDR;

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* MODULE   MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length); 
/* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. 
*/ 
void _remove(HDR * _h);
/* This function removes a header from the freeList
*/
void SplitBlocks ( HDR * _h);
/* This function splits a block into two children blocks
*/
void MergeBlocks ( HDR * _h);
/* This function finds a blocks buddy and attempts to merge 
*/
int release_allocator(); 
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails.
*/ 
void check_list();
/* This function calls an assortment of splits and merges - purely for testing purposes
*/
void print_freeList();
/* This function prints the freeList based on whether or not it has a value in it
*/
Addr my_malloc(unsigned int _length); 
/* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. */ 

int my_free(Addr _a); 
/* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 

#endif 
