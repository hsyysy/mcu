/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "lptim.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c_bm.h"
#include "mcu.h"
#include "wdt.h"
#include "ds1307.h"
#include "eeprom.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief This function provides minimum delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
#define HAL_MAX_DELAY_BM      0x00007FFFU

void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a period to guaranty minimum wait */
//  if (wait < HAL_MAX_DELAY_BM)
//  {
//    wait++;
//  }

  while((HAL_GetTick() - tickstart) < wait)
  {
  }
}

void TurnOnLED(void);
void TurnOffLED(void);
void PowerON(void);
void PowerDOWN(void);
void BM1684_RST(void);
void Clean_ERR_INT(void);

#define PMIC_ADDR 0x3c

#define IO_MODECTRL			0x24
#define BUCK1_VOUTFBDIV		0x3B
#define BUCK1_DVS0CFG1		0x48
#define BUCK1_DVS0CFG0		0x49
#define BUCK1_DVSSEL		0x53
#define BUCK1_EN_DLY		0x56
#define BUCK2_VOUTFBDIV		0x58
#define BUCK2_DVS0CFG1		0x62
#define BUCK2_DVS0CFG0		0x63
#define BUCK3_VOUTFBDIV		0x72
#define BUCK3_DVS0CFG1		0x7C
#define BUCK3_DVS0CFG0		0x7D
#define BUCK4_VOUTFBDIV		0x8c
#define BUCK4_DVS0CFG1		0x96
#define BUCK4_DVS0CFG0 		0x97

uint8_t val;
void led_on(void);

void PowerON(void)
{
	HAL_GPIO_WritePin(GPIOH, EN_5V_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOH, EN_3P3_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	val = 0xE5;
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, IO_MODECTRL,1, &val, 1, 1000);// 1.2v
	val = 0;
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK1_VOUTFBDIV,1, &val, 1, 1000);// 1.2v
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK2_VOUTFBDIV,1, &val, 1, 1000);// 1.2v
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK3_VOUTFBDIV,1, &val, 1, 1000);// 1.2v
	val = 0xD0;
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK1_DVS0CFG1,1, &val, 2, 1000);// LDO1V_IN
	HAL_Delay(30);
	HAL_GPIO_WritePin(GPIOB, EN_VDDIO18_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN1_ISL68127_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDDIO33_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDD_PHY_Pin, GPIO_PIN_SET);//EN_PHY
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, P08_PWR_GOOD_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDD_PCIE_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO2_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN0_ISL68127_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO3_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	val = 0xE5;
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK2_DVS0CFG1,1, &val, 2, 1000);//DDR_VDDQ 1.1v
	HAL_Delay(1);
//	val = 0xE5;//1.8ms
//	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK3_DVS0CFG1,1, &val, 2, 1000);//DDR*_DDR_VDDQLP 1.1v
	val = 0x7D;//1.8ms
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK3_DVS0CFG1,1, &val, 2, 1000);//DDR*_DDR_VDDQLP 0.6v
	HAL_Delay(1);//5ms
	HAL_GPIO_WritePin(GPIOB, EN_VDD_TPU_MEM_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, EN_VDD_TPU_MEM_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO1_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VQPS18_Pin, GPIO_PIN_SET);
	HAL_Delay(30);
	HAL_GPIO_WritePin(SYS_RST_X_GPIO_Port, SYS_RST_X_Pin, GPIO_PIN_SET);
	HAL_Delay(30);
	HAL_GPIO_WritePin(GPIOA, DDR_PWR_GOOD_Pin, GPIO_PIN_SET);
	led_on();
}

void PowerDOWN(void)
{
	HAL_GPIO_WritePin(GPIOA, DDR_PWR_GOOD_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(SYS_RST_X_GPIO_Port, SYS_RST_X_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, EN_VQPS18_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO1_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDD_TPU_MEM_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO3_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN1_ISL68127_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDD_PCIE_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, P08_PWR_GOOD_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDD_PHY_Pin, GPIO_PIN_RESET);//EN_PHY
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDDIO33_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN0_ISL68127_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, EN_VDDIO18_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_I2C_Mem_Write(&hi2c2,PMIC_ADDR, BUCK1_DVS0CFG1,1, &val, 1, 1000);// LDO1V_IN
	HAL_Delay(40);
	HAL_GPIO_WritePin(GPIOB, EN_PMIC_Pin, GPIO_PIN_RESET);// EN_PMIC 1->0
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOH, EN_3P3_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOH, EN_5V_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void BM1684_RST(void)
{
	return ;
}
void Clean_ERR_INT(void)
{
	return ;
}

//CPLD Command
#define CMD_CPLD_PWR_ON			0x01       //1684 power on
#define CMD_CPLD_PWR_DOWN		0x02	   //1684 power down
#define CMD_CPLD_1684RST		0x03       //reset 1684
#define CMD_CPLD_SWRST			0x04	   //soft resetting
#define CMD_CPLD_CLR			0x05       // clean MCU_ERR_INT, set 0.
//BM1684 Command
#define CMD_BM1684_REBOOT		0x06       // power is always on
#define CMD_BM1684_RST			0x07       // power down

uint8_t pg_core = 0;

uint8_t sec_count = 0;

#define MCU_VERSION 0x01

#define VENDER_SA5	0x01
#define VENDER_SC5	0x02
#define VENDER_SE5	0x03


void led_filcker(void)
{
	HAL_GPIO_WritePin(GPIOA, MCU_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA, MCU_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
}

void led_on(void)
{
	HAL_GPIO_WritePin(GPIOA, MCU_LED_Pin, GPIO_PIN_SET);
}
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected to the EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);

  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
  switch (GPIO_Pin)
  {
  case GPIO_PIN_5:
	  // tempture too high
	  led_filcker();
	  i2c_regs.intr_status1 = 0x02;
	  break;
  case GPIO_PIN_14:
	  // reset 1684 pcie
	  break;
  case GPIO_PIN_15:
	  // power good
	  pg_core = 1;
	  break;
  }
}

//TODO: receive data from bm1684 or CPLD
__weak void i2c_cb(void)
{
	//set i2c_regs paramer
//	i2c_regs.cmd_reg = data;
}

uint8_t ADC_PCB_Ver0;
uint8_t ADC_PCB_Ver1;
uint8_t ADC_BOM_Ver0;
uint8_t ADC_BOM_Ver1;
uint32_t ADC_Buf[5];

/* 0\1  0   0.5   1   1.5
 * 0    0   4     8   12
 * 0.5  1   5     9   13
 * 1    2   6     10  14
 * 1.5  3   7     11  15
 */

uint8_t hw_version[4][4] = { \
		{0, 4, 8, 12}, \
		{1, 5, 9, 13}, \
		{2, 6, 10, 14}, \
		{3, 7, 11, 15}  \
};

void SET_HW_Ver(void)
{
	  uint8_t i;

	  for (i = 0; i < 5; i++) {
		  HAL_ADC_Start(&hadc);
		  HAL_ADC_PollForConversion(&hadc, 0XFFFF);

		  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_EOC)) {
			  ADC_Buf[i] = HAL_ADC_GetValue(&hadc);
		  }
	  }

	  //calculate voltage
	  ADC_PCB_Ver0 = (float)ADC_Buf[0] /4096 * 2;
	  ADC_PCB_Ver1 = (float)ADC_Buf[1] /4096 * 2;
	  ADC_BOM_Ver0 = (float)ADC_Buf[2] /4096 * 2;
	  ADC_BOM_Ver1 = (float)ADC_Buf[3] /4096 * 2;

	  i2c_regs.hw_ver = (hw_version[ADC_PCB_Ver0 * 2][ADC_PCB_Ver1 * 2] << 4) | hw_version[ADC_BOM_Ver0][ADC_BOM_Ver1];


	  HAL_ADC_Stop(&hadc);
}

