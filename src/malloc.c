#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define FIT 0
#define ALIGN4(s)         (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)      ((b) + 1)
#define BLOCK_HEADER(ptr)   ((struct _block *)(ptr) - 1)


static int atexit_registered = 0;
static int num_mallocs       = 0;
static int num_frees         = 0;
static int num_reuses        = 0;
static int num_grows         = 0;
static int num_splits        = 0;
static int num_coalesces     = 0;
static int num_blocks        = 0;
static int num_requested     = 0;
static int max_heap          = 0;

/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */
void printStatistics( void )
{
  printf("\nheap management statistics\n");
  printf("mallocs:\t%d\n", num_mallocs );
  printf("frees:\t\t%d\n", num_frees );
  printf("reuses:\t\t%d\n", num_reuses );
  printf("grows:\t\t%d\n", num_grows );
  printf("splits:\t\t%d\n", num_splits );
  printf("coalesces:\t%d\n", num_coalesces );
  printf("blocks:\t\t%d\n", num_blocks );
  printf("requested:\t%d\n", num_requested );
  printf("max heap:\t%d\n", max_heap );
}

struct _block 
{
   size_t  size;         /* Size of the allocated _block of memory in bytes */
   struct _block *prev;  /* Pointer to the previous _block of allcated memory   */
   struct _block *next;  /* Pointer to the next _block of allcated memory   */
   bool   free;          /* Is this _block free?                     */
   char   padding[3];
};


struct _block *heapList = NULL; /* Free list to track the _blocks available */
struct _block *savePtr;
/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes 
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 */
struct _block *findFreeBlock(struct _block **last, size_t size) 
{
   struct _block *curr = heapList;

#if defined FIT && FIT == 0
   /* First fit */
   while (curr && !(curr->free && curr->size >= size)) 
   {
      *last = curr;
      curr  = curr->next;
   }
#endif

#if defined BEST && BEST == 0
   /* Best fit */
   //keep track of best fitting block
   struct _block *bestfit = NULL;
   //keep track of least space wasted
   size_t LestSpace = (size_t) INT_MAX; 

   //run through the entire heap list
   while (curr) 
   {
      //if suitable block found
      if(curr->free && (curr->size >= size))
      {
         //check to see if it wasts less space than the 
         //least space
         if((curr -> size - size) < LestSpace)
         {
            //Update the best fitting block
            bestfit = curr;
            //Update of the least space wasted 
            LestSpace = (curr -> size - size);
         }
      }
      //move the pointer
      curr  = curr->next;
   }
   //returning curr 
   curr = bestfit;
#endif

#if defined WORST && WORST == 0
   /* worst fit */
   //keep track of worst fitting block
   struct _block *worstfit = NULL;
   //keep track of most space wasted
   size_t MostSpace = 0; 

   //run through the entire heap list
   while (curr) 
   {
      //if suitable block found
      if(curr->free && (curr->size >= size))
      {
         //check to see if it wasts more space than the 
         //mostspace
         if((curr -> size - size) > MostSpace)
         {
            //Update the worst fitting block
            worstfit = curr;
            //Update of the most space wasted 
            MostSpace = curr -> size - size;
         }
      }
      //move the pointer
      curr  = curr->next;
   }
   //returning curr
   curr = worstfit;
#endif

#if defined NEXT && NEXT == 0
   /*Next Fit */
   
   //Check to see if there exists a leftoff location
   if(savePtr != NULL)
   {
      //start from left off location
      curr = savePtr;
   }

   //loop though the linked list unless it ends or
   //finds a correct block
   while (curr && !(curr->free && curr->size >= size)) 
   {
      //Update the last(its passed as reference)
      *last = curr;
      //Move pointer forward.
      curr  = curr->next;
   }

   //if curr comes out as NULL that means no space found and 
   //checking to see if savePtr is NULL to make sure we dont 
   //Rerun through the entire list again
   if(curr == NULL && savePtr != NULL )
   {
      //Restart the search at the begining of the head
      curr = heapList;
      //Search until savePtr(To make sure not to rerun through the loop)
      while(curr != savePtr && !(curr->free && curr->size >= size))
      {
         *last =curr;
         curr = curr -> next;
      }
      //if curr ends up as saveptr, no memory was found.
      //need to grow heap
      if(curr == savePtr)
      {
         return NULL;
      }

   }
   savePtr = curr;

#endif

