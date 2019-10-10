/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/I2C_DataExchange/Slave/main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
//#include "stm8s_eval.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "stm8s_uart1.h"
#include "stm8s_exti.h"
/** @addtogroup I2C_TwoBoards
  * @{
  */

/** @addtogroup I2C_DataExchange
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
#define PUTCHAR_PROTOTYPE int putchar (int c)
///////////////// uart //////////////////
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  UART1_SendData8(c);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  return (c);
}
static void UART_Config(void)
{
  UART1_DeInit();
  UART1_Init(115200,
             UART1_WORDLENGTH_8D,       //8bit data
             UART1_STOPBITS_1,          //1 stop bit
             UART1_PARITY_NO,           //No Parity
             UART1_SYNCMODE_CLOCK_DISABLE,
             UART1_MODE_TX_ENABLE);   //TX & RX duplex mode
  UART1_Cmd(ENABLE);
}
//////////////////////// end ///////////////
void Delay(uint16_t nCount)
{
  while (nCount != 0)
  {
    nCount--;
  }
}
//char *buff;

void main(void)
{

  /* system_clock / 1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
      CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  UART_Config();
  printf("startdd\r\n");
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT/*GPIO_MODE_IN_PU_IT*/);   // SCL //ok
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);   // SDA //
  //GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT/*GPIO_MODE_IN_PU_IT*/);   // SCL //ok

  //EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_ONLY);
  //enableInterrupts();
  /*while(1){
    Delay(10);
  }*/
#if 1
  char buff[870] ;
  uint16_t countbyte = 0;
  uint8_t countbit  = 0;
  uint8_t stopBit = 1;
  uint8_t brise=0;
  uint8_t a;
  uint16_t i = 0;
  //uint8_t brun = 2;
  for(i = 0;i<1;i++){
    //brun=2;
    countbyte = 0;
    while(countbyte<870){
      if(GPIOD->IDR&GPIO_PIN_3){//scl
        if(brise == 0)continue;
        else brise = 0;
        a = GPIOD->IDR&GPIO_PIN_4;
        if(stopBit){
          stopBit--;
          continue;
        }
        if(countbit >= 8){
          countbit = 0;
          countbyte++;
          if((countbyte%6) == 0){
            stopBit = 1;
            /*if(((buff[countbyte-5]>>1)==0x1b)&& buff[countbyte-4]==0x3b && buff[countbyte-3]==0x6b && buff[countbyte-2]==0xbf && buff[countbyte-1]==0x25){
              if(brun>0)brun--;
            }
            if(((buff[countbyte-5]>>1)==0x19) || ((buff[countbyte-5]>>1)==0x15)){
              countbyte -= 6;
            }else if((buff[countbyte-5]>>1)==0x07){
              countbyte -= 6;
            }else if(brun){
              countbyte -= 6;
            }*/
            //if((buff[countbyte-5]>>1)==0x22 && (i == 1)){
            //  printf("aa\r\n");
              //printf("0x%02x-%x:0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\r\n",buff[countbyte-6],buff[countbyte-5]&0x01,buff[countbyte-5]>>1,buff[countbyte-4],buff[countbyte-3],buff[countbyte-2],buff[countbyte-1]);
            //}
            continue;
          }else continue;
        }
        if(a){
          buff[countbyte] = (buff[countbyte]<<1) | 0x01;
        }else{
          buff[countbyte] = (buff[countbyte]<<1) | 0x00;
        }
        countbit++;
      }else{
        brise = 1;
      }
    }
  }
#else
  extern uint16_t countbyte;
  while(countbyte<840){
  };
#endif
  printf("page:%d\r\n",i);
  for(i = 0;i<870;i+=6){
    printf("0x%02x-%x:0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\r\n",buff[i],buff[i+1]&0x01,buff[i+1]>>1,buff[i+2],buff[i+3],buff[i+4],buff[i+5]);
  }

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

