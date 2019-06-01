/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "init.h"
#include "lcd.h"
#include "delay.h"
#if defined TEM
#include "ds18b20.h"
#endif
#if defined DEBUG
#define DBG_BUF 255
char rx_dbg[DBG_BUF];
char tx_dbg[DBG_BUF];
uint16_t DbgCounter;
uint16_t cmd;
bool new_cmd = FALSE;
#endif
#if defined TEM
#endif
#if defined ADC
float averageVoltage = 0,temperature = 19;
uint16_t tdsValue = 0;
#endif

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
unsigned char bl_state;
unsigned char data_value;
void show_value(uint8_t line,uint16_t value);
void main(void)
{
    uint8_t i;
    unsigned char s = 0x00;
    //                    TDS: XXX PPM
  const char txt1[] = {"  TDS:      PPM "};
  const char txt2[] = {"   CLEAN WATER  "};
  const char txt3[] = {"   DIRTY WATER  "};
  const char txt4[] = {" !! NO WATER !! "};
  const char txt5[] = {"                "};
    //booting system
    booting();
    // DS18B20_init();
    LCD_init();
    LCD_clear_home();
    LCD_goto(0, 0);
    LCD_putstr(txt1);

#if defined DEBUG
    while(1)
    {
        if( new_cmd ){
            //the first 3 character is command code
            cmd = (rx_dbg[0]-0x30)*100 + (rx_dbg[1]-0x30)*10 + (rx_dbg[2]-0x30);
            switch (cmd)
            {
                //cmd format:   xxx xx
                case CMD_NEW_TEMPERATOR:
                    for(i=0;i<DbgCounter;i++)
                    {
                        //frontend should already valid the arguments!!!
                        if( (rx_dbg[i] == ' ') && (rx_dbg[i+1] != ' ') )
                        {
                            temperature = (rx_dbg[i+1]-0x30)*10 + (rx_dbg[i+2]-0x30);
                            tdsValue = read_tds();
                            tx_dbg[0] = (char)( tdsValue/10000 + 0x30);
                            tx_dbg[1] = (char)((tdsValue/1000)%10 + 0x30);
                            tx_dbg[2] = (char)((tdsValue/100)%10 + 0x30);
                            tx_dbg[3] = (char)((tdsValue/10)%10 + 0x30);
                            tx_dbg[4] = (char)( tdsValue%10 + 0x30);
                            for(i=0;i<5;i++)
                                print(tx_dbg[i]);
                            break;
                        }
                    }
                    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                break;
            }
            DbgCounter = 0;
            new_cmd = FALSE;
        }
    }
#endif
    while (1)
    {
#if defined ADC
        for(i=0;i<TDS_MEASURE_REPEAT;i++){
            // _delay_ms(500);
            tdsValue=read_tds();
            if(tdsValue > TDS_LIMIT){
                if(tdsValue > TDS_OVERLOAD)              
                {
                    LCD_goto(6, 0);
                    LCD_putchar('>');
                    show_value(0,TDS_OVERLOAD);
                }
                else
                {
                    show_value(0,tdsValue);
                }
                LCD_goto(0, 1);
                LCD_putstr(txt3);
                _delay_ms(500);
                LCD_goto(0, 1);
                LCD_putstr(txt5);
            }
            else if(tdsValue < 5){
                show_value(0,0);
                LCD_goto(0, 1);
                LCD_putstr(txt4);
                _delay_ms(500);
                LCD_goto(0, 1);
                LCD_putstr(txt5);
                // blink_led(TDS_GPIO_PIN,0);  //turn off tds led
                // GPIO_WriteBit(TDS_GPIO_PORT, TDS_GPIO_PIN, (BitAction)(0));
            }
            else
            {
                show_value(0,tdsValue);
                LCD_goto(0, 1);
                LCD_putstr(txt2);
                // blink_led(TDS_GPIO_PIN,0);
                // GPIO_WriteBit(TDS_GPIO_PORT, TDS_GPIO_PIN, (BitAction)(1));
            }
            
        }       
#endif
        // GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_6)));
    }
}

