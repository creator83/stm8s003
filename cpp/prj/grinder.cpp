#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
#include "spi.h"
#include "segled.h"
#include "tact.h"
//function
void init_timer ();
void init_down_counter ();
void init_button_counter ();
void scan_enc ();
void scan_button ();
void btn_action ();
void enc_btn_action ();


const char button = 5 ;
const char enc_button = 4 ;
const char triac = 6;
const char encA = 3 ;
const char encB = 2;

struct encdr
{
	uint8_t state;
	uint16_t count;
}encod;
uint16_t N = 62;

struct flags
{
  unsigned led_indicator_delay : 1;
  unsigned off :1;
  unsigned enc_off :1;
}flag;

  struct b_flags
{
  unsigned lmt_sh_press : 6;
  unsigned curr_state   : 1;
  unsigned action_state : 1;
  unsigned count        : 7;
  unsigned sh_press     : 1;
  unsigned l_press      : 1;
  unsigned debouncer    : 1;
}button1, enc_btn;

tact frq (tact::HSE);
segled indicator;
Gpio D (Gpio::D);

INTERRUPT_HANDLER(down_counter, TIM2_OVR_UIF_vector)
{
  TIM2->SR1 &= ~TIM2_SR1_UIF;
  flag.enc_off = 1;
	if (N)
	{
		N--;
		indicator.get_buffer (N);
		D.setPin (triac);
	}
	else 
	{
		TIM2->CR1 &= ~TIM2_CR1_CEN;
		D.clearPin (triac);	
                TIM2->CNTRH = 0;
                TIM2->CNTRL = 0;
		indicator.get_buffer (encod.count>>1);
		flag.enc_off = 0;
	}
}

//Main loop
INTERRUPT_HANDLER(main_loop, TIM4_OVR_UIF_vector )
{
  TIM4->SR1 &= ~TIM4_SR1_UIF;
  if (flag.off) ;
  else
  {
    if (flag.led_indicator_delay)flag.led_indicator_delay = 0;
    else
    {
      indicator.digit();
      flag.led_indicator_delay = 1;
    }
  }
  if (!(flag.off||flag.enc_off)) scan_enc ();
  if (button1.sh_press) btn_action ();
  if (enc_btn.sh_press)enc_btn_action ();

  
}

INTERRUPT_HANDLER(scan_buttons, TIM1_OVR_UIF_vector )
{
  TIM1->SR1 &= ~TIM1_SR1_UIF;
  scan_button ();
}




int main()
{
  encod.count = N << 1;
  indicator.get_buffer (encod.count>>1);
  D.setOutPin (triac);
  D.setInPin (button);
  D.setInPin (enc_button);
  D.setInPin (encA);
  D.setInPin (encB);  
  
  init_timer ();
  init_down_counter ();
  init_button_counter ();
   
  while (1)
  {
   
  }
}

//main_loop
void init_timer ()
{
  __enable_interrupt ();
  CLK->PCKENR1 |=CLK_PCKENR1_TIM4;
  TIM4->SR1 &= ~TIM4_SR1_UIF;
  TIM4->IER |= TIM4_IER_UIE;
  TIM4->PSCR = 7;
  TIM4->ARR = 124;
  TIM4->CR1 |= TIM4_CR1_CEN;
}

//100ms
void init_down_counter ()
{
  CLK->PCKENR1 |=CLK_PCKENR1_TIM2;
  TIM2->SR1 &= ~TIM2_SR1_UIF;
  TIM2->IER |= TIM2_IER_UIE;
  TIM2->PSCR = 8;
  TIM2->ARRH = 0x18;
  TIM2->ARRL = 0x6A;
}

//20ms
void init_button_counter ()
{
  CLK->PCKENR1 |=CLK_PCKENR1_TIM1;
  TIM1->SR1 &= ~TIM1_SR1_UIF;
  TIM1->IER |= TIM1_IER_UIE;
  TIM1->PSCRH = 0x3E;
  TIM1->PSCRL = 0x80;
  TIM1->ARRH = 0;
  TIM1->ARRL = 19; 
  TIM1->CR1 |= TIM1_CR1_CEN;
}

void scan_enc ()
{
  uint8_t new_;
  new_ = (1 << D.pin_state(encA))|D.pin_state(encB);
  switch (encod.state)
  {
    case 2:
    {
      if (new_ == 3) encod.count++;
      if (new_ == 0) encod.count--;
      break;
    }
    case 0:
    {
      if (new_ == 2) encod.count++;
      if (new_ == 1) encod.count--;
      break;
    }
    case 1:
    {
      if (new_ == 0) encod.count++;
      if (new_ == 3) encod.count--;
      break;
    }		
    case 3:
    {
      if (new_ == 1) encod.count++;
      if (new_ == 2) encod.count--;
      break;
    }				
  }
    encod.state = new_;
    N = encod.count>>1;
    indicator.get_buffer (N);

}


void btn_action()
{
  button1.sh_press = 0;
  enc_btn.debouncer = 1;
  N = encod.count>>1;
  TIM2->CR1 |= TIM2_CR1_CEN;		
}


void enc_btn_action ()
{
  enc_btn.debouncer = 1;
  enc_btn.sh_press = 0;
  flag.off ^= 1;
  indicator.OFF ();
}

void scan_button ()
{
  //debouncer
	
//scan button
  if (!flag.off)
  {
    if  (!button1.debouncer)
    {
      if (button1.curr_state)
      {
	if (!D.pin_state(button)) button1.count++;
	if (D.pin_state(button)&&button1.count>2)
	{
          button1.curr_state = 0;
          button1.count = 0;
          button1.sh_press = 1;
	}

      }
      else
      {
	if (!D.pin_state(button)) button1.curr_state = 1;
      }
    }
    else button1.count++;
    if (button1.debouncer&&button1.count>5) 
    {
      button1.debouncer=0;
      button1.count = 0;
    }
  }
	
	//scan button encoder
		if  (!enc_btn.debouncer)
		{
		if (enc_btn.curr_state)
	{
		if (!D.pin_state(enc_button)) enc_btn.count++;
		if (D.pin_state(enc_button)&&enc_btn.count>2)
		{
			enc_btn.curr_state = 0;
			enc_btn.count = 0;
			enc_btn.sh_press = 1;
		}

	}
	else
	{
		if (!D.pin_state(enc_button)) enc_btn.curr_state = 1;
	}
	}
		else enc_btn.count++;
	if (enc_btn.debouncer&&enc_btn.count>5) 
	{
		enc_btn.debouncer=0;
		enc_btn.count = 0;
	}
}
