#include <stdio.h>
#include <stdint.h>
#define BIT_RETURN(A,LOC) ((A & 1 << LOC) ? 1:0) 

uint8_t get_n_mantissa_bits(float number,int number_of_bits)
{
	int i=31;
	int *b;
	b=&number;
	
	int bit_store_32=0;
	uint8_t bit_store_8=0;
	
	for (i=number_of_bits;i>=0;i--)
	{
		if (BIT_RETURN(*b,i)==1)
		{
			//temp = (temp & (~(1 << temp))) | (1 << i);
			bit_store_32 |= 1 << i;
		}
	}
	
	bit_store_8=(uint8_t)bit_store_32;
	return bit_store_32;
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
    
    
    while (i--)
    {
        
        if(BIT_RETURN(fu.u,i)==1)
        {
            bit_store_32 |= 1 << i;
        }
        
    }

    bit_store_8=(uint8_t)bit_store_32;
    return bit_store_8;
}

