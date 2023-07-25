/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  aycetheeeng
  * @version V2.3.0
  * @date    25-07-2023
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

unsigned long temp;

#define SPI_SCK_PORT									GPIOC
#define SPI_SCK_PIN										GPIO_PIN_5
#define SPI_MISO_PORT									GPIOC
#define SPI_MISO_PIN									GPIO_PIN_7
#define SPI_CS_PORT										GPIOC
#define SPI_CS_PIN										GPIO_PIN_6

#define SPI_SCK_PIN_init()						GPIO_Init(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST)
#define SPI_MISO_PIN_init()						GPIO_Init(SPI_MISO_PORT, SPI_MISO_PIN, GPIO_MODE_IN_PU_NO_IT)
#define SPI_CS_PIN_init()							GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW)
#define SPI_CS_ON()										GPIO_WriteLow(SPI_CS_PORT,SPI_CS_PIN)  // Chip_select_on
#define SPI_CS_OFF()									GPIO_WriteHigh(SPI_CS_PORT,SPI_CS_PIN)  // Chip_select_off


void SPIDeInit(void)
{
  SPI_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
	SPI_SCK_PIN_init();
  SPI_MISO_PIN_init();
  SPI_CS_PIN_init();

 }


void SPIInit(void)
{
 
	SPI_SCK_PIN_init();
  SPI_MISO_PIN_init();
  SPI_CS_PIN_init();
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_256, SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX,
           SPI_NSS_SOFT, 0x07);
					 		 
}

	
unsigned char SPI1_ReadByte(void)
{
  unsigned char Data = 0;
	unsigned char wait=0;

  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET)
  { 
		wait++;
		if(wait>200)
		{
			return(1);
		}
  }

  SPI_SendData(0);
	
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
  {
		wait++;
		if(wait>200)
		{
			return(1);
		}
  }

  Data = SPI_ReceiveData();
 	
	return Data;
} 

void spi_init(void)
{
	SPIDeInit();
	SPIInit();
	
}

unsigned long sensor;

unsigned long thermocouplevalues(void)
{
	unsigned char Data[4];
  
	
  SPI_CS_ON(); 
	SPI_Cmd(ENABLE);
	
	Data[0]=SPI1_ReadByte();
	Data[1]=SPI1_ReadByte();
	Data[2]=SPI1_ReadByte();
	Data[3]=SPI1_ReadByte();
	
	
	sensor=Data[0];
	sensor=sensor<<8;
	sensor=sensor|Data[1];
	sensor=sensor<<8;
	sensor=sensor|Data[2];
	sensor=sensor<<8;
	sensor=sensor|Data[3];
	
	
		temp=sensor>>20;
		(temp & 0x00000000000000000000000000000FFF);
		
		
	SPI_Cmd(DISABLE);					
	SPI_CS_OFF();
}



int main(){
	
spi_init();

while(1){

sensor = thermocouplevalues();

}

return 0;

}



#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
