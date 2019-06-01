#include "stm8s.h"
#include "init.h"

__IO uint32_t Timingdelay = 0;
/*-----------------------------------------------------------------*/
/*
 * System initialization
 */
void booting(void){
    //booting system
    CLK_Config();   /* Clock configuration  */
    GPIO_Config();  /* GPIO configuration   */
    ADC_Config();   /* ADC configuration    */
    // TIM4_Config();  /* TIM4 configuration   */
#if defined DEBUG
    UART1_Config(); /* UART1 configuration  */
#endif
}



/**
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
  */
void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
}


#if defined DEBUG
/**
  * @brief  Configure UART1 for the synchronous mode communication
  * @param  None
  * @retval None
  */
void UART1_Config(void)
{
  /* UART1 configured as follow:
          - Word Length = 8 Bits
          - 1 Stop Bit
          - No parity
          - BaudRate = 9600 baud
          - UART1 Clock enabled
          - Polarity Low
          - Phase Middle
          - Last Bit enabled
          - Receive and transmit enabled
   */
  UART1_DeInit();
  // UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, 
              // (UART1_SyncMode_TypeDef)(UART1_SYNCMODE_CLOCK_ENABLE | UART1_SYNCMODE_CPOL_LOW |UART1_SYNCMODE_CPHA_MIDDLE |UART1_SYNCMODE_LASTBIT_ENABLE),
              // UART1_MODE_TXRX_ENABLE);
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  UART1_Cmd(ENABLE);
}
#endif
/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
// void TIM4_Config(void)
// {
//   /* TIM4 configuration:
//    - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
//    clock used is 16 MHz / 64 = 250000 Hz
//   - With 125 000 Hz we can generate time base:
//       max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 250000 = 1.024 ms = 1024 us
//       min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 250000 = 0.008 ms = 8us
//   - In this example we need to generate a time base equal to 10 us
//    so TIM4_PERIOD = (0.000020 * 250000 - 1) = 4 */

//   /* Time base configuration */
//   TIM4_TimeBaseInit(TIM4_PRESCALER_64, TIM4_PERIOD);
//   /* Clear TIM4 update flag */
//   TIM4_ClearFlag(TIM4_FLAG_UPDATE);
//   /* Enable update interrupt */
//   TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
//   /* enable interrupts */
//   enableInterrupts();

//   /* Enable TIM4 */
//   TIM4_Cmd(ENABLE);
// }

/**
  * @brief  Configure GPIO for LEDs available on the evaluation board
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
  /* Initialize I/Os in Output Mode */
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_DeInit(GPIOB);
#if defined TEM
  GPIO_DeInit(GPIOD);
#endif
}

/**
  * @brief  Configure ADC2 Continuous Conversion with End Of Conversion interrupt 
  *         enabled .
  * @param  None
  * @retval None
  */
void ADC_Config()
{
  /*  Init GPIO for ADC1 */
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
  
  /* De-Init ADC peripheral*/
  ADC1_DeInit();

  /* Init ADC1 peripheral */
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3,\
            DISABLE);

  /* Enable EOC interrupt */
  // ADC1_ITConfi-------g(ADC1_IT_EOCIE,ENABLE);

  /* Enable general interrupts */  
  // enableInterrupts();
  
  /*Start Conversion */
  // ADC1_StartConversion();
}
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
// void _delay_ms(__IO uint32_t nTime)
// {
//   Timingdelay = nTime * 1000;

//   while (Timingdelay != 0);
// }

// void delay_us(__IO uint32_t nTime)
// {
//   Timingdelay = nTime;

//   while (Timingdelay != 0);
// }
/**
  * @brief  Decrements the Timingdelay variable.
  * @param  None
  * @retval None
  */
// void TimingDelay_Decrement(void)
// {
//   if (Timingdelay != 0x00)
//   {
//     Timingdelay-=1;
//   }
// }

