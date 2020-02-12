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

// bit returned at location
/*static inline int bit_return(int a, int loc)   
{
    int buf = a & 1<<loc;

    if (buf == 0) return 0;
    else return 1; 
}*/

//Clear a bit at a given location and then set the bit if specified 
static inline void transfer_bits_to_int(float number)
{
	int i=31;
	int *b;
	b=&number;
	int temp=0;
	uint8_t tmp_8=0;
	printf("\nBefore setting bits=");
	print_int_bits(temp);

	for (i=31;i>=0;i--)
	{
		if (BIT_RETURN(*b,i)==1)
		{
			//temp = (temp & (~(1 << temp))) | (1 << i);
			temp |= 1 << i;
		}
	}
	printf("\nAfter setting bits=");
	print_int_bits(temp);
	printf("\ntemp=%d\t",temp);
	tmp_8=(uint8_t)temp;
	printf("\ntmp_8=%d\t",tmp_8);
	
}


int main(int argc, char **argv[])
{
	
	 //11000010111011010100000000000000  
    // 1 sign bit | 8 exponent bit | 23 fraction bits
    //float a = -118.625; 
    float a=0.129195;
    int *b;
    b = &a;
    printf("\na=%f\t",a);
    int i;
    printf("\nBits of float a=");
    for (i = 31; i>=0; i--)
    {
        printf("%d",BIT_RETURN(*b,i));
    }
    //clock_t begin=clock();
    transfer_bits_to_int(a);
    //clock_t end=clock();
    //double time_spent=(double)(end-begin)/CLOCKS_PER_SEC;
    //printf("\ntime_spent=%f\t",time_spent);
    return 0;
}

