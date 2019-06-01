/**
  ******************************************************************************
  * @file     stm8s_it.h
  * @author   MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief    This file contains the headers of the interrupt handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_IT_H
#define __STM8S_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "init.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef _COSMIC_
 void _stext(void); /* RESET startup routine */
 INTERRUPT void NonHandledInterrupt(void);
#endif /* _COSMIC_ */

#ifndef _RAISONANCE_
//  INTERRUPT void TRAP_IRQHandler(void); /* TRAP */
//  INTERRUPT void TLI_IRQHandler(void); /* TLI */
//  INTERRUPT void AWU_IRQHandler(void); /* AWU */
//  INTERRUPT void CLK_IRQHandler(void); /* CLOCK */
//  INTERRUPT void EXTI_PORTA_IRQHandler(void); /* EXTI PORTA */
//  INTERRUPT void EXTI_PORTB_IRQHandler(void); /* EXTI PORTB */
//  INTERRUPT void EXTI_PORTC_IRQHandler(void); /* EXTI PORTC */
//  INTERRUPT void EXTI_PORTD_IRQHandler(void); /* EXTI PORTD */
//  INTERRUPT void EXTI_PORTE_IRQHandler(void); /* EXTI PORTE */
//  INTERRUPT void SPI_IRQHandler(void); /* SPI */
//  INTERRUPT void TIM1_CAP_COM_IRQHandler(void); /* TIM1 CAP/COM */
//  INTERRUPT void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void); /* TIM1 UPD/OVF/TRG/BRK */
//  INTERRUPT void TIM2_UPD_OVF_BRK_IRQHandler(void); /* TIM2 UPD/OVF/BRK */
//  INTERRUPT void TIM2_CAP_COM_IRQHandler(void); /* TIM2 CAP/COM */
//  INTERRUPT void UART1_TX_IRQHandler(void); /* UART1 TX */
//  INTERRUPT void UART1_RX_IRQHandler(void); /* UART1 RX */
//  INTERRUPT void I2C_IRQHandler(void); /* I2C */
@far @interrupt void ADC1_IRQHandler(void); /* ADC1 */
// @far @interrupt void TIM4_UPD_OVF_IRQHandler(void); /* TIM4 UPD/OVF */
//  INTERRUPT void EEPROM_EEC_IRQHandler(void); /* EEPROM ECC CORRECTION */
#endif /* _RAISONANCE_ */

#endif /* __STM8S_IT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
