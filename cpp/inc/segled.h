#include "stm8s.h"                 // Device header
#include "gpio.h"


#ifndef SEGLED_H
#define SEGLED_H


namespace segledDef
{
  //segment A
  const Gpio::Port segAport = Gpio::A;
  const uint8_t segApin = 1;
  //segment B
  const Gpio::Port segBport = Gpio::A;
  const uint8_t segBpin = 1;
  //segment C
  const Gpio::Port segCport = Gpio::A;
  const uint8_t segCpin = 1;
  //segment D
  const Gpio::Port segDport = Gpio::A;
  const uint8_t segDpin = 1;
  //segment E
  const Gpio::Port segEport = Gpio::A;
  const uint8_t segEpin = 1;
  //segment F
  const Gpio::Port segFport = Gpio::A;
  const uint8_t segFpin = 1;
  //segment G
  const Gpio::Port segGport = Gpio::A;
  const uint8_t segGpin = 1;
  //segment Dp
  const Gpio::Port segDpport = Gpio::A;
  const uint8_t segDppin = 1;
  //digit 1
  const Gpio::Port digit1port = Gpio::A;
  const uint8_t digit1pin = 1;
  //digit 2
  const Gpio::Port digit2port = Gpio::A;
  const uint8_t digit2pin = 1;
  //digit 3
  const Gpio::Port digit3port = Gpio::A;
  const uint8_t digit3pin = 1;
  //digit 4
  const Gpio::Port digit4port = Gpio::A;
  const uint8_t digit4pin = 1;
    
}


class Segled
{
//variables
public:	
	
private:
  Gpio segA, segB, segC, segD, segE, segF, segG , segDp;
  Gpio dig1, dig2, dig3, dig4;
  static char number [13];
  static char numberDp [13];
  Gpio * segments [8];
  Gpio * digits [4];
  uint8_t segPins [8];
  uint8_t digPins [4];
  uint8_t N;
public:	
  Segled (uint8_t nDigit);
	
  void setSegments (char * val);
  void clearSegments ();
        
  void setDigit (uint8_t d);
  void clearDigits ();
        
  void frame (char * val, uint8_t d);
        
private:
	
	
};


#endif


