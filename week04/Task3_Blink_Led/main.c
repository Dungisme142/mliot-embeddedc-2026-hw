/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (PA0: Button, PC13: LED Onboard)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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
/* Biến lưu mốc thời gian phục vụ Blink chu kỳ 500ms ở Task 3 */
static uint32_t last_blink_time = 0;

/* --- CÁC BIẾN PHỤC VỤ CHỐNG DỘI VÀ CHỐNG GIỮ NÚT NHẤN (TASK 4) --- */
static uint8_t button_stable_state = 1; // Trạng thái ổn định sau chống dội (1: Thả, 0: Nhấn)
static uint8_t last_button_state = 1;   // Trạng thái vòng lặp trước để bắt sườn xuống
static uint32_t last_debounce_time = 0; // Mốc thời gian lọc nhiễu dội phím

/* Fix lỗi Linker RCC do không dùng file hal_rcc.c */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  
  /* USER CODE BEGIN 2 */
  last_blink_time = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* ------------------- TASK 3: NHẤP NHÁY ĐÈN TỰ ĐỘNG (500ms) ------------------- */
    if (HAL_GetTick() - last_blink_time >= 500)
    {
      /* Chỉ đảo trạng thái LED Onboard PC13 */
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      last_blink_time = HAL_GetTick();
    }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  /* USER CODE BEGIN SystemClock_Config */
  /* Để trống theo cấu hình tối giản không dùng driver RCC */
  /* USER CODE END SystemClock_Config */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* Bật xung cho PORT A và PORT C thông qua thanh ghi */
  RCC->APB2ENR |= (1 << 2) | (1 << 4);
  /* USER CODE END MX_GPIO_Init_1 */

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // PC13 ban đầu HIGH (Tắt LED tích cực LOW)

  /*Configure GPIO pin : PC13 (LED Onboard) */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 (NÚT NHẤN) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;     // Đổi hẳn sang chế độ INPUT
  GPIO_InitStruct.Pull = GPIO_PULLUP;        // Kéo trở lên nguồn (Input Pull-up)
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief Handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}
/* USER CODE END 4 */