#include<lpc214x.h>
#define WATER (IO0PIN & 1<<14)
#define PLOCK 0x00000400
unsigned int adc(int no,int ch);
void plantWatering();
void SystemInit(void);
void delay_ms(unsigned int j);
void delay_ms(unsigned int j)
{ 
  unsigned int x,i; 
  for(i=0;i<j;i++) 
   { 
      for(x=0; x<10000; x++);     
   } 
} 

void SystemInit(void) 
{ 
   PLL0CON = 0x01;  
   PLL0CFG = 0x24;  
   PLL0FEED = 0xAA;  
   PLL0FEED = 0x55;  
   while( !( PLL0STAT & PLOCK )) 
   { ; } 
   PLL0CON = 0x03; 
   PLL0FEED = 0xAA;  // lock the PLL registers after setting the required PLL 
   PLL0FEED = 0x55; 
   VPBDIV = 0x01;      // PCLK is same as CCLK i.e 60Mhz   
} 


void plantWatering()
{
	unsigned int value = adc(1,4);
	if(value<100)
	{
		 unsigned int  no_of_steps_clk = 50, no_of_steps_aclk = 50; 
		 IO0DIR |= 1U << 31 | 0x00FF0000 | 1U << 30; // to set P0.16 to P0.23 as o/ps  
		 
		 SystemInit( ); 
		do{ 
		IO0CLR = 0X000F0000;IO0SET = 0X00010000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		IO0CLR = 0X000F0000;IO0SET = 0X00020000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		IO0CLR = 0X000F0000;IO0SET = 0X00040000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		IO0CLR = 0X000F0000;IO0SET = 0X00080000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		}while(1); 
		while((value = adc(1,4))<100);
		do{ 
		IO0CLR = 0X000F0000;IO0SET = 0X00080000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		IO0CLR = 0X000F0000;IO0SET = 0X00040000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		IO0CLR = 0X000F0000;IO0SET = 0X00020000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		IO0CLR = 0X000F0000;IO0SET = 0X00010000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		}while(1); 
		IO0CLR = 0X00FF0000; 
		while(1); 

	}
}

unsigned int adc(int no,int ch) 
{ 
            // adc(1,4) for humidity sensor sensor LM34, digital value will increase as temp increases 
 // adc(1,3) for LDR - digival value will reduce as the light increases 
 // adc(1,2) for trimpot - digital value changes as the pot rotation 
 unsigned int val; 
 PINSEL0 |=  0x0F300000;   /* Select the P0_13 AD1.4 for ADC function */ 
                                             /* Select the P0_12 AD1.3 for ADC function */ 
           /* Select the P0_10 AD1.2 for ADC function */ 
       switch (no)        //select adc 
    { 
        case 0: AD0CR=0x00200600|(1<<ch);       //select channel 
                AD0CR|=(1<<24);                             //start conversion 
                while((AD0GDR& (1U<<31))==0); 
                val=AD0GDR; 
                break; 
  
        case 1: AD1CR=0x00200600|(1<<ch);       //select channel 
                AD1CR|=(1<<24);                              //start conversion 
                while((AD1GDR&(1U<<31))==0); 
                val=AD1GDR; 
                break; 
    } 
    val=(val >> 6) & 0x03FF;         // bit 6:15 is 10 bit AD value 
    return val; 
}

int main()
{
	do
	{
		plantWatering();
	}while(1);
	return 0;
}