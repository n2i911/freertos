/* Kernel includes. */

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Demo application includes. */

#include "partest.h"
#include "flash.h"
#include "flop.h"
#include "integer.h"
#include "PollQ.h"
#include "semtest.h"
#include "dynamic.h"
#include "BlockQ.h"
#include "blocktim.h"
#include "countsem.h"
#include "GenQTest.h"
#include "recmutex.h"
#include "death.h"
#include "serial.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

/* Application includes */
#include "userbtn.h"
#include "i2c.h"
#include "shell.h"
#include "spi.h"
#include "lis302dl.h"
#include "mco.h"
#include "pwm.h"

/* Priorities for the demo application tasks. */
#define mainFLASH_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1UL )
#define mainQUEUE_POLL_PRIORITY             ( tskIDLE_PRIORITY + 2UL )
#define mainSEM_TEST_PRIORITY               ( tskIDLE_PRIORITY + 1UL )
#define mainBLOCK_Q_PRIORITY                ( tskIDLE_PRIORITY + 2UL )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3UL )
#define mainFLOP_TASK_PRIORITY              ( tskIDLE_PRIORITY )
#define mainCOM_TASK_PRIORITY               ( tskIDLE_PRIORITY + 2UL )

static void prvSetupHardware( void )
{
    /* Setup STM32 system (clock, PLL and Flash configuration) */
    // SystemInit();

    /* Ensure all priority bits are assigned as preemption priority bits. */
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

    /* Setup the LED outputs. */
    vParTestInitialise();

    /* Configure the button input.  This configures the interrupt to use the
     *     lowest interrupt priority, so it is ok to use the ISR safe FreeRTOS API
     *         from the button interrupt handler. */
    STM_EVAL_PBInit( BUTTON_USER, BUTTON_MODE_EXTI );

    /* Setup the USART2 */
#define comBUFFER_LEN   ( 64 + 1 )
    xSerialPortInitMinimal( 115200UL, comBUFFER_LEN );

    /* Setup the User button function */
    vUserBtnInit();

    /* Setup the I2C2 */
    vI2cInit();

    /* Setup the SPI1 */
    vSpiInit();

    /* Setup the lis302dl */
    vLis302dlInit();

    /* Setup MCO1 & MCO2 */
    vMcoInit();

    /* Setup the PWM, this is for servo control */
    vPwmInit( PWM_PERIOD, PWM_PRESCALER );
}

static xComPortHandle xPort = NULL;
void prvUSARTTask( void *pvParameters )
{
signed char cByteRxed;
TickType_t xLastWakeTime;
const TickType_t xFrequency = 30;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for( ;; )
    {
#define comRX_BLOCK_TIME    ( ( TickType_t ) 0xffff )
#define comNO_BLOCK         ( ( TickType_t ) 0 )
        if( xSerialGetChar( xPort, &cByteRxed, comRX_BLOCK_TIME ) )
        {
            if( xSerialPutChar( xPort, cByteRxed, comNO_BLOCK ) == pdPASS )
            {
            }
        }
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}

int main(void)
{
    /* Configure the hardware ready to run the test. */
    prvSetupHardware();

    /* Start standard demo/test application flash tasks.  See the comments at
       the top of this file.  The LED flash tasks are always created.  The other
       tasks are only created if mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to
       0 (at the top of this file).  See the comments at the top of this file for
       more information. */
    vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );

    /* Start shell application. Would handle and response what user type */
    vStartShellTasks( mainCOM_TASK_PRIORITY );

#define comSTACK_SIZE   128
#if 0
    /* Start COM Rx/TX echo tasks */
    xTaskCreate( prvUSARTTask, "COMRxTx", comSTACK_SIZE, NULL, mainCOM_TASK_PRIORITY, ( TaskHandle_t * ) NULL );
#endif

    /* Start the scheduler. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following line
       will never be reached.  If the following line does execute, then there was
       insufficient FreeRTOS heap memory available for the idle and/or timer tasks
       to be created.  See the memory management section on the FreeRTOS web site
       for more details. */
    for( ;; );
}

void vApplicationTickHook( void )
{

}

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
       to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
       task.  It is essential that code added to this hook function never attempts
       to block in any way (for example, call xQueueReceive() with a block time
       specified, or call vTaskDelay()).  If the application makes use of the
       vTaskDelete() API function (as this demo application does) then it is also
       important that vApplicationIdleHook() is permitted to return to its calling
       function, because it is the responsibility of the idle task to clean up
       memory allocated by the kernel to any task that has since been deleted. */
}

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
       configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
       function that will get called if a call to pvPortMalloc() fails.
       pvPortMalloc() is called internally by the kernel whenever a task, queue,
       timer or semaphore is created.  It is also called by various parts of the
       demo application.  If heap_1.c or heap_2.c are used, then the size of the
       heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
       FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
       to query the size of free heap space that remains (although it does not
       provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
       configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
       function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
