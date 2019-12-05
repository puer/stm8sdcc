#include "stm8s.h"
#include "main.h"

#define TIM4_PERIOD       124
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
static void TIM4_Config(void);

int main(void)
{
  CLK->CKDIVR = ((uint8_t) 0x0);
  TIM4_Config();

  enableInterrupts();


  while (1)
  {

    Delay(100);

  }
}

static void TIM4_Config(void) 
{
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

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
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
  
  UNUSED(file);
  UNUSED(line);

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
