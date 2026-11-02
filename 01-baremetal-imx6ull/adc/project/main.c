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
#include "adc.h"
#include "spi.h"
#include "adxl345.h"

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
 //   adc1_channel1_init();
    spi3_channel0_init();
    adxl345_init();

    struct ADXL345_Data t;

    while (1)
    {
        t = adxl345_collect();
        int i, j, k;  
        i = t.xg * 10;  //31
        j = t.yg * 10;  //25
        k = t.zg * 10;  //31
        printf("x = %d.%d, y = %d.%d, z = %d.%d\n", i / 10, i % 10, j / 10, j % 10, k / 10, k % 10);
        delay_ms(200);
    }

    return 0;
}