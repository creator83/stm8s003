#include "senc.h"




Senc::Senc (Gpio::Port pA, uint8_t pinA, Gpio::Port pB, uint8_t pinB,uint16_t range)
:pha (pA, pinA, Gpio::Floating), phb (pB, pinB, Gpio::Floating)
{
  high = range << 2;
}

uint16_t Senc::getValue ()
{
  return value >> 1;
}

void Senc::setValue  (uint16_t val)
{
  value = val << 1;
}

void Senc::scan ()
{
  uint8_t new_;
  new_ = (1 << pha.state())|phb.state();
  switch (previusState)
  {
    case 2:
    {
    if (new_ == 3) value++;
    if (new_ == 0) value--;
    break;
    }
    case 0:
    {
    if (new_ == 2) value++;
    if (new_ == 1) value--;
    break;
    }
    case 1:
    {
    if (new_ == 0) value++;
    if (new_ == 3) value--;
    break;
    }		
    case 3:
    {
    if (new_ == 1) value++;
    if (new_ == 2) value--;
    break;
  }				
  }
  previusState = new_;
}

