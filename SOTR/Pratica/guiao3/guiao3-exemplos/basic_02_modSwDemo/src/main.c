/*
 * Paulo Pedreiras, 2022/09
 * 
 * Simple SW module example
 * 
 * Ilustrates the simples way of organizing SW modules in CMake / Zephyr
 *      This approach is simple but has several limitatons. Fine for simple projects.
 *      Note that the standard CMakesLists.txt must be updated as follows (lines with ->)    
 *
 *          ...
 *          project(ModularSWDemo)
 *
 *     ->   zephyr_include_directories(MyMath) #Add this line
 *          target_sources(app PRIVATE src/main.c)
 *     ->   target_include_directories(app PRIVATE src/MyMath) #Add this line
 *     ->   target_sources(app PRIVATE src/MyMath/MyMath.c)  #Add this line
 *
 * 
 * 
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <MyMath.h>

#define STEPTIME_MS 500 /* Time between calls to MyAdd (in ms) */ 

/* Main function */
void main(void) {

    /* Variables */
    int x=0,y=10,res=0;

    /* Task init code */
    printk("Example if the use of SW modules  \n");
    printk("Call to MyAdd(x,y) in MyMath module\n\r"); 

    /* Blink loop */
    while(1) {  
        res=MyAdd(x++,y++);
        printk("MyAdd(%d,%d)=%d \n\r",x,y,res);

        /* Pause  */ 
        k_msleep(STEPTIME_MS);      
   }
    
    return;
} 