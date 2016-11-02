#include "stm8s.h"                 // Device header
#include "gpio.h"


#ifndef SEGLED_H
#define SEGLED_H


namespace segledDef
{
  //segment A
  const Gpio::Port segAport = Gpio::D;
  const uint8_t segApin = 2;
  //segment B
  const Gpio::Port segBport = Gpio::D;
  const uint8_t segBpin = 3;
  //segment C
  const Gpio::Port segCport = Gpio::D;
  const uint8_t segCpin = 4;
  //segment D
  const Gpio::Port segDport = Gpio::D;
  const uint8_t segDpin = 5;
  //segment E
  const Gpio::Port segEport = Gpio::D;
  const uint8_t segEpin = 6;
  //segment F
  const Gpio::Port segFport = Gpio::A;
  const uint8_t segFpin = 1;
  //segment G
  const Gpio::Port segGport = Gpio::A;
  const uint8_t segGpin = 2;
  //segment Dp
  const Gpio::Port segDpport = Gpio::A;
  const uint8_t segDppin = 3;
  //digit 1
  const Gpio::Port digit1port = Gpio::C;
  const uint8_t digit1pin = 5;
  //digit 2
  const Gpio::Port digit2port = Gpio::C;
  const uint8_t digit2pin = 4;
  //digit 3
  const Gpio::Port digit3port = Gpio::C;
  const uint8_t digit3pin = 3;
  //digit 4
  const Gpio::Port digit4port = Gpio::C;
  const uint8_t digit4pin = 3;
    
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


