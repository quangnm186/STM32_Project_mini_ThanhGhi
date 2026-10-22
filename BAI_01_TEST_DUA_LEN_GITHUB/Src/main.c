#include<stdio.h>
#include<stdint.h>

#define RCC_ADD_BASE 0x40021000ul
#define RCC_ADD_APB2ENR (RCC_ADD_BASE + 0x18)
#define APB2ENR (*((volatile uint32_t*)RCC_ADD_APB2ENR))

//cau hinh che do cho GPIOC
#define GPIOC_ADD_BASE 0x40011000ul
#define GPIOC_ADD_CRH (GPIOC_ADD_BASE + 0x04)
#define GPIOC_ADD_ODR (GPIOC_ADD_BASE + 0x0C)
#define GPIOCCRH (*((volatile uint32_t*)GPIOC_ADD_CRH))
#define GPIOCODR (*((volatile uint32_t*)GPIOC_ADD_ODR))	
	
// Thanh ghi SysTick (Core Cortex-M3)
#define SYSTICK_BASE      0xE000E010ul
#define STK_CTRL          (*((volatile uint32_t*)(SYSTICK_BASE + 0x00)))
#define STK_LOAD          (*((volatile uint32_t*)(SYSTICK_BASE + 0x04)))
#define STK_VAL           (*((volatile uint32_t*)(SYSTICK_BASE + 0x08)))

void mDelay(uint32_t ms)
{
    while(ms--)
    {
        // Nap so tick cho 1ms voi clock 8MHz (8000 - 1)
        STK_LOAD = 72000 - 1;
        
        // Reset bo dem hien tai
        STK_VAL = 0;
        
        // Bit 0: ENABLE = 1 (Bat bo dem)
        // Bit 2: CLKSOURCE = 1 (Dung xung core FCLK = 8MHz)
        STK_CTRL = 0x05;
        
        // Cho co COUNTFLAG (bit 16) nhay len 1 (khi bo dem dem ve 0)
        while((STK_CTRL & (1 << 16)) == 0);
        
        // Tat SysTick
        STK_CTRL = 0;
    }
}

int main()
	{
		//bat clock cho port C
		APB2ENR = 0x00000010;//quy doi ra la 0000 ... 0001 0000 (he nhi phan 32 bit) 
		//bat che do output pushpull 50MHz cho portC
		GPIOCCRH = 0x300000;
		while(1)
			{
				GPIOCODR |= (1<<13);
				mDelay(500);
				GPIOCODR &= (uint32_t)~(1<<13);
				mDelay(500);
			}
	}