#if defined ADC
int getMedianNum(int* bArray) 
{
    int i, j, bTemp;
    int bTab[SCOUNT];
      for (i = 0; i<SCOUNT; i++)
	  bTab[i] = bArray[i];
      for (j = 0; j < SCOUNT - 1; j++) 
      {
	  for (i = 0; i < SCOUNT - j - 1; i++) 
          {
	    if (bTab[i] > bTab[i + 1]) 
            {
		bTemp = bTab[i];
	        bTab[i] = bTab[i + 1];
		bTab[i + 1] = bTemp;
	     }
	  }
      }
      if ((SCOUNT & 1) > 0)
	bTemp = bTab[(SCOUNT - 1) / 2];
      else
	bTemp = (bTab[SCOUNT / 2] + bTab[SCOUNT / 2 - 1]) / 2;
      return bTemp;
}
#endif

#if defined DEBUG
/**
  * @brief Retargets the C library printf function to the UART.
  * @param c Character to send
  * @retval char Character sent
  */
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  UART1_SendData8(c);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);

  return (c);
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval char Character to Read
  */
GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
  char c = 0;
#else
  int c = 0;
#endif
  /* Loop until the Read data register flag is SET */
  // while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
    c = UART1_ReceiveData8();
  return (c);
}
#endif

#if defined ADC
/**
  * @this function read adc value.
  */
uint16_t read_adc(void)
{
    ADC1_StartConversion();                                   // start conversion
    while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);                    // end of conversion
    ADC1_ClearFlag(ADC1_FLAG_EOC);
    return ADC1_GetConversionValue();                                      // return ADC value 
}
#endif

#if defined ADC
/**
  * @this function calculate tds value from the ADC red from read_adc() function.
  */
uint16_t read_tds(void){
    uint8_t analogBufferIndex;
    int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
    float compensationCoefficient;
    float compensationVolatge;
    float ppmf;
    // uint16_t voltage;

    for( analogBufferIndex=0; analogBufferIndex < SCOUNT; analogBufferIndex++)
    {
        analogBuffer[analogBufferIndex] = read_adc();
        _delay_ms(50);//delay 50ms
    }

    // read the analog value more stable by the median filtering
    // algorithm, and convert to voltage value
    averageVoltage = getMedianNum(analogBuffer) * (float)VREF / 1024.0;
    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    compensationCoefficient=1.0+0.02*(temperature-25.0);
    compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
    // voltage=(uint16_t)(compensationVolatge*100);
    // show_value(1,voltage);
    //convert voltage value to tds value
    //a.x^3 + b.x^2 + c.x +d
    // ppmf = (float)(14.467*compensationVolatge*compensationVolatge*compensationVolatge - 13.544*compensationVolatge*compensationVolatge + 320.05*compensationVolatge - 9.169);
    ppmf = (float)(4.4431*compensationVolatge*compensationVolatge*compensationVolatge*compensationVolatge - 
                   10.498*compensationVolatge*compensationVolatge*compensationVolatge +
                   33.389*compensationVolatge*compensationVolatge + 286.79*compensationVolatge
                   - 2.2933);
    if (ppmf <=0)
        return 0;
    else
        return (uint16_t)ppmf;
}
#endif

//line 0: tds value
//line 1: voltage value
void show_value(uint8_t line,uint16_t value)
{
    char ch = 0x00;
    ch = ((value / 1000) + 0x30);
    LCD_goto(7, line);
    LCD_putchar(ch);
    ch = (((value / 100) % 10) + 0x30);
    LCD_goto(8, line);
    LCD_putchar(ch);
    ch = (((value / 10) % 10) + 0x30);
    LCD_goto(9, line);
    LCD_putchar(ch);
    ch = ((value % 10) + 0x30);
    LCD_goto(10, line);
    LCD_putchar(ch);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
