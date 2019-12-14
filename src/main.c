#include "main.h"
#include "stdio.h"
#include "stm8s.h"

#define TIM4_PERIOD 124
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;
__IO u16 count = 1000;
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
static void TIM4_Config(void);
static void TIM1_Config(void);
static void UART1_Config(void);
void LEDOn(void);
void LEDOff(void);

u16 pwm_apr = 267; // 50%;

int main(void) {
  CLK->CKDIVR = ((uint8_t)0x0);
  TIM4_Config();
  EXTI->CR1 |= (uint8_t)((uint8_t)(0x03) << 4);
  UART1_Config();
  TIM1_Config();
  enableInterrupts();
  Delay(100);
  puts("1");

  // enable tim1
  TIM1->CR1 |= TIM1_CR1_CEN;

  // enable pwm output
  TIM1->BKR |= TIM1_BKR_MOE;
  puts("2");
  LED_PORT->DDR |= (uint8_t)LED_PIN;
  LED_PORT->CR1 |= (uint8_t)LED_PIN;
  LED_PORT->CR2 &= (uint8_t)(~(LED_PIN));

  // EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PCIS);
  RE_PORT->DDR &= (uint8_t)(~(RE_A_PIN));
  RE_PORT->DDR &= (uint8_t)(~(RE_B_PIN));

  RE_PORT->CR1 |= (uint8_t)RE_A_PIN;
  RE_PORT->CR1 |= (uint8_t)RE_B_PIN;

  puts("main-2\r\n");
  RE_PORT->CR2 |= (uint8_t)RE_A_PIN;
  puts("main-2.2\r\n");
  RE_PORT->CR2 |= (uint8_t)RE_B_PIN;
  LEDOn();
  while (1) {
    printf("main [%d]\r\n", count);
    // puts(TIM1->CNTRL);
    Delay(count);
    LED_PORT->ODR ^= (uint8_t)LED_PIN;
  }
}

void LEDOn(void) { LED_PORT->ODR &= (uint8_t)(~LED_PIN); }

void LEDOff(void) { LED_PORT->ODR |= (uint8_t)LED_PIN; }

static void TIM1_Config(void) {
  puts("tim1_config\r\n");
  // 533 = 16M / 30K
  u16 TIM1_Period = 532;
  u16 TIM1_Prescaler = 0;
  // TIM1_COUNTERMODE_UP =0
  u8 TIM1_CounterMode = 0;
  u8 TIM1_RepetitionCounter = 0;

  /* Set the Autoreload value */
  TIM1->ARRH = (uint8_t)(TIM1_Period >> 8);
  TIM1->ARRL = (uint8_t)(TIM1_Period);

  /* Set the Prescaler value */
  TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);

  /* Select the Counter Mode */
  TIM1->CR1 = (uint8_t)(
      (uint8_t)(TIM1->CR1 & (uint8_t)(~(TIM1_CR1_CMS | TIM1_CR1_DIR))) |
      (uint8_t)(TIM1_CounterMode));
  TIM1_OCIDLESTATE_SET;
  /* Set the Repetition Counter value */
  TIM1->RCR = TIM1_RepetitionCounter;

  // TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE,
  // TIM1_OUTPUTNSTATE_DISABLE,
  //              CCR1_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH,
  //              TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_RESET);
  /* Disable the Channel 1: Reset the CCE Bit, Set the Output State ,
  the Output N State, the Output Polarity & the Output N Polarity*/
  TIM1->CCER1 &= (uint8_t)(~(TIM1_CCER1_CC2E | TIM1_CCER1_CC2NE |
                             TIM1_CCER1_CC2P | TIM1_CCER1_CC2NP));
  TIM1->CCER1 |= (uint8_t)((uint8_t)((uint8_t)(0x11 & TIM1_CCER1_CC1E) |
                                     (uint8_t)(0 & TIM1_CCER1_CC1NE)) |
                           (uint8_t)((uint8_t)(0x22 & TIM1_CCER1_CC1P) |
                                     (uint8_t)(0 & TIM1_CCER1_CC1NP)));

  // pwm 2
  /* Reset the Output Compare Bits & Set the Output Compare Mode */
  TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_OCM)) |
                          (uint8_t)0x70);
  /* Reset the Output Idle state & the Output N Idle state bits */
  TIM1->OISR &= (uint8_t)(~(TIM1_OISR_OIS1 | TIM1_OISR_OIS1N));
  /* Set the Output Idle state & the Output N Idle state configuration */
  TIM1->OISR |= (uint8_t)((uint8_t)(0x55 & TIM1_OISR_OIS1) |
                          (uint8_t)(0 & TIM1_OISR_OIS1N));
  TIM1->CCR1H = (uint8_t)(pwm_apr >> 8);
  TIM1->CCR1L = (uint8_t)pwm_apr;

}

