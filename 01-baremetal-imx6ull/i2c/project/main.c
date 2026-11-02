#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "core_ca7.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "interrupt.h"
#include "clock.h"
#include "epit.h"
#include "gpt.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "i2c.h"
#include "lm75.h"

int main(void)
{
    clock_init();
    system_interrupt_init();
    led_init();
    beep_init();
//   key_init(); 
//   epit1_init();
    gpt1_init();
    uart1_init();
    i2c1_init();

    while (1)
    {
        float f = get_lm75_temp();
        int k = f * 10;    // 61
        int m = k / 10;    // 6
        int n = k % 10;    // 1
        printf("%d.%d\n", m, n);
        delay_ms(500);
    }

    return 0;
}