#include "stm8s.h"                  // Device header


#ifndef BTIMER_H
#define BTIMER_H



class Btimer
{
	//variables
public:

  enum divider {div1, div2, div4, div8, div16, div32, div64, div128};
private:
		
protected:
  
//functions
public:
  Btimer (divider d);
  Btimer ();
  void setCnt (uint8_t);
  void setPsc (divider d);
  void setArr (uint8_t);
  void start ();
  void stop ();
  uint8_t getCnt ();
  void clearFlag();
  void interrupt (bool);
  
protected:
  
  
private:		
  
  
};
 
#endif
