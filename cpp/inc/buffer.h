#include "stm8s.h"


#ifndef BUFFER_H
#define BUFFER_H


const char size = 6;

class Buffer
{
public:
  static const char Array_char [11];
  static const char hexChar [16];
  static const char ArraySegChar [11];
  static const char ArraySegDpChar [11];	
private:
  uint8_t n;
  uint8_t count;
  char arr[size];
  char * real;
  const char * font;
public:
  Buffer();
  void parsDec16 (const uint16_t &);
  void parsFloat (const uint16_t &);
  void parsHex32 (uint32_t value);
  bool setElement (uint8_t el, uint8_t val);
  uint8_t getArraySize ();
  char * getArray ();
  char * getContent ();
  char * getElement (uint8_t n);
  uint8_t getCount ();
  void setFont (const char *);
};

	
#endif
