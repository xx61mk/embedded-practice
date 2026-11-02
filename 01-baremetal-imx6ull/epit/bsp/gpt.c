#include "gpt.h"
#include "MCIMX6Y2.h"

void gpt1_init(void)
{
    GPT1->CR = 0;
    GPT1->CR |= (1 << 15);      // 复位GPT
    while ((GPT1->CR & (1 << 15)) != 0);    // 等待GPT复位完成
    GPT1->CR |= (1 << 9);       // GPT工作模式：Free-Run模式
    GPT1->CR |= (1 << 6);       // 时钟源：ipg_clk
    GPT1->PR = 65;              // 66分频
    GPT1->CR |= (1 << 0);       // 使能GPT
}

