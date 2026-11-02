#include "adc.h"
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "stdio.h"


int adc1_calibration(void)
{
    // 校准
    ADC1->GC |= (1 << 7);
    while ((ADC1->GC & (1 << 7)) != 0);

    if ((ADC1->GS & (1 << 1)) == 0)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
} 

void adc1_channel1_init(void)
{
    // 配置引脚复用功能
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO01_GPIO1_IO01, 0);
    // 配置电气属性
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO01_GPIO1_IO01, IOMUXC_SW_PAD_CTL_PAD_PKE(1));
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO01_GPIO1_IO01, IOMUXC_SW_PAD_CTL_PAD_PUE(0));

    ADC1->HC[0] &= ~(1 << 7);
    ADC1->CFG = 0;
    ADC1->CFG |= (1 << 3) | (3 << 0);
    ADC1->GC = 0;
    ADC1->GC |= (1 << 0);

    // 校准
    printf(adc1_calibration() ? "calibration completed normally!\n" : "calibration completed failed!\n");
}

unsigned short adc1_get_origin_value(void)
{
    ADC1->HC[0] = 0;
    ADC1->HC[0] = 1;
    while ((ADC1->HS & (1 << 0)) == 0);

    return ADC1->R[0] & 0xFFF;      
}

void swap(unsigned short *a,unsigned short *b)
{
    unsigned short t =*a;
    *a=*b;
    *b=t;
}

void sort(unsigned short *a, int len)
{
    int i, j;
    for(i = 0; i < len - 1; i++)
    {
        for(j = 0; j < len - 1 - i; j++)
        {
            if(a[j] > a[j+1])
            {
                swap(&a[j], &a[j+1]);
            }
        }
    }
}

unsigned int sum_of_array(unsigned short *a, int len)
{
    unsigned int sum = 0;
    int i = 0;
    for (i = 0; i < len; i++)
    {
        sum += a[i];
    }

    return sum;
}

//读取ADC电压值
float adc1_get_value(void)
{
    unsigned short t[100];
    int i;
    for (i = 0; i < 100; i++)
    {
        t[i] = adc1_get_origin_value(); 
    }
    sort(t, sizeof(t) / sizeof(t[0])); 
    
    return sum_of_array(t+10, 80) / 80 / 4095.0 * 3.3;
}