   //if the function return not NULL, 
   //a block has been reused.
   if(curr != NULL)
   {
      num_reuses++;
   }

   return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically 
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size) 
{
   /* Request more space from OS */
   struct _block *curr = (struct _block *)sbrk(0);
   struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

   assert(curr == prev);

   /* OS allocation failed */
   if (curr == (struct _block *)-1) 
   {
      return NULL;
   }

   /* Update heapList if not set */
   if (heapList == NULL) 
   {
      heapList = curr;
   }

   /* Attach new _block to prev _block */
   if (last) 
   {
      last->next = curr;
   }

   /* Update _block metadata */
   curr->size = size;
   curr->next = NULL;
   curr->free = false;
   curr->prev = last;
   num_grows++;
   num_blocks++;
   max_heap += size;
   return curr;
}

/*brief combineblocksNxt
* combines two consecutive forward blocks
* parameters curr block to combine with blocks next block
*
*/
struct _block* combineblocksNxt(struct _block* curr){

   //the new size will be the current blocks size + size of next block
   //+ the size of struck of the block we get rid of.
   curr -> size = curr -> size +  curr->next->size + sizeof(struct _block);
   //Attach the pointers.
   curr -> next = curr -> next -> next;

   //Deal with Counters.
   num_blocks--;
   num_coalesces++;

   return curr;
}


/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */
void *malloc(size_t size) 
{
   num_requested += size;
   
   if( atexit_registered == 0 )
   {
      atexit_registered = 1;
      atexit( printStatistics );
   }

   /* Align to multiple of 4 */
   size = ALIGN4(size);

   /* Handle 0 size */
   if (size == 0) 
   {
      return NULL;
   }

   /* Look for free _block */
   struct _block *last = heapList;
   struct _block *next = findFreeBlock(&last, size);

   /* TODO: Split free _block if possible */
   if(next != NULL)
   {
      //Split only if  block has size greater
      //than requested size and the size diffrence 
      //is big enough to carty the next block
      if(((next -> size) - size) > sizeof(struct _block))
      {
         //Save the pointer to the next node 
         struct _block* temp_next = next -> next;
         //Save the original size of returned block 
         size_t temp_size = next -> size;

         //Attach the new size onto the block
         next -> size = size; 
         //Set the next pointer as a new block 
         //-> this should be size + size of block ahead oh the head pointer

         //Casting Block_header as char pointer so that when 
         //we increase the pointer using size, it moves 1 byte * size
         next -> next = (struct _block*)(((char*)BLOCK_HEADER(next))    
                        + sizeof(struct _block) + (size));
      
         //set the next's next back to the original pointer
         next -> next -> next = temp_next;
         //set the size of the next block to the remaining size
         next -> next -> size = temp_size - size - sizeof(struct _block);
         //set the prev pointer for the next block
         next -> next -> prev = next;
         //free the pointer
         next->next->free = 1;
         ++num_splits;
         ++num_blocks;
      }

   } 

   /* Could not find free _block, so grow heap */
   if (next == NULL) 
   {
      next = growHeap(last, size);
   }

   /* Could not find free _block or grow heap, so just return NULL */
   if (next == NULL) 
   {
      return NULL;
   }
   
   
   /* Mark _block as in use */
   next->free = false;

   num_mallocs++;

   /* Return data address associated with _block */
   return BLOCK_DATA(next);
}


/*
 * \brief calloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes, nmemb number of sizes.
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */

void *calloc(size_t nmemb, size_t size){

   //returns BLOCKdata of next
   struct _block *next = malloc(nmemb*size);
   //set all the old data into 0
   memset(next,0,next->size);
   //return next
   return next;
} 

 /*
 * \brief realloc
 *
 * finds the _block of heap memory given for the calling process.
 * Shrinks the _block of heap memory given for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes, pte pointer to initial malloc
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */

