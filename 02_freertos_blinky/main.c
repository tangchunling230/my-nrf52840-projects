#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
/*FreeRTOS 核心头文件*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/*基于 Nano 7221 07BB(nRF52840)的板载 LED 定义，后面是基于Utra BT 712的板载 LED*/
#define redLED      NRF_GPIO_PIN_MAP(1,15)  // 红灯：Port 1, Pin 15
#define greenLED    NRF_GPIO_PIN_MAP(1,13) // 绿灯：Port 1, Pin 13

SemaphoreHandle_t xkey;//声明一个全局变量xkey，它的类型是 SemaphoreHandle_t（信号量句柄）。它在程序中代表了那把唯一的“钥匙”，因为需要在红灯和绿灯两个不同的函数里去申请它，所以必须定义为全局变量。
static void redLEDtask(void*pvParameters)
{
    (void)pvParameters;

    nrf_gpio_cfg_output(redLED);//设置为输出模式
    nrf_gpio_pin_set(redLED);//初始化，初始为高电平，LED灯熄灭

    for(;;)//无限循环
    {
        if(xSemaphoreTake(xkey,portMAX_DELAY)==pdTRUE)
        {
        nrf_gpio_pin_clear(redLED);//LED灯亮
        vTaskDelay(pdMS_TO_TICKS(500));
        nrf_gpio_pin_set(redLED);
        xSemaphoreGive(xkey);
        }
    vTaskDelay(pdMS_TO_TICKS(50));
    }
}
static void greenLEDTask(void*pvParameters)
{
    (void)pvParameters;

    nrf_gpio_cfg_output(greenLED);
    nrf_gpio_pin_set(greenLED);

    for(;;)
    {
        if(xSemaphoreTake(xkey,portMAX_DELAY)==pdTRUE)
        {
            nrf_gpio_pin_clear(greenLED);
            vTaskDelay(pdMS_TO_TICKS(500));

            nrf_gpio_pin_set(greenLED);
            xSemaphoreGive(xkey);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

int hal_cpu_set_vcore(uint32_t voltage) {
#define HIGH_VOLTAGE_MODE 1

  switch(voltage) {
    case 1800:
      voltage = UICR_REGOUT0_VOUT_1V8;//UICR_REGOUT0_VOUT_1V8 是 Nordic 官方 SDK 为 nRF52840 芯片定义好的一个硬件常量（暗号）。用一句话概括它的物理意义：它是芯片出厂自带的默认配置，代表命令内部高压稳压器（REG0）输出 1.8V 的工作电压。
      break;
    case 2100:
      voltage = UICR_REGOUT0_VOUT_2V1;
      break;
    case 2400:
      voltage = UICR_REGOUT0_VOUT_2V4;
      break;
    case 2700:
      voltage = UICR_REGOUT0_VOUT_2V7;
      break;
    case 3000:
      voltage = UICR_REGOUT0_VOUT_3V0;
      break;
    case 3300:
      voltage = UICR_REGOUT0_VOUT_3V3;
      break;
    default:
      voltage = UICR_REGOUT0_VOUT_DEFAULT;
      break;
  }

  if(NRF_POWER->MAINREGSTATUS == HIGH_VOLTAGE_MODE) {
    if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) == (UICR_REGOUT0_VOUT_DEFAULT << UICR_REGOUT0_VOUT_Pos)) {
      NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
      while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

      NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
                          (voltage << UICR_REGOUT0_VOUT_Pos);

      NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
      while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

      // System reset is needed to update UICR registers.
      NVIC_SystemReset();
    }
  } else {  // normal voltage mode in 52840
      // nothing to do
  }

  return 0;
}

int main(void)
{
    hal_cpu_set_vcore(3000);//配置VCORE电压的函数，voltage的单位是 mV
    xkey = xSemaphoreCreateMutex();
    if(xkey != NULL)
    {
        xTaskCreate(redLEDtask, "RED", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(greenLEDTask, "GREEN", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        vTaskStartScheduler();
    }
    while(true) {}
}