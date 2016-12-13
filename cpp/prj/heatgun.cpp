#include "stm8s.h"
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "qenc.h"
#include "gtimer.h"
#include "atimer.h"
#include "btimer.h"
#include "buffer.h"
#include "button.h"
#include "pwm.h"
#include "pid.h"
#include "senc.h"
#include "adc.h"

//pid value

const double p  = 9.0;
const double i  = 2.0;
const double d  = 3.0;

const uint8_t buttEncPin = 1;
const uint8_t tiltPin = 2;
//const uint8_t heaterPin = 3;
//const uint8_t fanPin = 3;
const uint8_t encAPin = 4;
const uint8_t encBPin = 5;

const uint16_t TsetVal=250;
const uint16_t speedVal=60;

uint16_t adcValue [8] = {0};

const char cursorChar[8] =
{ 
0x18,
0x1C,
0x1E,
0x1F,
0x1F,
0x1E,
0x1C,
0x18,
};

const char celsiusChar[8] =
{ 
0x18,
0x18,
0x00,
0x06,
0x09,
0x08,
0x09,
0x06,
};

Tact frq;
Hd44780 lcd;
Gtimer heater (Gtimer::div128);
Atimer fan (20);
Btimer timer4;
Button buttonEncoder (Gpio::A, buttEncPin);
Button tilt (Gpio::A, tiltPin);
Buffer value;
Pin encA (Gpio::B, encAPin, Gpio::Floating);
Pin encB (Gpio::B, encBPin, Gpio::Floating);
Pid regulator (p, i, d, TsetVal);
Senc encoder (Gpio::B, encAPin, Gpio::B, encBPin, 100);
Adc sensor (Adc::channel3);

typedef void (*PtrF)();

struct period_
{
  uint8_t lcd;
  uint8_t adc;
  uint8_t pid;
}period = {10, 40, 100};

struct encdr
{
  uint8_t state;
  uint16_t count;
}encod;

struct flags
{
  unsigned encLongPress : 1;
  unsigned encShortPress : 2;
  unsigned encReady : 1;
  unsigned screens :1;
  unsigned shift :1;
}flag;

struct position
{
  uint8_t row;
  uint8_t coloumn;
}speedCursor, tempCursor, pCursor, iCursor, dCursor;

struct data
{
  uint16_t value;
  position pos;	
}speed, currTemp, setTemp, pVal, iVal, dVal, pidVal;

position * ScreenCursor [2][3] = {
{&speedCursor, &tempCursor}, 
{&pCursor, &iCursor, &dCursor}
};

enum newChar {celsius, cursor};


data * ScreenVal [2] [4]= {
{&speed, &setTemp, &currTemp},
{&pVal, &iVal, &dVal, &pidVal}
};


void mainScreen ();
void pidScreen ();
void changeScreen ();
void getMainScreen ();
void getPidScreen ();
PtrF screenF [2] = {&getMainScreen, &getPidScreen};

void changeLpFlag ();
void changeSpFlag ();
void initHeater ();
void initFun ();
void scan_enc ();
void initPosition ();
void initDataPosition ();
void clearCursors ();

INTERRUPT_HANDLER(TIM4_OVR_UIF, TIM4_OVR_UIF_vector)
{
  static struct counters
  {
    uint8_t lcd;
    uint8_t adc;
    uint8_t pid;
    //uint8_t button;
  }counter = {0,0,0};
  timer4.clearFlag();
  ++counter.lcd;
  ++counter.adc;
  ++counter.pid;
  //++counter.button;


    buttonEncoder.scanButton ();
    buttonEncoder.scanAction();
    
  
  //опрос энкодера при длительном нажатии кнопки
  if (flag.encLongPress)encoder.scan ();

  if (counter.lcd>period.lcd)
  {
    screenF [flag.screens]();
    
    if (flag.encLongPress)
    {   
      clearCursors ();
      lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
      lcd.data (cursor);
      ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue ();
    }
    //draw value
    data **tempPtr = &ScreenVal[0][0];
    for (uint8_t i=0;i<3;++i)
      {
      lcd.setPosition ((*tempPtr)->pos.row, (*tempPtr)->pos.coloumn);
      value.parsDec16 ((*tempPtr)->value, 3);
      lcd.sendString (value.getElement(2));
      *tempPtr++; 
      }
      tempPtr = &ScreenVal[1][0];
      for (uint8_t i=0;i<3;++i)
      {
      lcd.setPosition ((*tempPtr)->pos.row, (*tempPtr)->pos.coloumn);
      value.parsFloat ((*tempPtr)->value);
      lcd.sendString (value.getElement(2));
      *tempPtr++; 
      }
  }
   
   if (counter.adc>period.adc)
  {
    uint16_t tempAdc = 0;
    for (uint8_t i=0;i<8;++i)
    {
      tempAdc += sensor.getValue();
    }
    currTemp.value = tempAdc >> 3;
    counter.adc = 0;
  } 
  
 if (counter.pid>period.pid)
  {
    regulator.setP (pVal.value);
    regulator.setI (iVal.value);
    regulator.setD (dVal.value);
    pidVal.value = regulator.compute (currTemp.value);
    counter.pid = 0;   
  }

}
void timer4_init ();

