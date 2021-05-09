#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define PAGESIZE 4096

int
main(int argc, char *argv[]){

    //#if FIFO
    printf(1,"-- Process Starts Executing\n");
    int i,j;
    char key_stroke[10];
    char *arr[14];
    printf(1,"\npress ctrl+P to get initial process details\n");
    gets(key_stroke,10);
    printf(1,"\n-- Adding 12 more pages to the process\n");
    //We already have 3 pages for the process in the main memory.
    //We will now allocate 12 more pages to get to the 15 page limit that we had set
    for(i = 0; i<12; i++){
        arr[i] = sbrk(PAGESIZE);
        printf(1,"arr[%d]=0x%x\n",i,arr[i]);
    }
    printf(1,"\npress ctrl+P to get process details\n");
    gets(key_stroke,10);

    
    //Adding one more page should replace page number 0 in memory with 12. 
    printf(1,"\n-- Aloocating 16th page for the process\n");
    arr[12] = sbrk(PAGESIZE);
    //sbrk returning 61440 but below statement not getting called
    printf(1,"\npress ctrl+P to get process details\n");
    gets(key_stroke,10);
    /*
    Note that the number of page swaps are 2 
    bcz page 0 is swapped with page 16 and then 
    page fault occurs. Now page 0 will be swapped 
    with page 1. Now swap space has page 1.
    */

   
    printf(1,"\n-- Allocating 17th page for the process\n");
    arr[13] = sbrk(PAGESIZE);
    printf(1,"arr[13]=0x%x",arr[13]);
    printf(1,"\npress ctrl+P to get process details\n");
    gets(key_stroke,10);
    /* 
    The number of page swaps should be 4 bcz page 17 
    is swpped for page 2 but page 2 contains user 
    stack so it will be swapped for page 3. Now swap 
    space has 2 pages i.e page 1 and page 3.
    */


    printf(1,"\n-- Trying to cause 6 more page faults by using pages from the swapspace\n");
    for(i=0; i<6; i++){
        for(j=0; j<PAGESIZE; j++)
            arr[i][j] = 'S';
    }
    printf(1,"\npress ctrl+P to get process details\n");
    gets(key_stroke,10);
    /* 
    Note that this function calls arr[i][j] which 
    needs the arr allocated but it was swapped out 
    to the disk. Hence, there is a page fault for 
    every iteration. Thus the total page faut count 
    goes to 2+6 = 8.
    */


    printf(1,"\n-- Testing for fork()\n");
    if(fork() == 0){
        printf(1,"\nRunning the child process now, PID: %d\n",getpid());
        printf(1,"\npress ctrl+P to get process details\n");
        gets(key_stroke,10);
        arr[6][0] = 'J';
        printf(1,"\nExpected page table fault for page number 9.\n");
        printf(1,"\npress ctrl+P to get process details\n");
        gets(key_stroke,10);
        exit();
    }
    else{
        wait();
        printf(1,"\n-- Deallocating all pages from the parent process\n");
        sbrk(-14*PAGESIZE);
        printf(1,"\npress ctrl+P to get process details\n");
        gets(key_stroke,10);
        exit();   
    }
    return 0;
}