void READ_Temper(void)
{
	if (sec_count == 1) {
		// detection of temperature value
		HAL_I2C_Mem_Read(&hi2c2,0x98, 0x0,1, &i2c_regs.temp1684, 1, 1000);
		HAL_I2C_Mem_Read(&hi2c2,0x4c, 0x0,1, &i2c_regs.temp_board, 1, 1000);
		if ((i2c_regs.temp1684 > 75) || (i2c_regs.temp_board > 70)) {//temperature too high alert
			led_filcker();
			i2c_regs.intr_status1 = BOARD_OVER_TEMP;
		} else if ((i2c_regs.temp1684 > 85) || (i2c_regs.temp_board > 75)) {//temperature too high, powerdown
			i2c_regs.intr_status1 = BM1684_OVER_TEMP;
			PowerDOWN();
			led_filcker();
		}
		sec_count = 0;
	} else {
		HAL_Delay(1);
		sec_count = 1;
	}
}

typedef struct factory_info_t {
	uint16_t manufacturer;
	uint16_t board_type;
	uint16_t date;
	uint16_t Serial_number;
	uint8_t  PCB_Ver;
	uint8_t  PCBA_Ver;
	uint8_t  sub_unit_Ver;
	uint16_t MAC_Addr;
}Factory_Info;

Factory_Info fty_Info;

void Factory_Info_Get(void)
{
//	  EEPROM_Write(addr, writeFlashData);
//	  EEPROM_ReadWords(addr_offset, &fty_Info.manufacturer, sizeof(Factory_Info));
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  i2c_regs.vender = VENDER_SA5;
  i2c_regs.sw_ver = MCU_VERSION;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_I2C2_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_LPTIM1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
//  Factory_Info_Get();
  i2c_ctx0 = (struct i2c_ctx*)malloc(sizeof(struct i2c_ctx));
  i2c_ctx3 = (struct i2c_ctx*)malloc(sizeof(struct i2c_ctx));
  memset(i2c_ctx0, 0, sizeof(struct i2c_ctx));
  memset(i2c_ctx3, 0, sizeof(struct i2c_ctx));

  i2c_init(hi2c1.Instance,i2c_ctx0);
  i2c_init(hi2c3.Instance,i2c_ctx3);
  ds1307_init();
  mcu_init(i2c_ctx0);
  mcu_init(i2c_ctx3);
  wdt_init();
  i2c_slave_start(i2c_ctx0);
  i2c_slave_start(i2c_ctx3);
  // make sure PB6 is high
//  HAL_GPIO_WritePin(GPIOB, EN_VDD_TPU_MEM_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, TPU_I2C_ADD3_Pin, GPIO_PIN_SET);
  // set PCB & BOM version by voltage value
  SET_HW_Ver();

  PowerON();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  // response CPLD's commands
	  switch(i2c_regs.cmd_reg) {
	  case CMD_CPLD_PWR_ON:
		  PowerON();
		  break;
	  case CMD_CPLD_PWR_DOWN:
		  PowerDOWN();
		  break;
	  case CMD_CPLD_1684RST:
		  BM1684_RST();
		  i2c_regs.rst_1684_times++;
		  break;
	  case CMD_CPLD_SWRST:
		  break;
	  case CMD_CPLD_CLR:
		  Clean_ERR_INT();
		  break;
	  case CMD_BM1684_REBOOT:
		  break;
	  case CMD_BM1684_RST:
		  BM1684_RST();
		  break;
	  }
//	  led_filcker();
	  // read temperature every 2 seconds
	  READ_Temper();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C3
                              |RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_LPTIM1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.LptimClockSelection = RCC_LPTIM1CLKSOURCE_LSI;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/