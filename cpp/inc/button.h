#include "stm8s.h"               // Device header
#include "pin.h"


#ifndef BUTTON_H
#define BUTTON_H

class Button;
typedef void (Button::*Func)();

class Button
{
//variables
public:
  enum mode {oneAction, twoAction};
private:
  uint16_t counter;
  bool shortPress;
  bool longPress;
  bool lastState;
  //bool pushState;
  bool currentState;
  uint8_t state;
  uint16_t shortLimit;
  uint16_t longLimit;
  void (*shortFunction)();
  void (*longFunction)();
  uint8_t p;
  Pin pin_;
  static Func buttonFunc [2];
  static Func actionFunc [2];
  uint8_t scanMode;
//func
public:
Button (Gpio::Port, uint8_t pin_, mode m);
  void scanButton ();
  void scanButtonSingle ();
  void scanButtonDouble ();
  void scanAction ();
  void scanActionSingle ();
  void scanActionDouble ();
  void setShortLimit (uint16_t);
  void setLongLimit (uint16_t);
  bool & getShortPress (){return shortPress;}
  bool & getLongPress (){return longPress;}
  void setshortPressAction (void (*f)());
  void setlongPressAction (void (*f)());
};

#endif
