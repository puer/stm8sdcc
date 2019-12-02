#include "stm8s.h"

#define LED_PIN GPIO_PIN_4
#define LED_PORT GPIOA


#define RE_PORT GPIOC
#define RE_PSBTN_PIN GPIO_PIN_3
#define RE_A_PIN GPIO_PIN_4
#define RE_B_PIN GPIO_PIN_5

inline void LEDOn(void);
inline void LEDOff(void);
uint32_t LSIMeasurment(void);
void Delay(uint16_t nCount);