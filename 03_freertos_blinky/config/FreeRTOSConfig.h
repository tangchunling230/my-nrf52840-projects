#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------
 * FreeRTOS 核心系统参数配置 (针对 nRF52840 Cortex-M4F 内核)
 * -------------------------------------------------------------------- */
#define configUSE_PREEMPTION                     1   // 启用抢占式调度
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1   // 启用硬件计算前导零，加速任务切换
#define configUSE_TICKLESS_IDLE                  0   // 低功耗休眠，学习阶段先关闭
#define configCPU_CLOCK_HZ                       64000000UL // nRF52840 主频 64MHz
#define configTICK_RATE_HZ                       1024       // 系统心跳频率 (1024Hz)
#define configMAX_PRIORITIES                     5          // 最大任务优先级设定为 5
#define configMINIMAL_STACK_SIZE                 64         // 最小空闲堆栈大小 (以字为单位，256字节)
#define configTOTAL_HEAP_SIZE                    10240      // 分配给 OS 堆内存的总大小 (10KB)
#define configMAX_TASK_NAME_LEN                  16         // 任务命名最大长度

/* --------------------------------------------------------------------
 * 软件定时器、互斥量及信号量支持 (必须开启以满足我们的实验)
 * -------------------------------------------------------------------- */
#define configUSE_MUTEXES                        1   // 🚨 必须设为 1，开启互斥锁功能！
#define configUSE_RECURSIVE_MUTEXES              1   // 开启递归互斥锁
#define configUSE_COUNTING_SEMAPHORES            1   // 开启计数信号量
#define configUSE_TIMERS                         1   // 开启软件定时器

/* --------------------------------------------------------------------
 * 协程与辅助功能
 * -------------------------------------------------------------------- */
#define configUSE_CO_ROUTINES                    0   // 关闭协程
#define configMAX_CO_ROUTINE_PRIORITIES          2

/* --------------------------------------------------------------------
 * 钩子函数 (Hooks) 配置 - 调测时很有用
 * -------------------------------------------------------------------- */
#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0
#define configUSE_MALLOC_FAILED_HOOK             0   // 内存分配失败钩子
#define configUSE_DAEMON_TASK_STARTUP_HOOK       0

/* --------------------------------------------------------------------
 * 任务运行状态及信息收集
 * -------------------------------------------------------------------- */
#define configGENERATE_RUN_TIME_STATS            0
#define configUSE_TRACE_FACILITY                 0
#define configUSE_STATS_FORMATTING_FUNCTIONS     0

/* --------------------------------------------------------------------
 * 核心 API 裁剪 (使能我们需要使用的函数)
 * -------------------------------------------------------------------- */
#define INCLUDE_vTaskPrioritySet                 1
#define INCLUDE_uxTaskPriorityGet                1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskCleanUpResources            0
#define INCLUDE_vTaskSuspend                     1   // 🚨 必须设为 1，vTaskDelay 会用到挂起
#define INCLUDE_vTaskDelayUntil                  1
#define INCLUDE_vTaskDelay                       1   // 🚨 必须设为 1，使能 vTaskDelay 延时功能

/* --------------------------------------------------------------------
 * 中断优先级配置 (针对 Cortex-M4 硬件中断)
 * -------------------------------------------------------------------- */
#define configKERNEL_INTERRUPT_PRIORITY          255 // 内核中断优先级（最低）
#define configMAX_SYSCALL_INTERRUPT_PRIORITY     91  // 允许调用 OS API 的最高硬件中断优先级

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_CONFIG_H */