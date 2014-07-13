/* 
    File: my_allocator.c

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include "my_allocator.h"
#include <math.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    typedef struct header{
       int MagicNumber;			//initial check to ensure correct address
       int  is_free;            // 0 = free, 1 = not free
       int binary_Index;        // 2^binary_index = size
       int side;                // 0 = left; 1 = right; NULL = none
       int Inheritance;			// 0 = left inheritane, 1 = right, NULL = none
       int size;                // size of the block 
       struct header* next;		// points to next block
       struct header* prev;		// points to previous block
} ;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

  int BlockNum = 0;
  int index = 0;
  void** freeList;
  int totalLength;
  void * totalMemory;
  int blocksize;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/


/*void MergeBlocks ( HDR * _h){
  if( _h -> MagicNumber == 3028)         // Check magic number
    if (_h -> is_free == 0)              // if it's free

      if (_h -> side == 0)               // if it's a left block
        if ((_h+_h->size)->MagicNumber == 3028)
          if ((_h+_h->size)->is_free == 0){       // if it's buddy is free
            HDR * _h1 = (HDR *)(_h+_h->size);
            _h -> size = _h->size + (_h1)->size;   // add sizes of buddies
            // add a way to check if top block is reached. If yes then set
            // set side = 2; Inheritance = 2; 
            _h -> side = _h -> Inheritance; 
            _h -> Inheritance = (_h1)-> Inheritance;
            _h -> fibIndex = _h -> fibIndex + 1;
            

  // remove _h from free list 

            if (_h -> prev == NULL && _h -> next == NULL){    // first of list
                freeList[_h -> fibIndex -1] = NULL;
               // freeList[_h -> fibIndex] = NULL;
            }
            else if (_h -> prev == NULL && _h -> next != NULL){
                freeList[_h -> fibIndex -1] = _h -> next;
                _h -> next -> prev = NULL;
                _h -> next = NULL;
            }
            else if(_h -> prev != NULL && _h -> next == NULL){
              _h -> prev -> next == NULL;
              _h -> prev = NULL;
            }
            else if (_h -> prev != NULL && _h -> next != NULL){
              _h -> prev -> next = _h -> next;
              _h -> next -> prev = _h -> prev;
              _h -> prev = NULL;
              _h -> next = NULL;
            }
            else {}

  // remove (_h1) from free list

            if ((_h1) -> prev == NULL && (_h1) -> next == NULL){    // first of list
                freeList[(_h1) -> fibIndex] = NULL;
                //freeList[(_h1) -> fibIndex] = NULL;
            }
            else if ((_h1) -> prev == NULL && (_h1) -> next != NULL){
                freeList[(_h1) -> fibIndex] = (_h1) -> next;
                (_h1) -> next -> prev = NULL;
                (_h1) -> next = NULL;
            }
            else if((_h1) -> prev != NULL && (_h1) -> next == NULL){
              (_h1) -> prev -> next == NULL;
              (_h1) -> prev = NULL;
            }
            else if ((_h1) -> prev != NULL && (_h1) -> next != NULL){
              (_h1) -> prev -> next = (_h1) -> next;
              (_h1) -> next -> prev = (_h1) -> prev;
              (_h1) -> prev = NULL;
              (_h1) -> next = NULL;
            }
            else {}

  // add _h to proper free list 

            if (freeList[_h -> fibIndex] == NULL)
              freeList[_h -> fibIndex] = _h;
            else{
              freeList[_h -> fibIndex] -> prev = _h;           
              _h -> next = freeList[_h -> fibIndex];
              freeList[_h -> fibIndex] = _h; 
            }

          MergeBlocks(_h);     
          }


     else if (_h -> side == 1)
      {
        int i = (_h-> size / fib(_h -> fibIndex));    // represents block size 
        i = i* fib((_h->fibIndex)+1);                 // represents size of left block
        HDR * _h1 = (HDR *)(_h - i); 

        if ((_h1)->MagicNumber == 3028)
          if ((_h1)->is_free == 0){       // if it's buddy is free
            _h1 ->size = _h->size + (_h1)->size;   // add sizes of buddies
            _h1 -> side = _h1 -> Inheritance; 
            _h1 -> Inheritance = _h-> Inheritance;
            _h1 ->  fibIndex = _h -> fibIndex + 2;
            
      
  // remove _h from free list 

            if (_h -> prev == NULL && _h -> next == NULL){    // first of list
                freeList[_h -> fibIndex] = NULL;
               // freeList[_h -> fibIndex] = NULL;
            }
            else if (_h -> prev == NULL && _h -> next != NULL){
                freeList[_h -> fibIndex] = _h -> next;
                _h -> next -> prev = NULL;
                _h -> next = NULL;
            }
            else if(_h -> prev != NULL && _h -> next == NULL){
              _h -> prev -> next == NULL;
              _h -> prev = NULL;
            }
            else if (_h -> prev != NULL && _h -> next != NULL){
              _h -> prev -> next = _h -> next;
              _h -> next -> prev = _h -> prev;
              _h -> prev = NULL;
              _h -> next = NULL;
            }
            else {}

  // remove (_h1) from free list

            if ((_h1) -> prev == NULL && (_h1) -> next == NULL){    // first of list
                freeList[(_h1) -> fibIndex-1] = NULL;
                //freeList[(_h1) -> fibIndex] = NULL;
            }
            else if ((_h1) -> prev == NULL && (_h1) -> next != NULL){
                freeList[(_h1) -> fibIndex-1] = (_h1) -> next;
                (_h1) -> next -> prev = NULL;
                (_h1) -> next = NULL;
            }
            else if((_h1) -> prev != NULL && (_h1) -> next == NULL){
              (_h1) -> prev -> next == NULL;
              (_h1) -> prev = NULL;
            }
            else if ((_h1) -> prev != NULL && (_h1) -> next != NULL){
              (_h1) -> prev -> next = (_h1) -> next;
              (_h1) -> next -> prev = (_h1) -> prev;
              (_h1) -> prev = NULL;
              (_h1) -> next = NULL;
            }
            else {}

  // add _h1 to proper free list 

            if (freeList[_h1 -> fibIndex] == NULL)
              freeList[_h1 -> fibIndex] = _h1;
            else{
              freeList[_h1 -> fibIndex] -> prev = _h1;           
              _h1 -> next = freeList[_h1 -> fibIndex];
              freeList[_h1 -> fibIndex] = _h1; 
            }

          MergeBlocks(_h1);     
          }
      }
     else {}
}

void SplitBlocks ( HDR * _h){
  HDR * _hR = _h + fib(_h -> fibIndex -1) * blocksize; 

       if( _h -> MagicNumber == 3028)         // Check magic number
        if (_h -> is_free == 0){            // Check if free
    // set right block values 
          _hR -> size = blocksize * fib(_h fibIndex -2);
          _hR -> fibIndex = _h -> fibIndex -2;
          _hR -> Inheritance = _h -> Inheritance;
          _hR -> side = 1;
    // set left block
          _h -> fibIndex = _h -> fibIndex -1;
          _h -> size = fib(_h -> fibIndex-1)*blocksize;
          _h -> Inheritance = _h -> side;
          _h -> side = 0;
          //change _h free list location
         if (_h -> prev == NULL && _h -> next == NULL){    // first of list
                freeList[_h -> fibIndex +1] = NULL;
            }
            else if(_h -> prev != NULL && _h -> next == NULL){
              _h -> prev -> next == NULL;
              _h -> prev = NULL;
            }
            else if (_h -> prev == NULL && _h -> next != NULL){
                freeList[_h -> fibIndex +1] = _h -> next;
                _h -> next -> prev = NULL;
                _h -> next = NULL;
            }
            else if (_h -> prev != NULL && _h -> next != NULL){
              _h -> prev -> next = _h -> next;
              _h -> next -> prev = _h -> prev;
              _h -> prev = NULL;
              _h -> next = NULL;
            }
            else {}
          // add to new free list 
              if (freeList[_h -> fibIndex] == NULL)
              freeList[_h -> fibIndex] = _h;
            else{
              freeList[_h -> fibIndex] -> prev = _h;           
              _h -> next = freeList[_h -> fibIndex];
              freeList[_h -> fibIndex] = _h; 
            }
    // free up left block from memory 
            if (_hR -> prev == NULL && _hR -> next == NULL){    // first of list
                freeList[_hR -> fibIndex +1] = NULL;
            }
            else if(_hR -> prev != NULL && _hR -> next == NULL){
              _hR -> prev -> next == NULL;
              _hR -> prev = NULL;
            }
            else if (_hR -> prev == NULL && _hR -> next != NULL){
                freeList[_hR -> fibIndex +1] = _hR -> next;
                _hR -> next -> prev = NULL;
                _hR -> next = NULL;
            }
            else if (_hR -> prev != NULL && _hR -> next != NULL){
              _hR -> prev -> next = _hR -> next;
              _hR -> next -> prev = _hR -> prev;
              _hR -> prev = NULL;
              _hR -> next = NULL;
            }
            else {}
  // add to new free list 
              if (freeList[_hR -> fibIndex] == NULL)
              freeList[_hR -> fibIndex] = _hR;
            else{
              freeList[_hR -> fibIndex] -> prev = _hR;           
              _hR -> next = freeList[_hR -> fibIndex];
              freeList[_hR -> fibIndex] = _h; 
            }
        }
}
*/
int release_allocator(){
  free(freeList);
  free(totalMemory);
  return 0;
}


