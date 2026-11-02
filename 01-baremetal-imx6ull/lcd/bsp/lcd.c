#include "lcd.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "gpio.h"
#include "delay.h"

struct tftlcd_t lcd_dev;

// lcd引脚初始化
void lcd_pin_init(void)
{
    // 1. 设置lcd引脚复用功能
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0);         // pclk
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);     // HSYNC
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);     // VSYNC
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);   // DE
//  IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0);     // blt_pwm

    // 2. 设置lcd引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0xB9);
//    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0xB9);

    // 3. 配置pwm引脚
/*
    GPIO_Type_t t = 
    {
        .direction = gpio_output,
        .default_value = 1
    };
    gpio_init(GPIO1, 8, &t);
    gpio_set_value(GPIO1, 8, 1);
*/
}

// lcd时钟初始化
void lcd_clock_init(void)
{
    // BYPASS
    CCM_ANALOG->PLL_VIDEO |= (1 << 16);
    // Powerdown PLL
    CCM_ANALOG->PLL_VIDEO |= (1 << 12);
    CCM_ANALOG->PLL_VIDEO_NUM = 0;
    CCM_ANALOG->PLL_VIDEO_DENOM = 1;
    // 设置42倍频
    CCM_ANALOG->PLL_VIDEO &= ~(0x7F << 0); 
    CCM_ANALOG->PLL_VIDEO |= (42 << 0);    
    // 设置1分频
    CCM_ANALOG->PLL_VIDEO &= ~(3 << 19);  
    CCM_ANALOG->PLL_VIDEO |= (1 << 20);
    // poweron PLL
    CCM_ANALOG->PLL_VIDEO &= ~(1 << 12);
    // enable PLL5 output
    CCM_ANALOG->PLL_VIDEO |= (1 << 13);
    // 轮询等待时钟频率稳定
    while ((CCM_ANALOG->PLL_VIDEO & (1 << 31)) == 0);
    // 切回 PLL5
    CCM_ANALOG->PLL_VIDEO &= ~(1 << 16);

    // 选通门 pll5 + 4分频
    unsigned int t = CCM->CSCDR2;
    t &= ~(7 << 15);
    t |= (1 << 16);  
    t &= ~(7 << 12);
    t |= (3 << 12);
    t &= ~(7 << 9);
    CCM->CSCDR2 = t;

    // 8分频
    CCM->CBCMR |= (7 << 23);
}

// lcd复位
void lcd_reset(void)
{
    // 复位lcd
    LCDIF->CTRL |= (1 << 31);
    // 延时20ms
    delay_ms(20);
    // 清零
    LCDIF->CTRL &= ~(3 << 30);
}

// 清屏
void lcd_clear_screen(unsigned int col)
{
    unsigned int (*p)[800] = (unsigned int (*)[800])lcd_dev.frame_buffer;

    int i = 0;
    int j = 0;

    for (j = 0; j < lcd_dev.height; j++)   
    {
        for (i = 0; i < lcd_dev.width; i++) 
        {
            p[j][i] = col;
        }
    }
}

// 画像素点
void lcd_drawpoint(int x, int y, unsigned int col)
{
    unsigned int *p = (unsigned int *)lcd_dev.frame_buffer;

    if (x > lcd_dev.width || y > lcd_dev.height)
    {
        return;
    }

    *(p + lcd_dev.width * y + x) = col;
}

// lcd初始化
void lcd_init(void)
{
    lcd_pin_init();
    lcd_clock_init();
    lcd_reset();

    // lcd参数配置
    lcd_dev.width = 800;
    lcd_dev.height = 480;
    lcd_dev.hspw = 48;
    lcd_dev.hfp = 40;
    lcd_dev.hbp = 88;
    lcd_dev.vspw = 3;
    lcd_dev.vfp = 13;
    lcd_dev.vbp = 32;
    lcd_dev.pix_size = 4;
    lcd_dev.frame_buffer = LCD_FRAMEBUF_ADDR;
    lcd_dev.fore_color = 0x00FF0000;
    lcd_dev.back_color = 0x00FFFFFF;

    // LCD寄存器
    LCDIF->CTRL |= (1 << 19) | (1 << 17) | (3 << 10) | (3 << 8) | (1 << 5);
    LCDIF->CTRL1 = (7 << 16);
    LCDIF->TRANSFER_COUNT = (lcd_dev.height << 16) | (lcd_dev.width << 0);
    LCDIF->CUR_BUF = lcd_dev.frame_buffer;
    LCDIF->NEXT_BUF = lcd_dev.frame_buffer;
    LCDIF->VDCTRL0 |= (1 << 28) | (1 << 24) | (1 << 21) | (1 << 20) | (lcd_dev.vspw << 0);
    LCDIF->VDCTRL1 = (lcd_dev.height + lcd_dev.vfp + lcd_dev.vbp + lcd_dev.vspw);
    LCDIF->VDCTRL2 = (lcd_dev.hspw << 18) | (lcd_dev.width + lcd_dev.hfp + lcd_dev.hbp + lcd_dev.hspw);
    LCDIF->VDCTRL3 = ((lcd_dev.hspw + lcd_dev.hbp) << 16) | ((lcd_dev.vspw + lcd_dev.vbp) << 0);
    LCDIF->VDCTRL4 |= (1 << 18) | (lcd_dev.width << 0);
    LCDIF->CTRL |= (1 << 0);
}