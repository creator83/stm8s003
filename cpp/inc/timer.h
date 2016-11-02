#include "stm8s.h"                  // Device header


#ifndef ATIMER_H
#define ATIMER_H

class Atimer
{
	//variables
public:
		
  enum nChannel {channel1, channel2, channel3, channel4};
		
private:
		
protected:
  nChannel ch_;
//functions
public:
  Atimer (uint16_t divider);
  Atimer ();
  void setCnt (uint16_t);
  void setPsc (uint16_t);
  void setArr (uint16_t);
  void setChannelValue (uint16_t);
  void start ();
  void stop ();
  void clearFlag();
protected:
  uint16_t getCnt ();
private:		

};
 
#endif