extern unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length){ 
  blocksize = _basic_block_size;
  if ((double)(_length/_basic_block_size) < 1)
    return 1;
  else{
    while (BlockNum < _length/_basic_block_size){
      index++;
      BlockNum = pow(2.0,index);
    }
  }
// creates free list of "index" spaces 
  *freeList = malloc(index * sizeof(Addr));
// allocate total amount of space
  totalMemory = malloc(BlockNum * _basic_block_size);
// create HDR for total memory 
  header * a = (header*)totalMemory;
  a-> MagicNumber = 3028;
  a-> is_free = 0;				
  a-> binary_Index = index;		
  a-> side = 2;					
  a-> Inheritance = 2;			
  a-> size = BlockNum * _basic_block_size;
  a-> next = NULL;
  a-> prev = NULL;

  freeList[index] = a;  
  
  totalLength = BlockNum * _basic_block_size;
  return totalLength;

}



/* Don't forget to implement "init_allocator" and "release_allocator"! */


extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  
 HDR * a = NULL;
  int i = _length/blocksize;    // number of blocks
  int indexx = 0;
  while (fib(indexx) < _length)   // find the block size that is closest to the num of blocks requested. 
    indexx++;
  int j;
  for (j = indexx; j < index; j++){     // find the available free block
    if (freeList[indexx] != NULL){
                                        // remove from free list 
      a = (HDR * )freeList[indexx];
      a -> is_free = 1;
      a -> next -> prev = NULL;
      freeList[indexx] = a-> next;
      a -> next = NULL;
      break;

    }
  }
  if (a == NULL)
    return 0;
  else
    return a + sizeof(a);                // give pointer that is past header. 
}*/
}
extern int my_free(Addr _a) {
  /* Same here! */
  free(_a);
  return 0;
}


