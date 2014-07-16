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

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

  int BlockNum = 0;         // number of blocks
  int index = 0;            // binary index of largest block
  HDR** freeList;           // pointer to freeList of headers
  int totalLength;          // size of largest block
  void * totalMemory;       // pointer to first header/total memory
  int blocksize;            // basic_block_size initilized by user

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/


void _remove(HDR * _h){
  if(_h -> prev == NULL && _h -> next == NULL){               // only element in list
          freeList[_h ->binary_Index] = NULL;
        }
  if(_h -> prev == NULL && _h -> next != NULL){               // first element in list
          freeList[_h ->binary_Index] = _h -> next;
          _h -> next -> prev = NULL;
          _h -> next = NULL;

        }
        if(_h -> prev != NULL && _h -> next == NULL){         // last element in list
          _h -> prev -> next = NULL;
          _h -> prev = NULL;
        }
        if(_h -> prev != NULL && _h -> next != NULL){         // in between list
          _h -> prev -> next = _h -> next;
          _h -> next -> prev = _h -> prev;
          _h -> prev = NULL;
          _h -> next = NULL;
        }
}

void SplitBlocks ( HDR * _h){
    if(_h -> MagicNumber == 3028){
  		if(_h -> binary_Index > 0){
  			int b_index = _h -> binary_Index -1;
  			int newsize = pow(2,b_index)* blocksize;
  			HDR * _hR = (Addr)((char*)_h + newsize);     
  			_remove(_h);                               // _remove _h from freelist
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
  			                                            // add headers to freeList
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
  		}
  		else
  			printf("%s\n", "error: cannot split last block ");		
	  }		
    else
      printf("%s\n", "error: no magic number found, ");
    
}

void MergeBlocks ( HDR * _h){
  if(_h->MagicNumber == 3028){
	  if(_h -> binary_Index < index){
	  int newindex = _h -> binary_Index +1;
	  int newsize = pow(2,newindex) * blocksize;
	  HDR * _h_ = NULL;

		  if (_h -> side == 0){                         // if left block side check if right block is free
			 HDR * temp = (Addr)((char*)_h + _h->size);
			 _h_ = temp;}
		  else{                                         // vice-versa
			 HDR * temp = (Addr)((char*)_h - _h->size);
			 _h_ = temp;}

		  if(_h -> is_free == 0){                   
			if(_h_ -> MagicNumber == 3028){
				if(_h_ -> is_free == 0){                    // _remove 2 headers from free list
				  if(_h_ -> size == _h -> size){
					 _remove(_h);
					 _remove(_h_);

					 if (_h -> side == 0){                    // assign new values to the block on left
						_h -> MagicNumber = 3028;               // (must check which block is left )
						_h ->  is_free = 0;            
						_h -> binary_Index = newindex;        
						_h -> side = _h -> inheritance;
						_h -> inheritance = _h_ -> inheritance;
						_h -> size = newsize; 

						if(freeList[newindex] == NULL){          // add the updated header into freeList
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
						 MergeBlocks(_h);                      // recursive call to merge blocks
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
						MergeBlocks(_h_);                   // recursive call to merge blocks
					}
					
				  }
				  else{}
					//printf("%s\n","error: not the same size blocks" );
				}
				else{}
				  //printf("%s\n","error: memory location for second block is not free" );
			}
			else{}
			  //printf("%s\n","error: memory location for second block is not valid" );
		  }    
		  else{}
			//printf("%s\n","error: memory location for first block is not free" );
		}
		  else{}
			//printf("%s\n","error: cannot Merge top block");
	}
  else{}
   // printf("%s\n","error: memory location for initial block is not valid" );
}

int release_allocator(){
  free(freeList);
  free(totalMemory);
  return 0;
}


extern unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length){ 
  if (_basic_block_size < 40){
    blocksize = 40;
  }
  else{
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
  return totalLength;

}



/* Don't forget to implement "init_allocator" and "release_allocator"! */

//**********************************************************
//						IGNORE CHECK_LIST() - purely for testing purposes
//**********************************************************
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

Addr my_malloc(unsigned int _length) {
     _length += 40;             // adds size of header to requested length
    int valid_length = 0;       // the possible size closest to that requested 
    int valid_index = 0;        // index of possible size of length requested
	HDR * return_hdr;             // adress of header that is being return (not really. I am returning address past the header)
    if (_length < 40){
      //printf("%s\n","error: requested memory - too small");
      return NULL;
    }
  else{
    while (valid_length < _length){         // find the size that is >= length requested
      valid_index++;
      valid_length = pow(2.0,valid_index) * blocksize;
    } 
  }

  if (index < valid_index){                 // if greater than max index then the length requested is too large
    //printf("%s\n","error: requested memory - too large");
	return NULL;
	}
  else{
		int _index = valid_index;               // trying to find an available piece of memory 
		while(freeList[_index] == NULL){
		  _index++;
		}
		if(_index > index){                     // no memory block is availible of that size or larger
			//printf("%s\n","error: no memory blocks available" );
			return NULL;
		 }
		else{
		  while (_index > valid_index){         // keep spliting till we get to the index that we want (i.e. index of size requested)
			SplitBlocks(freeList[_index]);
			_index--;
		  }
			return_hdr = (HDR*)freeList[valid_index];
			_remove(freeList[valid_index]);                // remove from free list
		  if(return_hdr -> MagicNumber == 3028){         // sets header to not free
				return_hdr -> is_free = 1;
			}
		  else{
				//printf("%s \n","error: invalid memory location ");
        return NULL;
      }

		  return (Addr)((char*)return_hdr + sizeof(HDR));
		}
  }


}
int my_free(Addr _a) {

	if(_a == NULL){
		//printf("%s \n","error: address is NULL ");
		return 1;
	}
	else{

   // _a =  (Addr)((char*)(_a - sizeof(HDR)));
		HDR* temp = (HDR*)((char*)_a - sizeof(HDR));    

		int newindex = temp->binary_Index;
		temp -> is_free = 0;                // sets header to free
		
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
  return 0;
}

