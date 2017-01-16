#include "stm8s.h"
#include "pin.h"
#include "delay.h"

Pin triac1 (Gpio::A, 3, Gpio::lowSpeed);
Pin triac2 (Gpio::D, 3, Gpio::lowSpeed);
Pin triac3 (Gpio::D, 4, Gpio::lowSpeed);
Pin triac4 (Gpio::D, 5, Gpio::lowSpeed);
Pin triac5 (Gpio::D, 6, Gpio::lowSpeed);
union rgb24
{
	uint32_t color;
	struct colors
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};
};

Pin * triacs [5] = {&triac1, &triac2, &triac3, &triac4, &triac5};

int main()
{
  CLK->CKDIVR = 0;

  while (1)
  {
    for (uint8_t i=0;i<5;++i) triacs [i]->set ();
    delay_ms (1000);
    for (uint8_t i=0;i<5;++i) triacs [i]->clear ();
    delay_ms (1000);
  
    
  }
}
