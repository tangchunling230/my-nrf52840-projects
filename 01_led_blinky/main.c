#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define MY_LED_PIN    NRF_GPIO_PIN_MAP(1,15) //定义LED引脚
int main(void)
{
    nrf_gpio_cfg_output(MY_LED_PIN);//配置LED引脚为输出模式
    nrf_gpio_pin_set(MY_LED_PIN);//LED初始状态为高电平，熄灭
    while(true)
    {
        nrf_gpio_pin_toggle(MY_LED_PIN);//翻转
        nrf_delay_ms(500);//延时500ms
    }
}