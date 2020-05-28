#include<lpc214x.h>
#define DOOR_OPEN (IO0PIN & 1<<14)
#define PLOCK 0x00000400
#define LED_OFF IO0SET = 1 << 31
#define LED_ON IO0CLR = 1 << 31
unsigned int adc(int no,int ch);
void plantWatering();
void SystemInit(void);
void delay_ms(unsigned int j);
void doorOpening();
void beep();
void beep()
{
	PINSEL1 |= 0x00080000; /* P0.25 as DAC output :option 3 - 10 (bits18,19)*/ 

 //P0.25 used as AOUT - DAC output 
 //val - 0 to 1023  : 10 bit DAC, P0.25 used as AOUT generates analog output 
 unsigned int val=0;
	for(int i=0;i<10;i++)
	{
	 DACR = ( (1<<16) | (1023<<6) ); 
	 delay_ms(1); 
		DACR = ( (1<<16) | (0<<6) ); //stop the buzzer
		delay_ms(1); 
	}
 
 
}
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
		

	}
}

unsigned int adc(int no,int ch) 
{ 
            // adc(1,4) for external humidity sensor
 //  adc(1,3) for internal humidity sensor 
 //  adc(1,2) for light sensor
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

void doorOpening()
{
	//P0.12 for IR sensor
	unsigned int digitalValue = !(IO0PIN & (1<<12)); //read value of port P0.12
	if(digitalValue==1)
	{
		beep(); //buzzer 
		if(!DOOR_OPEN) //switch to open the door. If it is pressed the stepper motor attached to the door opens the door.
		{
			unsigned int  no_of_steps_clk = 50, no_of_steps_aclk = 50; 
		 IO0DIR |= 1U << 31 | 0x00F00000 | 1U << 30; // to set P0.20 to P0.23 as o/ps  
		 
		 
		do{ 
		IO0CLR = 0X00F00000;IO0SET = 0X00100000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		IO0CLR = 0X00F00000;IO0SET = 0X00200000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		IO0CLR = 0X00F00000;IO0SET = 0X00400000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		IO0CLR = 0X00F00000;IO0SET = 0X00800000;delay_ms(10);if(--no_of_steps_clk == 0) break; 
		}while(1); 
		while((digitalValue = !(IO0PIN & (1<<12)))==1);
		do{ 
		IO0CLR = 0X00F00000;IO0SET = 0X00800000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		IO0CLR = 0X00F00000;IO0SET = 0X00400000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		IO0CLR = 0X00F00000;IO0SET = 0X00200000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		IO0CLR = 0X00F00000;IO0SET = 0X00100000;delay_ms(10);if(--no_of_steps_aclk == 0) break; 
		}while(1); 
		IO0CLR = 0X00F00000; 
		 

		}
	}
}

void rundcmotor(unsigned int speed){
	
	PINSEL0 |= 2<< 18; //P0.9 for PWM6
	PWMPCR = (1<<14);
	PWMMR0 = 1000;
	PWMMR6 = (1000U*speed)/100;
	PWMTCR = 0x00000009;
	PWMLER = 0x70;
}
	

void runfan(){
		unsigned int val = adc(1,3)/10;
		if(val>30) rundcmotor(100);
		else{
			if(val<5)  rundcmotor(0);
			else{
				rundcmotor(val);
			}
		}
		delay_ms(10);
}

void turnonlights(){
	unsigned int val = adc(1,2);
	if(val>200) LED_OFF;
	else LED_ON;
}



int main()
{
	SystemInit( ); 
	IO0DIR|= 1U<<31; // FOR LED P0.31
	do
	{
		//doorOpening();
		//plantWatering();
		runfan();
		//turnonlights();
	}while(1);
	return 0;
}
