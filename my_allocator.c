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
		if(_h -> binary_Index > 0){
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
			printf("%s\n", "error: cannot split last block ");		
	}		
    else
      printf("%s\n", "Error no magic number found, ");
    
}
//*******************************************************
//             MERGE BLOCKS
//*******************************************************
void MergeBlocks ( HDR * _h){
  if(_h->MagicNumber == 3028){
	  if(_h -> binary_Index < index){
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
						 MergeBlocks(_h);
					 }
					 else{
						_h_ -> MagicNumber = 3028;      
						_h_ ->  is_free = 0;            
						_h_ -> binary_Index = newindex;        
						_h_ -> side = _h_ -> inheritance;
						_h_ -> inheritance = _h -> inheritance;               
						_h_ -> size = newsize; 

						if(freeList[newindex] == NULL){     // add the updated header into freeList
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
						MergeBlocks(_h_);
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
			printf("%s\n","Cannot Merge Top Block");
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
  if (_basic_block_size < 40){
    printf("%s %d\n","Block size is being reset. Header size: ", sizeof(HDR));
    blocksize = 40;
  }
  else{
    printf("%s %d\n","Block size not being reset. Header: ", sizeof(HDR));
    blocksize = _basic_block_size;
  }
  if ((double)(_length/blocksize) < 1)
    return 1;
  else{
    while (BlockNum < _length/blocksize){
      index++;
      BlockNum = pow(2.0,index);
    }
  }
// creates free list of "index" spaces 
// allocate total amount of space
  totalMemory = malloc(BlockNum * blocksize);
  freeList = (HDR**)malloc(index * sizeof(HDR));
// create HDR for total memory 
  HDR * a = (HDR*)totalMemory;
  a-> MagicNumber = 3028;
  a-> is_free = 0;        
  a-> binary_Index = index;
  a-> side = 2;
  a-> inheritance = 2;
  a-> size = BlockNum * blocksize;
  a-> next = NULL;
  a-> prev = NULL;
  freeList[index] = a;  
  
  totalLength = BlockNum * blocksize;
  totalMemory = totalMemory;
  printf("%s %d\n", "Index: ", index );
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
      printf("%s\n\n","Starting Merge 2");
      MergeBlocks(freeList[index-3]);
        print_freeList();
      printf("%s\n\n","Starting Merge 3");
      MergeBlocks(freeList[index-3]);
        print_freeList();
      printf("%s\n\n","Starting Merge 4");
      MergeBlocks(freeList[index-3]);
        print_freeList();
      printf("%s\n\n","Starting Merge 5");
      MergeBlocks(freeList[index-3]);
        print_freeList();
      printf("%s\n\n","Starting Merge 6");
      MergeBlocks(freeList[index-2]);
        print_freeList();
      printf("%s\n\n","Starting Merge 7");
      MergeBlocks(freeList[index-2]);
        print_freeList();
      printf("%s\n\n","Starting Merge 8");
      MergeBlocks(freeList[index-1]);
        print_freeList(); 
     // printf("%d\n", (HDR*) freeList[index]->MagicNumber );
    }
}
void print_freeList(){
  int i;
  for (i = 0 ; i <= index; i++){
    int a = pow(2, i);
    if(freeList[i] == NULL)
      printf("%s %d\n", "No elements of size: ", a * blocksize);
    else
      printf("%s %d\n", "Element found at size", a * blocksize);
  }
  printf("\n");
}
extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.*/
     _length += 40;
    int valid_length = 0;
    int valid_index = 0;
	HDR * return_hdr;
    if (_length < 40){
      printf("%s\n","Error: requested memory - too small");
    }
  else{
    while (valid_length < _length){
      valid_index++;
      valid_length = pow(2.0,valid_index) * blocksize;
    } 
  }
  printf("%s %d\n","Length: ",_length);
  printf("%s %d\n","Valid Index: ",valid_index);
  printf("%s %d\n","Valid Length: ",valid_length);

  if (index < valid_index)
    printf("%s\n","Error: requested memory - too large");
  else{
    int _index = valid_index;
    while(freeList[_index] == NULL){
      _index++;
    }
    printf("%s %d\n","New valid Index: ",_index);
    if(_index > index)
      printf("%s\n","Cannot Provide Memory Block" );
    else{
      while (_index > valid_index){
        SplitBlocks(freeList[_index]);
        _index--;
      }
		print_freeList();
		return_hdr = (HDR*)freeList[valid_index];
		_remove(freeList[valid_index]);
		print_freeList();
//      printf("%s %u\n","Location of beginning totalMemory: ", totalMemory);
//     printf("%s %u\n","Location of end of totalMemory: ", totalMemory + *****);
      printf("%s %u\n","Normal Address(i.e. header location): ", (Addr)((char*)return_hdr ));
      printf("%s %u\n","Return Address: ", (Addr)((char*)return_hdr+sizeof(HDR) ));
	  if(return_hdr -> MagicNumber == 3028){
			return_hdr -> is_free = 1;
			printf("%s \n","My Malloc.... header in correct location ");
		}
	  else
			printf("%s \n","My Malloc....WRONG LOCATION ");

      return (Addr)((char*)return_hdr + sizeof(HDR));
    }
  }


}
extern int my_free(Addr _a) {
  /* Same here! */

//  if (_a < totalMemory || _a > (totalMemory + (blocksize * BlockNum) ))
 //     printf("%s\n","Address out of bounds");
	if(_a == NULL){
		printf("%s \n","error: address is NULL ");
		return 1;
	}
	else{

   // _a =  (Addr)((char*)(_a - sizeof(HDR)));
		HDR* temp = (HDR*)((char*)_a - sizeof(HDR));
		printf("\n\n%s %d\n\n","Calling my_free on....", temp->size);
		//printf("%s %u\n","Header for temp: ", temp );
		//printf("%d\n", temp -> MagicNumber );
		int newindex = temp->binary_Index;
		temp -> is_free = 0;
		
		if(freeList[newindex] == NULL){     // add the updated header into freeList
            freeList[newindex] = temp;
             temp -> prev = NULL;
             temp -> next = NULL;
         }
         else{
            temp -> next = freeList[newindex];
            freeList[newindex] -> prev = temp;
            temp -> prev = NULL;
            freeList[newindex] = temp;
         }
		 MergeBlocks(temp);
  }
  print_freeList();
  return 0;
}

