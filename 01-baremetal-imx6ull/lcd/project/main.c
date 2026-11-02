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
#include "framebuffer.h"
#include "lcd.h"
#include "pwm.h"
#include "gt1148.h"

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
    i2c2_init();
 //   adc1_channel1_init();
    spi3_channel0_init();
    adxl345_init();
    lcd_init();
    lcd_clear_screen(0x00FFFFFF);
    //pwm1_init();
    gt1148_init();
    int i = 0;
    char s[32]={0};


    while (1)
    {
/*
        float f = get_lm75_temp();
        int k = f * 10;
        int m = k / 10;
        int n = k % 10;
        sprintf(buffer, "temp:%d.%d", m, n);
        lcd_show_string(400, 240, 16 * strlen(buffer), 32, 32, buffer);

        delay_ms(100);
        global_ratio -= 0.05;
        if (global_ratio < 0)
        {
            global_ratio = 1.0;
        }
        set_ratio();
*/
        if(gt1148_dev.available !=0)
        {
            printf("touch_points num =%d\n",gt1148_dev.num);

            for(i=0;i<gt1148_dev.num;i++)
            {
                sprintf(s,"x=%d,y=%d    ",gt1148_dev.x[i],gt1148_dev.y[i]);
                lcd_show_string(100, 100 + 32 * i, 16 * strlen(s), 32, 32, s);
            }
        }
    }

    return 0;
}