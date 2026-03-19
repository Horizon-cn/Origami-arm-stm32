/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define TIM4_CCR3_OFFSET 0x3C
#define TIM4_CCR4_OFFSET 0x40
#define TIM4_ADDR 0x40000800

#define NUM_OF_CHANNELS 1
#define REPORT_FREQ_HZ 50
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
HAL_StatusTypeDef a;
int read_data = 0;

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bno055_t bno;
error_bno err;
volatile uint8_t time;
int initial_angle[NUM_OF_CHANNELS];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim ==&htim3){
		time = 1;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t buf[12];
  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint32_t *CCR3 = (uint32_t*)(TIM4_ADDR + TIM4_CCR3_OFFSET);
  *CCR3 = 25;
  uint32_t *CCR4 = (uint32_t*)(TIM4_ADDR + TIM4_CCR4_OFFSET);
  *CCR4 = 25;

  // i2c_mux_t mux = {
  //   .hi2c = &hi2c1,
  //   .addr_offset = 0,
  //   .rst_port = NULL,
  //   .rst_pin = 0
  // };

  // init_BNO(&hi2c1, REPORT_FREQ_HZ, &mux, NUM_OF_CHANNELS);

  // for(int i = 0;i<NUM_OF_CHANNELS;i++){
  // 	 read_init_angle(&hi2c1,initial_angle,i);
  // }

  // printf("Initial angles: %d\r\n", initial_angle[0]);

//    if (BNO_Init() != HAL_OK) {
//        printf("BNO085 initialization failed!\r\n");
//        while(1){
//        	printf("BNO085 initialization failed!\r\n");
//        };
//    }
//    printf("BNO085 initialization successful, starting data collection\r\n\r\n");
//
//    HAL_Delay(1000);

    //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

  TCA9548A_SetChannel(0);
  read_data = TCA9548A_Init();


  if (init_BNO085() != HAL_OK) {
        printf("BNO085 初始化失败！\r\n");
        while(1){
        	printf("failed\r\n");
        }
    }
    printf("BNO085 初始化完成，开始采集数据\r\n\r\n");
  

  HAL_Delay(1000);

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

  // bno = (bno055_t){
  //     .i2c = &hi2c1, .addr = BNO_ADDR, .mode = BNO_MODE_IMU,  ._temp_unit = 0,
  // };
  // HAL_Delay(5000);

  // if ((err = bno055_init(&bno)) == BNO_OK) {
	//   printf("[+] BNO055 init success\r\n");
	//   HAL_Delay(100);
  // } else {
	//   printf("[!] BNO055 init failed\r\n");
	//   printf("%s\n", bno055_err_str(err));
	//   Error_Handler();
  // }
  // HAL_Delay(100);

  // err = bno055_set_unit(&bno, BNO_TEMP_UNIT_C, BNO_GYR_UNIT_DPS,
  //                       BNO_ACC_UNITSEL_M_S2, BNO_EUL_UNIT_DEG);
  // if (err != BNO_OK) {
  //     printf("[BNO] Failed to set units. Err: %d\r\n", err);
  // } else {
  //     printf("[BNO] Unit selection success\r\n");
  // }

  // HAL_Delay(1000);

  // bno055_vec3_t gyr;
  // bno055_vec3_t acc;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Start_IT(&htim3);

  printf("while begins\r\n");
  
  while (1)
  {
	  //strcpy((char*)buf, "Hello\r\n");
	  //HAL_UART_Transmit(&hlpuart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//	  ret = HAL_I2C_Master_Transmit(&hi2c1, tca, &sel, 1, 100);
//	  if(ret != HAL_OK){
//		  strcpy((char*)buf, "Error Tx\r\n");
//	  }else{
//		  ret = HAL_I2C_Master_Receive(&hi2c1, tca, &sel, 1, 100);
//	  }

	  // bno.gyro(&bno, &gyr);
	  // bno.acc(&bno, &acc);
    //   printf("[+] GYR - x: %+2.2f | y: %+2.2f | z: %+2.2f\r\n", gyr.x, gyr.y, gyr.z);
	  // printf("[+] ACC - x: %+2.2f | y: %+2.2f | z: %+2.2f\r\n", acc.x, acc.y, acc.z);

    BNO_Accelerometer_t accel = {0};
        
    if (read_accelerometer_data(&accel) == HAL_OK) {
        printf("X: %.2f, Y: %.2f, Z: %.2f (m/s²)\r\n",
                accel.X, accel.Y, accel.Z);
    } else {
        printf("Failed to read accelerometer data\r\n");
    }

	while(time == 0){

	}
	time = 0;
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
 #ifdef __GNUC__
 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
 #else
   #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
 #endif /* __GNUC__ */
 PUTCHAR_PROTOTYPE
 {
   HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFFFFFF);
   return ch;
 }

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
