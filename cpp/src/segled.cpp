#include "segled.h"

char Segled::number [13] = {0x3F ,0x06 , 0x5B , 0x4F , 0x66 , 0x6D , 0x7D, 0x07 , 0x7F , 0x6F ,  0x00, 0x60, 0x6A};
char Segled::numberDp [13] = {0xBF , 0x86 , 0xDB , 0xCF , 0xE6 , 0xED , 0xFD , 0x87 , 0xFF , 0xEF , 0x00, 0x60, 0x6A};


Segled::Segled (uint8_t nDigit)
:segA (segledDef::segAport), segB (segledDef::segBport), segC (segledDef::segCport), segD (segledDef::segDport), segE (segledDef::segEport), segF (segledDef::segFport), segG (segledDef::segGport), segDp (segledDef::segDpport),
dig1 (segledDef::digit1port), dig2 (segledDef::digit2port), dig3 (segledDef::digit3port), dig4 (segledDef::digit4port)
{
  N = nDigit;
  //Settings pin
  segA.setOutPin (segledDef::segApin);
  segB.setOutPin (segledDef::segBpin);
  segC.setOutPin (segledDef::segCpin);
  segD.setOutPin (segledDef::segDpin);
  segE.setOutPin (segledDef::segEpin);
  segF.setOutPin (segledDef::segFpin);
  segG.setOutPin (segledDef::segGpin);
  segDp.setOutPin (segledDef::segDppin);
  
  dig1.setOutPin (segledDef::digit1pin);
  dig2.setOutPin (segledDef::digit2pin);
  dig3.setOutPin (segledDef::digit3pin);
  dig4.setOutPin (segledDef::digit4pin);
  
  segments [0] = &segA;
  segments [1] = &segB;
  segments [2] = &segC;
  segments [3] = &segD;
  segments [4] = &segE;
  segments [5] = &segF;
  segments [6] = &segG;
  segments [7] = &segDp;
  
  digits [0] =  &dig1;
  digits [1] =  &dig2;
  digits [2] =  &dig3;
  digits [3] =  &dig4;
    
  segPins [0] = segledDef::segApin;
  segPins [1] = segledDef::segBpin;
  segPins [2] = segledDef::segCpin;
  segPins [3] = segledDef::segDpin;
  segPins [4] = segledDef::segEpin;
  segPins [5] = segledDef::segFpin;
  segPins [6] = segledDef::segGpin;
  segPins [7] = segledDef::segDppin;
  
  digPins [0] = segledDef::digit1pin;
  digPins [1] = segledDef::digit2pin;
  digPins [2] = segledDef::digit3pin;
  digPins [3] = segledDef::digit4pin;
}


void Segled::setSegments (char * val)
{
  for (uint8_t i=0; i<8; ++i)  
  {
    if (*val& 1<<i) segments [i]->setPin (segPins [i]);
    else segments [i]->clearPin (segPins [i]);
  }
}

void Segled::clearSegments ()
{
  for (uint8_t i=0;i<8;++i) segments [i]->clearPin (segPins [i]);
}

void Segled::setDigit (uint8_t d)
{
  digits[d]->setPin (digPins [d]);
}

void Segled::clearDigits ()
{
  for (uint8_t i=0;i<N;++i) digits [i]->clearPin (digPins[i]);
}

void Segled::frame (char * val, uint8_t d)
{
  clearSegments();
  clearDigits ();
  setSegments (val);
  setDigit (d);
}
