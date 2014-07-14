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
#include <stdio.h>
#include "my_allocator.h"
#include <math.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    typedef struct header{
      int MagicNumber;			//initial check to ensure correct address
      int  is_free;            // 0 = free, 1 = not free
      int binary_Index;        // 2^binary_index = size
      int side;                // left = 0; right = 1; no side = 2
	  int inheritance;		// left block holds parent's side; right block holds parent's inheritance; same as size
      int size;                // size of the block 
      struct header* next;		// points to next block
      struct header* prev;		// points to previous block
} HDR;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

  int BlockNum = 0;
  int index = 0;
  HDR** freeList;
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


//*******************************************************
//            REMOVE FROM FREELIST
//*******************************************************
void _remove(HDR * _h){
  if(_h -> prev == NULL && _h -> next == NULL){          // only element in list
           printf("Remove: Only element in List\n");
          freeList[_h ->binary_Index] = NULL;
        }
  if(_h -> prev == NULL && _h -> next != NULL){         // first element in list
          printf("%s\n", "Remove: First in List");
          freeList[_h ->binary_Index] = _h -> next;
          _h -> next -> prev = NULL;
          _h -> next = NULL;

        }
        if(_h -> prev != NULL && _h -> next == NULL){         // last element in list
           printf("%s\n", "Remove: Last in List");
          _h -> prev -> next = NULL;
          _h -> prev = NULL;
        }
        if(_h -> prev != NULL && _h -> next != NULL){         // in between list
           printf("%s\n", "Remove: Middle in List");
          _h -> prev -> next = _h -> next;
          _h -> next -> prev = _h -> prev;
          _h -> prev = NULL;
          _h -> next = NULL;
        }
}
//*******************************************************
//           SPLIT BLOCK
//*******************************************************
void SplitBlocks ( HDR * _h){
    if(_h -> MagicNumber == 3028){
        int b_index = _h -> binary_Index -1;
        int newsize = pow(2,b_index)* blocksize;
        HDR * _hR = (Addr)((char*)_h + newsize);     // error need to change _hR's address 
        // _remove _h from freelist
        _remove(_h);
        // set variables for _h and _hR
		_hR -> inheritance = _h -> inheritance;
		_h -> inheritance = _h -> side;
		
        _h -> MagicNumber = 3028;      
        _h ->  is_free = 0;            
        _h -> binary_Index = b_index;        
        _h -> side = 0;                
        _h -> size = newsize;                 
        _h -> next = _hR;    
        
        _hR -> MagicNumber = 3028;      
        _hR ->  is_free = 0;            
        _hR -> binary_Index = b_index;        
        _hR -> side = 1;                
        _hR -> size = newsize;                 
        _hR -> prev = _h;
        // assign headers to freeList
        if(freeList[b_index] == NULL){  
          freeList[b_index] = _h;
          _h -> prev = NULL;
          _hR -> next = NULL;
        }
        else{
          _hR -> next = freeList[b_index];
          freeList[b_index] -> prev = _hR;
          _h -> prev = NULL;
          freeList[b_index] = _h;
        }
        printf("%s %u %d\n"," location of _h with size of:", _h, newsize);
        printf("%s %u %d\n"," location of _hR with size of:", _hR , newsize);
    }
    else
      printf("%s\n", "Error no magic number found, ");
    
}
//*******************************************************
//             MERGE BLOCKS
//*******************************************************
void MergeBlocks ( HDR * _h){
  if(_h->MagicNumber == 3028){
  int newindex = _h -> binary_Index +1;
  int newsize = pow(2,newindex) * blocksize;
  HDR * _h_ = NULL;

      if (_h -> side == 0){                      // if right block side check if right block is free
         HDR * temp = (Addr)((char*)_h + _h->size);
         _h_ = temp;}
      else{                                      // vice-versa
         HDR * temp = (Addr)((char*)_h - _h->size);
         _h_ = temp;}

      if(_h -> is_free == 0){                   
        if(_h_ -> MagicNumber == 3028){
            if(_h_ -> is_free == 0){             // _remove 2 blocks from free list
              if(_h_ -> size == _h -> size){
                 _remove(_h);
                 _remove(_h_);
                 if (_h -> side == 0){            // assign new values to the block on left (must check which block is left )
                    _h -> MagicNumber = 3028;      
                    _h ->  is_free = 0;            
                    _h -> binary_Index = newindex;        
                    _h -> side = _h -> inheritance;
					_h -> inheritance = _h_ -> inheritance;
                    _h -> size = newsize; 
                    if(freeList[newindex] == NULL){   // add the updated header into freeList
                      freeList[newindex] = _h;
                      _h -> prev = NULL;
                      _h -> next = NULL;
                    }
                    else{
                      _h -> next = freeList[newindex];
                      freeList[newindex] -> prev = _h;
                      _h -> prev = NULL;
                      freeList[newindex] = _h;
                    }                
                     
                 }
                 else{
                    _h_ -> MagicNumber = 3028;      
                    _h_ ->  is_free = 0;            
                    _h_ -> binary_Index = newindex;        
                    _h_ -> side = _h_ -> inheritance;
					_h_ -> inheritance = _h -> inheritance;               
                    _h_ -> size = newsize; 
                    if(freeList[newindex] == NULL){  
                      freeList[newindex] = _h_;
                      _h_ -> prev = NULL;
                      _h_ -> next = NULL;
                    }
                    else{
                      _h_ -> next = freeList[newindex];
                      freeList[newindex] -> prev = _h_;
                      _h_ -> prev = NULL;
                      freeList[newindex] = _h_;
                    }
                }
              }
              else
                printf("%s\n","Not the same size blocks" );
            }
            else
              printf("%s\n","Memory Location for second block is not free" );

        }
        else
          printf("%s\n","Memory Location for second block is not valid" );
      }    
      else
        printf("%s\n","Memory Location for first block is not free" );
      }
  else
    printf("%s\n","Memory Location for initial block is not valid" );
}

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
// allocate total amount of space
  totalMemory = malloc(BlockNum * _basic_block_size);
  freeList = (HDR**)malloc(index * sizeof(HDR));
