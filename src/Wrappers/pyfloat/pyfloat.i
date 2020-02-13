
/* file : gfg.i */
  
/* name of module to use*/
%include "stdint.i"
%module pyfloat
%{ 
    /* Every thing in this file is being copied in  
     wrapper file. We include the C header file necessary 
     to compile the interface */
    #include "pyfloat.h" 
  	#include <stdint.h>
    /* variable declaration*/
   
%} 
  
/* explicitly list functions and variables to be interfaced */
uint8_t get_n_mantissa_bits(float number,int number_of_bits);
uint8_t get_n_mantissa_bits_safe(float f,int number_of_bits);
#define BIT_RETURN(A,LOC) ((A & 1 << LOC) ? 1:0)   
/* or if we want to interface all functions then we can simply 
   include header file like this -  
   %include "gfg.h" 
*/