static void TIM4_Config(void) {
  /* div by 128 count = 125 => 1ms UEV */
  TIM4->PSCR = (uint8_t)(0x07);
  /* Set the Autoreload value */
  TIM4->ARR = (uint8_t)(TIM4_PERIOD);
  /* enable interrupt */
  TIM4->IER |= TIM4_IER_UIE;

  /* enable arr auto reload */
  TIM4->CR1 |= TIM4_CR1_ARPE;
  /* enable tim4 */
  TIM4->CR1 |= TIM4_CR1_CEN;
}

static void UART1_Config(void) {
  UART1->CR2 = UART1_CR2_TEN | UART1_CR2_REN;
  // 1 Stop Bit
  // UART1->CR3;
  // 9600 baud
  UART1->BRR2 = 0x03;
  UART1->BRR1 = 0x68;
}

int putchar(int c) {
  while (!(UART1->SR & UART1_SR_TXE))
    ;
  UART1->DR = (uint8_t)c;
  return c;
}
/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void Delay(__IO uint32_t nTime) {
  TimingDelay = nTime;

  while (TimingDelay != 0)
    ;
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval : None
 */
void assert_failed(u8 *file, u32 line) {

  UNUSED(file);
  UNUSED(line);

  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1) {
  }
}
#endif

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23) {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  if (TimingDelay != 0x00) {
    TimingDelay--;
  }
  TIM4->SR1 = (uint8_t)(~TIM4_SR1_UIF);
}

static void update_pwm(u16 count) {
  u16 compare1 = count / 50 * 533 / 40;
  TIM1->CCR1H = (uint8_t)(compare1 >> 8);
  TIM1->CCR1L = (uint8_t)(compare1);
}
/**
 * @brief  External Interrupt PORTC Interrupt routine
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5) {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  static int rot_enc_table[] = {0, 1, -1, 0,  -1, 0,  0, 1,
                                1, 0, 0,  -1, 0,  -1, 1, 0};
  static int quarter = 0;
  static u8 prev = 0;

  // u8 n = ((GPIOC->IDR)>>6) & 0x03;

  u8 n = (RE_PORT->IDR & (RE_B_PIN | RE_A_PIN)) >> RE_SHIFT;

  if (((prev ^ n) & 0x03) != 0) {
    prev = ((prev << 2) | n) & 0x0f;

    if (1 == rot_enc_table[prev]) {
      if (quarter < 0) {
        quarter = 0;
      }
      quarter++;
      if (quarter == 4) {
        // right++;
        if (count < 2000) {
          count += 50;
        }
        puts("+\r\n");
        quarter = 0;
      }
    } else if (-1 == rot_enc_table[prev]) {
      if (quarter > 0) {
        quarter = 0;
      }
      quarter--;
      if (quarter == -4) {
        // left++;
        if (count >= 50)
          count -= 50;
        puts("-\r\n");
        quarter = 0;
      }
    }
    update_pwm(count);
  } else {
    printf("%x\r\n", RE_PORT->IDR);
  }
}


