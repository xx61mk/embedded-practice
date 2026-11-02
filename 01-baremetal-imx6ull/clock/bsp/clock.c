#include "MCIMX6Y2.h"
#include "clock.h"

// 使能外设时钟
void enable_clocks(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}

// 初始化时钟
void clock_init(void)
{
    unsigned int t = 0;

    // 1. 配置ARM时钟
    CCM->CCSR &= ~(1 << 8);
    CCM->CCSR |= (1 << 2);
    t = CCM_ANALOG->PLL_ARM;
    t &= ~(3 << 14);  
    t |= (1 << 13);
    t |= (88 << 0);
    CCM_ANALOG->PLL_ARM = t;
    CCM->CACRR |= (1 << 0);
    CCM->CCSR &= ~(1 << 2);

    // 2. 配置PLL2 PFD 
    t = CCM_ANALOG->PFD_528;
    t &= ~((0x3F << 0) | (0x3F << 8) | (0x3F << 16) | (0x3F << 24));
    t &= ~((1 << 7) | (1 << 15) | (1 << 23) | (1 << 31));
    t |= (27 << 0) | (16 << 8) | (24 << 16) | (32 << 24);
    CCM_ANALOG->PFD_528 = t; 

    // 3. 配置PLL3 PFD
    t = CCM_ANALOG->PFD_480;
    t &= ~((0x3F << 0) | (0x3F << 8) | (0x3F << 16) | (0x3F << 24));
    t &= ~((1 << 7) | (1 << 15) | (1 << 23) | (1 << 31));
    t |= (12 << 0) | (16 << 8) | (17 << 16) | (19 << 24);
    CCM_ANALOG->PFD_480 = t; 

    // 4. 配置三个根时钟
    t = CCM->CBCMR;
    t &= ~(3 << 18);
    t |= (1 << 18);
    CCM->CBCMR = t;

    t = CCM->CBCDR;
    t &= ~(1 << 25);
    t &= ~((7 << 10) | (3 << 8));
    t |= (1 << 11) | (1 << 8);
    CCM->CBCDR = t;

    CCM->CSCMR1 &= ~(1 << 6);
    CCM->CSCMR1 &= ~(0x3F << 0); 

    // 5. 打开所有外设时钟门
    enable_clocks();
}