void *realloc(void *ptr, size_t size)
{
   //check to see if given pointer and size is valid 
   if(ptr == NULL || size == 0)
   {
      return malloc(size);
   }
   //while reallocating user provides pointer to block data
   //convert it to block header.
   struct _block *curr = BLOCK_HEADER(ptr);

   //check to see if the given block is enough to fit 
   //the requested size;
   if(curr->size > size)
   {
      //if user calls for reallocating to pointer
      //to decreashrinkse the memory size.
      if(curr->size > (size + sizeof(struct _block)))
      {
         
         struct _block* temp_next = curr -> next;
         //Save the original size of returned block 
         size_t temp_size = curr -> size;

         //Attach the new size onto the block
         curr -> size = size; 
         //Set the next pointer as a new block 
         //-> this should be size + size of block ahead oh the head pointer
         
         //Casting Block_header as char pointer so that when 
         //we increase the pointer using size, it moves 1 byte * size
         curr -> next = (struct _block*)(((char*)(curr))     // REQUIRES FIX
                        + sizeof(struct _block) + (size));
         
         //set the next's next back to the original pointer
         curr -> next -> next = temp_next;
         //set the size of the next block to the remaining size
         curr -> next -> size = temp_size - size - sizeof(struct _block);
         
         //set the prev pointer for the next block
         curr -> next -> prev = curr;
         //free the pointer
         curr -> next -> free = true;
         
         
         ++num_splits;
         ++num_blocks; 
         return BLOCK_DATA(curr);
      }
      else
      {
         return BLOCK_DATA(curr);
      }
   }
   else
   {
      //check to see if the memory is expandable
      if(curr-> next && curr->next->free && ((curr->next->size + curr->size) > size ))
      {
         //setting the data into 0 before reallocating it
         //this avoids issues for programmers.
         //memset(BLOCK_DATA(curr), 0, curr->size);
         //Comibining blocks
         curr = combineblocksNxt(curr);
         
         if(curr->size > size + sizeof(struct _block))
         {

            //Save the pointer to the next node 
            struct _block* temp_next = curr -> next;
            //Save the original size of returned block 
            size_t temp_size = curr -> size;

            //Attach the new size onto the block
            curr -> size = size; 
            //Set the next pointer as a new block 
            //-> this should be size + size of block ahead oh the head pointer
            
            //Casting Block_header as char pointer so that when 
            //we increase the pointer using size, it moves 1 byte * size
            curr -> next = (struct _block*)(((char*)(curr))    
                           + sizeof(struct _block) + (size));
            
            //set the next's next back to the original pointer
            curr -> next -> next = temp_next;
            //set the size of the next block to the remaining size
            curr -> next -> size = temp_size - size - sizeof(struct _block);
            //set the prev pointer for the next block
            curr -> next -> prev = curr;
            //free the pointer
            curr -> next -> free = true;

            ++num_splits;
            ++num_blocks; 
            
            return BLOCK_DATA(curr);
         }
         else
         {
            return BLOCK_DATA(curr);            
         }
      }
      else if(curr -> next == NULL)
      {
         //this means that realloc needs to grow heap
         growHeap(curr, size - curr->size);
         //increase the requested memory
         num_requested += (size - curr->size);
         //combine the new block
         combineblocksNxt(curr);
         //set the block as taken
         curr -> free = false;
         return BLOCK_DATA(curr);
      }
      else
      {
         //this means that reacllocation failed
         //allocate new chunk of memory using malloc
         struct _block* newblock =  malloc(size);
         //copy the data onto new block
         memcpy(BLOCK_DATA(newblock),ptr,curr->size);
         //using ptr cause  block header is set in free 
         free(ptr);
         return BLOCK_DATA(newblock);
      }
   }
}

/*
 * \brief free 
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *ptr) 
{
   if (ptr == NULL) 
   {
      return;
   }

   /* Make _block as free */
   struct _block *curr = BLOCK_HEADER(ptr);
   assert(curr->free == 0);
   curr->free = true;
   num_frees++;


   //Utilizing Doubly linked list to colace the blocks
   
   //check to see if prev block exists and is free
   if(curr->next && curr->next->free == 1)
   {
      //if free combine with the currblock
      curr = combineblocksNxt(curr);
   }
   //check to see if prev block exists and is free
   if(curr->prev != NULL && curr->prev->free == 1)
   {
      //if free combine with the currblock with 
      //head pointer as prevblock's pointer
      curr = combineblocksNxt(curr->prev);
   }
}


/* 7f704d5f-9811-4b91-a918-57c1bb646b70       --------------------------------*/
/* vim: set expandtab sts=3 sw=3 ts=6 ft=cpp: --------------------------------*/
