#ifndef _ADC_H_
#define _ADC_H_

extern void adc1_channel1_init(void);
extern unsigned short adc1_get_origin_value(void);
extern float adc1_get_value(void);
extern void swap(unsigned short *a,unsigned short *b);
extern void sort(unsigned short *a, int len);
extern unsigned int sum_of_array(unsigned short *a, int len);

#endif