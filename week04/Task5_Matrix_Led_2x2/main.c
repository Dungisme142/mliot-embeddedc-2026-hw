/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body for Task 5 - Matrix Keypad 2x2 & LEDs
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* Biến lưu trạng thái phím ổn định sau chống dội (1: Thả, 0: Nhấn) */
/* matrix_keys[hàng][cột] */
static uint8_t key_stable_states[2][2] = {{1, 1}, {1, 1}};
static uint8_t key_last_states[2][2]   = {{1, 1}, {1, 1}};
static uint32_t key_debounce_timers[2][2] = {{0, 0}, {0, 0}};

/* Định nghĩa mảng chân LED tương ứng để quét vòng lặp cho gọn */
const uint16_t LED_PINS[4] = {GPIO_PIN_7, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6};

/* Fix lỗi Linker RCC do lược bỏ file hal_rcc.c */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void Matrix_Keypad_Scan(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* Infinite loop */
  while (1)
  {
    /* HÀM QUÉT MA TRẬN PHÍM LIÊN TỤC TRONG LOOP */
    Matrix_Keypad_Scan();
  }
}

/**
  * @brief Thuật toán quét ma trận phím 2x2 chống dội, chống giữ
  */
static void Matrix_Keypad_Scan(void)
{
  uint16_t row_pins[2] = {GPIO_PIN_2, GPIO_PIN_3}; // Hàng 0 (PA2), Hàng 1 (PA3)
  uint16_t col_pins[2] = {GPIO_PIN_0, GPIO_PIN_1}; // Cột 0 (PA0), Cột 1 (PA1)

  for (uint8_t r = 0; r < 2; r++)
  {
    /* Bước 1: Kéo hàng hiện tại xuống 0, hàng còn lại giữ mức 1 */
    HAL_GPIO_WritePin(GPIOA, row_pins[r], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, row_pins[1 - r], GPIO_PIN_SET);

    /* Một chút delay siêu nhỏ để điện áp trên đường dây kịp ổn định */
    __NOP(); __NOP();

    /* Bước 2: Duyệt qua từng cột để đọc trạng thái nút nhấn */
    for (uint8_t c = 0; c < 2; c++)
    {
      uint8_t raw_reading = HAL_GPIO_ReadPin(GPIOA, col_pins[c]);

      // Thuật toán chống dội (Debounce) cho từng phím riêng biệt
      if (raw_reading != key_last_states[r][c])
      {
        key_debounce_timers[r][c] = HAL_GetTick();
      }

      if ((HAL_GetTick() - key_debounce_timers[r][c]) >= 20)
      {
        if (raw_reading != key_stable_states[r][c])
        {
          key_stable_states[r][c] = raw_reading;

          // Thuật toán chống giữ (Edge Detection): Chỉ xử lý khi nhấn xuống (Sườn xuống)
          if (key_stable_states[r][c] == 0)
          {
            // Tính toán index của LED dựa trên Hàng và Cột:
            // SW1 (R0, C0) -> Index 0 (PB3)
            // SW2 (R0, C1) -> Index 1 (PB4)
            // SW3 (R1, C0) -> Index 2 (PB5)
            // SW4 (R1, C1) -> Index 3 (PB6)
            uint8_t key_index = (r * 2) + c;
            
            /* Đảo trạng thái con LED tương ứng thông qua firmware */
            HAL_GPIO_TogglePin(GPIOB, LED_PINS[key_index]);
          }
        }
      }
      key_last_states[r][c] = raw_reading;
    }
  }
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  /* Để trống theo cấu hình tối giản không dùng driver RCC */
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Kích hoạt xung nhịp cho PORT A, PORT B và AFIO thông qua thanh ghi */
  RCC->APB2ENR |= (1 << 2) | (1 << 3) | (1 << 0);

  /* ĐÃ FIX: Sử dụng đúng tên Macro hệ thống AFIO_MAPR_SWJ_CFG_NOJNTRST */
  /* Giải phóng chân PB7 và PB4 khỏi tính năng JTAG để làm GPIO thường */
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_NOJNTRST; 

  /* Cấu hình các chân LED xuất Output mức thấp ban đầu (Tắt LED) */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);

  /* Cấu hình cụm LED: PB7, PB4, PB5, PB6 làm Digital Output Push-Pull */
  GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* Cấu hình các chân Cột: PA0, PA1 làm Digital Input Pull-up */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  // ÉP TỐC ĐỘ BĂNG THÔNG THANH GHI ĐỂ ĐẢM BẢO CHÂN NHẬN DIGITAL CHUẨN
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Cấu hình các chân Hàng: PA2, PA3 làm Digital Output Push-Pull */
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  // Đặt mặc định ban đầu các Hàng đều ở mức cao
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);

  /* Cấu hình các chân Cột: PA0, PA1 làm Digital Input Pull-up */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
/**
  * @brief Phục vụ ngắt định thời định kỳ hệ thống cho HAL_GetTick()
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}
/* USER CODE END 4 */