int main()
{
  mainScreen ();
  pidScreen ();
  buttonEncoder.setLongLimit (100);
  buttonEncoder.setShortLimit (3); 
  
  buttonEncoder.setlongPressAction (changeLpFlag);
  buttonEncoder.setshortPressAction (changeSpFlag);
  value.setFont (Buffer::Array_char);
  initHeater ();
  initFun ();
  initPosition ();
  initDataPosition ();
  encoder.setValue (speed.value);
  timer4_init ();
  
  while (1)
  {
    
  }
}

void initPosition ()
{
  speedCursor.coloumn = 8;
  speedCursor.row = 0;
  tempCursor.coloumn = 8;
  tempCursor.row = 1;
  pCursor.coloumn = 16;
  pCursor.row = 0;
  iCursor.coloumn = 21;
  iCursor.row = 0;
  dCursor.coloumn = 26;
  dCursor.row = 0;
}

void initDataPosition ()
{
  speed.value = speedVal;
  speed.pos.coloumn = 11;
  speed.pos.row = 0;
  currTemp.value = 0;
  currTemp.pos.coloumn = 3;
  currTemp.pos.row = 1;
  setTemp.value = TsetVal;
  setTemp.pos.coloumn = 12;
  setTemp.pos.row = 1;
  pVal.value = regulator.getP();
  pVal.pos.coloumn = 18;
  pVal.pos.row = 0;
  iVal.value = regulator.getI ();
  iVal.pos.coloumn = 23;
  iVal.pos.row = 0;
  dVal.value = regulator.getD ();
  dVal.pos.coloumn = 28;
  dVal.pos.row = 0;
}

void mainScreen ()
{
  lcd.newChar (celsiusChar, celsius);
  lcd.newChar (cursorChar, cursor);
  lcd.setPosition (0, 0);
  lcd.sendString ("HeatGun");
  lcd.setPosition (0, 9);
  lcd.sendString ("F=");
  lcd.setPosition (0, 14);
  lcd.data ('%');
  lcd.setPosition (1, 0);
  lcd.sendString ("Tc=");
  lcd.setPosition (1, 6);
  lcd.data (0);	
  lcd.setPosition (1, 9);
  lcd.sendString ("Ts=");
  lcd.setPosition (1, 15);
  lcd.data (0);	
  lcd.setPosition (1, 7);
  lcd.data (0xFF);	
  lcd.setPosition (0, 7);
  lcd.data (0xFF);
  speedCursor.row = 0;
  speedCursor.coloumn = 9;
  tempCursor.row = 1;
  tempCursor.coloumn = 8;
}

void pidScreen ()
{
  lcd.setPosition (0, 17);
  lcd.data ('P');
  lcd.setPosition (0, 19);
  lcd.data ('.');
  lcd.setPosition (0, 22);
  lcd.sendString ("I");	
  lcd.setPosition (0, 24);
  lcd.data ('.');
  lcd.setPosition (0, 27);
  lcd.sendString ("D");	
  lcd.setPosition (0, 29);
  lcd.data ('.');
}

void getMainScreen ()
{
  lcd.command (clear_counter);
  flag.shift = 0;
}

void getPidScreen ()
{
  if (!flag.shift)
  {
    lcd.Shift (Hd44780::Window, Hd44780::Left, 16);
    flag.shift = 1;
  }
}

void changeLpFlag ()
{
  if (flag.encLongPress) 
  {
    flag.encLongPress = 0;
    //flag.encShortPress = flag.screens;
    clearCursors ();
  }
  else 
  {
    flag.encLongPress = 1;
    flag.encShortPress = 0;
    encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value); 
    lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
    lcd.data (cursor);
   
  }
}

void changeSpFlag ()
{
  if (!flag.encLongPress)
  {
      if (flag.encShortPress) flag.encShortPress = 0;
      else flag.encShortPress = 1;
      flag.screens ^= 1;
  }
	
  else if (flag.encLongPress&&flag.screens)//screenPid
  {
    ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue (); 
    flag.encShortPress++;
    if (flag.encShortPress>2) flag.encShortPress = 0;
    encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value);
  }
  else if (flag.encLongPress&&!flag.screens) //mainScreen
  {
    ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue ();   
    if (flag.encShortPress) flag.encShortPress = 0;
    else flag.encShortPress = 1;
    encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value);
  }
}
	

void initHeater ()
{
  //период 100мs
  heater.setArr (12500);
  heater.pwmMode (Gtimer::channel2);
  heater.start ();
}

void initFun ()
{
  fan.setArr (100);
  fan.pwmMode (Atimer::channel3);
  fan.start ();
}

void timer4_init ()
{
  timer4.setPsc (Btimer::div128);
  timer4.setArr (124);
  enableInterrupts();
  timer4.interrupt (true);
  timer4.start ();
}

void clearCursors ()
{
      //clear all cursors
      position **tPtr = &ScreenCursor[0][0];
      for (uint8_t i=0;i<2;++i)
      {
        lcd.setPosition ((*tPtr)->row, (*tPtr)->coloumn);
        lcd.data (' ');
        *tPtr++;
      }
      tPtr = &ScreenCursor[1][0];
       for (uint8_t i=0;i<3;++i)
      {
        lcd.setPosition ((*tPtr)->row, (*tPtr)->coloumn);
        lcd.data (' ');
        *tPtr++;
      }  
}