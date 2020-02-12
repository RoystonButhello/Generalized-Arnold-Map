#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define BIT_RETURN(A,LOC) ((A & 1 << LOC) ? 1:0) 

//Print bits of an integer
void print_int_bits(int num)
{	int x=1;
   for(int bit=(sizeof(int)*8)-1; bit>=0;bit--)
   {
      /*printf("%i ", num & 0x01);
      num = num >> 1;*/
   	  printf("%c",(num & (x << bit)) ? '1' : '0');
   }
}

//Clear a bit at a given location and then set the bit if specified 
static inline uint8_t transfer_bits_to_int(float number,int number_of_bits)
{
	int i=31;
	int *b;
	b=&number;
	int bit_store_32=0;
	uint8_t bit_store_8=0;
	printf("\nBefore setting bits=");
	print_int_bits(bit_store_32);

	for (i=number_of_bits;i>=0;i--)
	{
		if (BIT_RETURN(*b,i)==1)
		{
			//temp = (temp & (~(1 << temp))) | (1 << i);
			bit_store_32 |= 1 << i;
		}
	}
	printf("\nAfter setting bits=");
	print_int_bits(bit_store_32);
	bit_store_8=(uint8_t)bit_store_32;
	return bit_store_8;
}

//Print bits of a float
static inline void print_float_bits(float number)
{
  int *float_ptr;
  float_ptr=&number;
  printf("\nBits of float a=");
  int i=0;
    for (i = 31; i>=0; i--)
    {
        printf("%d",BIT_RETURN(*float_ptr,i));
    }
}

void main(int argc, char **argv[])
{
	
	 //11000010111011010100000000000000  
    // 1 sign bit | 8 exponent bit | 23 fraction bits
    //float a = -118.625; 
    float a=0.43072589;
    uint8_t bit_store_8=0;
    printf("\na=%f\t",a);
    print_float_bits(a);
    clock_t begin=clock();
    bit_store_8= transfer_bits_to_int(a,1);
    clock_t end=clock();
    double time_spent=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("\ntime_spent=%f\t",time_spent);
    printf("\nbit_store_8=%d\t",bit_store_8);
    //return 0;
}

