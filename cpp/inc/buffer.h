#include "stm8s.h"


#ifndef BUFFER_H
#define BUFFER_H

const char size = 8;

class Buffer
{
public:
	
private:
  static const char Array_char [11];
  static const char hexChar [16];
  static const char ArraySegChar [11];
  static const char ArraySegDpChar [11];
  uint8_t n;
  uint8_t count;
  char arr[size];
  char * real;
public:
  Buffer();
  void pars (const uint16_t &);
  void parsHex32 (uint32_t value);
  bool setElement (uint8_t el, uint8_t val);
  uint8_t getArraySize ();
  char * getArray ();
  char * getContent ();
  char * getElement (uint8_t n);
  uint8_t getCount ();
};

	
#endif
