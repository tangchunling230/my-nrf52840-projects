#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define MY_LED_PIN    NRF_GPIO_PIN_MAP(0,2) //定义LED引脚：P1.15
int main(void)
{
    nrf_gpio_cfg_output(MY_LED_PIN); // 1. 将 P0.13 引脚配置为输出模式
    nrf_gpio_pin_set(MY_LED_PIN);// 2. 默认让引脚输出高电平（在官方 DK 板上，高电平会使 LED 熄灭）
    while(true)
    {
        nrf_gpio_pin_toggle(MY_LED_PIN);//翻转
        nrf_delay_ms(500);//延时500ms
    }
}