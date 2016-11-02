#include "stm8s.h"                  // Device header


#ifndef GTIMER_H
#define GTIMER_H


class Gtimer
{
	//variables
public:
		
  enum nChannel {channel1, channel2};
  enum mode {pwm};  
  enum divider {div1, div2, div4, div8, div16, div32, div64, div128, div256, div512, div1024, div2048, div4096, div8192, div16384, div32768};
private:
		
protected:
  nChannel ch_;
  static uint8_t channelShift [3][2];
  static uint8_t ccmrShift [3];
//functions
public:
  Gtimer (divider d);
  Gtimer ();
  void setCnt (uint16_t);
  void setPsc (divider d);
  void setArr (uint16_t);
  void setChannelValue (uint16_t);
  void start ();
  void stop ();
  void clearFlag();
  void setMode (mode m);
  void pwmMode (nChannel ch);
  void interrupt (bool);
protected:
  uint16_t getCnt ();
  
private:
};
 
#endif