// create HDR for total memory 
  HDR * a = (HDR*)totalMemory;
  a-> MagicNumber = 3028;
  a-> is_free = 0;				
  a-> binary_Index = index;
  a-> side = 2;
  a-> inheritance = 2;
  a-> size = BlockNum * _basic_block_size;
  a-> next = NULL;
  a-> prev = NULL;
  freeList[index] = a;  
  
  totalLength = BlockNum * _basic_block_size;
  totalMemory = totalMemory + sizeof(HDR);
  return totalLength;

}



/* Don't forget to implement "init_allocator" and "release_allocator"! */

void check_list(){//int list_index) {
    if(freeList[index] != NULL){
      printf("%s %d %d\n", "Size of Free List", index, freeList);
      printf("%s %d\n", "Pointer of header of first element" ,freeList[index] );
      printf("%s %d\n", "Pointer to total memory", totalMemory );
      printf("%s\n\n","Starting Split 1");
      SplitBlocks(freeList[index]);
	       print_freeList();
      printf("%s\n\n","Starting Split 2");
      SplitBlocks(freeList[index-1]);
	       print_freeList();
      printf("%s\n\n","Starting Split 3");
      SplitBlocks(freeList[index-1]);
	       print_freeList();
      printf("%s\n\n","Starting Split 4");
      SplitBlocks(freeList[index-2]);
	       print_freeList();
      printf("%s\n\n","Starting Split 5");
      SplitBlocks(freeList[index-2]);
			print_freeList();
	   printf("%s\n\n","Starting Split 6");
      SplitBlocks(freeList[index-2]);
	       print_freeList();
      printf("%s\n\n","Starting Split 7");
      SplitBlocks(freeList[index-2]);
			print_freeList();
			 printf("%s\n\n","Starting Split 8");
      SplitBlocks(freeList[index-3]);
			print_freeList();
      printf("%s\n\n","Starting Merge");
      MergeBlocks(freeList[index-4]);
			print_freeList();
      // BUGGY BUGGY BUGGY
     printf("%s\n\n","Starting Merge 2");
      MergeBlocks(freeList[index-3]);
	  print_freeList();
     // printf("%d\n", (HDR*) freeList[index]->MagicNumber );
    }
}
void print_freeList(){
  int i;
  for (i = 0 ; i <= index; i++){
    int a = pow(2, i);
    if(freeList[i] == NULL)
      printf("%s %d\n", "No elements of size: ", a);
    else
      printf("%s %d\n", "Element found at size", a);
  }
}
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
                                        // _remove from free list 
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


