#include <stdio.h>
#include <stdint.h>
#include <limits.h> /* for CHAR_BIT */
#include <time.h>

#define BIT_RETURN(A,LOC) (( (A >> LOC ) & 0x1) ? 1:0) 

/** formatted output of ieee-754 representation of float */
void show_ieee754 (float f)
{
    union {
        float f;
        uint32_t u;
    } fu = { .f = f };
    int i = sizeof f * CHAR_BIT;

    printf ("  ");
    while (i--)
        printf ("%d ", BIT_RETURN(fu.u,i));

    putchar ('\n');
    printf (" |- - - - - - - - - - - - - - - - - - - - - - "
            "- - - - - - - - - -|\n");
    printf (" |s|      exp      |                  mantissa"
            "                   |\n\n");
}

//Print bits of an integer
void print_int_bits(int num)
{   int x=1;
   for(int bit=(sizeof(int)*8)-1; bit>=0;bit--)
   {
      /*printf("%i ", num & 0x01);
      num = num >> 1;*/
      printf("%c",(num & (x << bit)) ? '1' : '0');
   }
}

uint8_t get_n_mantissa_bits_safe(float f,int number_of_bits)
{
    union {
        float f;
        uint32_t u;
    } fu = { .f = f };
    //int i = sizeof f * CHAR_BIT;
    int i=number_of_bits;
    int bit_store_32=0;
    uint8_t bit_store_8=0;
    
    printf("\nBefore assigining any bits to bit_store_32=\n");
    print_int_bits(bit_store_32);

    while (i--)
    {
        
        if(BIT_RETURN(fu.u,i)==1)
        {
            bit_store_32 |= 1 << i;
        }
        
    }
    
    
    printf("\nAfter assigining bits to bit_store_32=\n");
    print_int_bits(bit_store_32);
    

    bit_store_8=(uint8_t)bit_store_32;
    return bit_store_8;
}

void main () 
{

    float f = 0.77808551f;
    uint8_t bit_store_8=0;

    printf ("\nIEEE-754 Single-Precision representation of: %f\n\n", f);
    show_ieee754 (f);
    
    clock_t begin=clock();
    bit_store_8=get_n_mantissa_bits_safe(f,31);
    clock_t end=clock();
    printf("\nbit_store_8=%d\t",bit_store_8);
    double time_difference=(end-begin)/CLOCKS_PER_SEC;
    printf("\ntime_difference=%f\t",time_difference);
    //return 0;
}