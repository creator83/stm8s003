#include "button.h"

Func Button::buttonFunc [2] = {&Button::scanButtonSingle, &Button::scanButtonDouble};


Button::Button (Gpio::Port p_, uint8_t pi, mode m)
:pin_ (p_, pi, Gpio::Floating)
{
  scanMode = m;
  shortPress = longPress = lastState = currentState = false;
  counter = 0;
  longLimit = 0xFFFF;
}

void Button::scanButton ()
{
  (this->*(Button::buttonFunc[scanMode]))();
}

void Button::scanButtonSingle ()
{
  currentState = !pin_.state (); 
  if (counter>shortLimit)
  {
     shortPress = 1;
     counter = 0;
  }  
  
  if (!shortPress)
  {
    state = lastState << 1| currentState;
    switch (state)
    {
    case 1:
    counter++;
    break;
    case 2:
    counter = 0;
    break;
    case 3:
    counter++;
    break;
    }
    lastState = currentState;
  }
}

void Button::scanButtonDouble ()
{
  currentState = !pin_.state (); 
  if (counter>shortLimit && counter<longLimit&&!currentState)
  {
     shortPress = 1;
     counter = 0;
  }  
  
  if (!shortPress&&!longPress)
  {
    state = lastState << 1| currentState;
    switch (state)
    {
    case 1:
    counter++;
    break;
    case 2:
    counter = 0;
    break;
    case 3:
    counter++;
    break;
    }
    lastState = currentState;
  }

    if (counter>longLimit)
    {
      longPress = 1;
      counter = 0;
    }
}

void Button::scanAction ()
{
  if (longPress) 
  {
    longFunction();
    do
    longPress = 0;
    while (!pin_.state ());
  }
  if (shortPress)
  {
    shortFunction ();
    shortPress = 0;
  }
}

void Button::setShortLimit (uint16_t val)
{
  shortLimit = val;	
}
void Button::setLongLimit (uint16_t val)
{
  longLimit = val;
}
		
void Button::setshortPressAction (void (*f)())
{
  shortFunction = f;
}

void Button::setlongPressAction (void (*f)())
{
  longFunction = f;
}

