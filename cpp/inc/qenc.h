#include "stm8s.h"    
#include "gpio.h"


/*
 * TIM3
 * PHA - PTC6 
 * PHB - PTC7 
 */

namespace QdDef
{
//===Defenitions===//
//PHA
const Gpio::Port PhaPort = Gpio::C;
const uint8_t PhaPin = 6;

//PHB
const Gpio::Port PhbPort = Gpio::C;
const uint8_t PhbPin = 7;
}




#ifndef QENC_H
#define QENC_H


class Qenc 
{
  //variables
public:

private:
	Gpio pha, phb;
	uint16_t high;
	uint16_t value;

public:
	Qenc (uint16_t range);
	uint16_t getValue ();
	void setValue  (uint16_t);
        void start ();
        void stop ();

private:
	void setMode ();
};



#endif
