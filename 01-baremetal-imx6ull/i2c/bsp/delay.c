#include "MCIMX6Y2.h"

void delay(unsigned int n)
{
    while (n--);
}

void delay_us(unsigned int n)
{
    unsigned int counter = 0;
    unsigned int old_counter = 0;
    unsigned int new_counter = 0;

    old_counter = GPT1->CNT;
    while (1)
    {
        new_counter = GPT1->CNT;
        if (new_counter != old_counter)
        {
            if (new_counter > old_counter)
            {
                counter += new_counter - old_counter;
            }
            else  
            {
                counter += 0xFFFFFFFF - old_counter + new_counter;
            }
            if (counter >= n)
            {
                return;
            }
            old_counter = new_counter;
        }
    }
}

void delay_ms(unsigned int n)
{
    while (n--)
    {
        delay_us(1000);
    }
}