#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
#include "intrinsics.h"



Gpio B (Gpio::C);

extern "C"
{
  INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13);
}

void init ();

INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
    B.ChangePinState (4);
   
}

#pragma vector = 13
__interrupt void messure(void)
 {
   B.ChangePinState (4);
	
         
 }

int main()
{
  CLK->CKDIVR = 0;
  B.setOutPin(4);
  init();
  
  while (1)
  {

  }
}

void init ()
{
   
  #define TIM2_ARR ((uint16_t)(1 * 62500 - 1))
  TIM2->PSCR = 7;//(uint8_t)(TIM2_PRESCALER_128);  //Настройка предделителя
  /* Настройка значения по которуму будет выполнятся сброс счетчика таймера */
  TIM2->ARRH = (uint8_t)((TIM2_ARR>>8)& 0xFF);
  TIM2->ARRL = (uint8_t)((TIM2_ARR & 0xFF));
  TIM2->SR1 &= ~TIM2_SR1_UIF;    // очистка флага прерывания TIM2 UPDATE
  TIM2->IER |= TIM2_IER_UIE;  //включение прерывания TIM2 UPDATE
  __enable_interrupt ();  //глабально разрешаем прерывания
  TIM2->CR1 |= TIM2_CR1_CEN; //запуск счета таймера
}