#include "stm8s.h"


#ifndef TACT_H
#define TACT_H

class Tact
{
//variables
public:
  enum src_tact {HSI,HSE};
  //enum frq_tact {frq4, frq8, frq12 , frq16, frq20, frq24 ,frq28 , frq32 , frq36,frq40, frq44 , frq48};
private:
  static uint8_t f_cpu;
  uint8_t src;
  //functions
public:
  Tact (uint8_t frq , src_tact s );
  Tact ();
  Tact (src_tact s);
  static uint8_t & get_frq (){return f_cpu;};
  void init_hsi (uint8_t mdiv=0, uint8_t cdiv=0);
  void init_hse ();
  void init_lsi ();
  void setHsiFrq (uint8_t mdiv=0, uint8_t cdiv=0);

};

#endif
