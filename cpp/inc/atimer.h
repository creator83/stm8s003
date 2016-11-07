#include "stm8s.h"                  // Device header


#ifndef ATIMER_H
#define ATIMER_H



class Atimer
{
	//variables
public:
		
  enum nChannel {channel1, channel2, channel3, channel4};
  enum mode {pwm, pwmInput};                  
private:
		
protected:
  nChannel ch_;
  static uint8_t channelShift [4][2];
  static uint8_t ccmrShift [4];
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
  void setMode (mode m);
  void pwmMode (nChannel ch);
  void pwmInputMode ();
protected:
  uint16_t getCnt ();
  
private:		
  
  
};
 
#endif
