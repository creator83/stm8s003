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
  void init_hsi ();    
private:
  void init_pll (uint8_t i);
  void init_pll ();  
  void init_hse ();
  void Set_frq (uint8_t frq);
};

#endif
