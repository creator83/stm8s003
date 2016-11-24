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

const double p  = 10.0;
const double i  = 2.0;
const double d  = 3.0;

const uint8_t buttEncPin = 1;
const uint8_t tiltPin = 2;
//const uint8_t heaterPin = 3;
//const uint8_t fanPin = 3;
const uint8_t encAPin = 4;
const uint8_t encBPin = 4;

const uint16_t TsetVal=250;
const uint16_t TcurrVal=10;
const uint16_t speedVal=60;

const uint8_t adcPeriod = 100;

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
}speed, temp, pVal, iVal, dVal;

position * ScreenCursor [2][3] = {
{&speedCursor, &tempCursor}, 
{&pCursor, &iCursor, &dCursor}
};

enum newChar {celsius, cursor};


data * ScreenVal [2] [3]= {
{&speed, &temp},
{&pVal, &iVal, &dVal}
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

INTERRUPT_HANDLER(TIM4_OVR_UIF, TIM4_OVR_UIF_vector)
{
  static uint8_t i=0;
  timer4.clearFlag();
  ++i;
  buttonEncoder.scanButton ();
  if (!flag.encLongPress)
  {
    screenF [flag.encShortPress]();
  }
  else
  {
    encoder.setValue (ScreenVal[flag.screens][flag.encShortPress]->data);
    encoder.scan ();
    lcd.setPosition (ScreenCursor[flag.screens][]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
    //lcd.data (cursor);
  }
  if (i>adcPeriod)
  {
    heater.setChannelValue (regulator.compute (sensor.getValue()));
    i=0;
  }
}
void timer4_init ();

int main()
{
  mainScreen ();
  pidScreen ();
  buttonEncoder.setLongLimit (1500);
  buttonEncoder.setShortLimit (10);
  
  buttonEncoder.setlongPressAction (changeLpFlag);
  initHeater ();
  initFun ();
  timer4_init ();
  
  while (1)
  {
    lcd.Shift (Hd44780::Window, Hd44780::Left, 16);
    delay_ms (1000);
    lcd.command (clear_counter);
    delay_ms (1000);
  }
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
  lcd.setPosition (0, 16);
  lcd.sendString ("P=");
  lcd.setPosition (0, 20);
  lcd.data ('.');
  lcd.setPosition (0, 22);
  lcd.sendString ("I=");	
  lcd.setPosition (0, 26);
  lcd.data ('.');
  lcd.setPosition (0, 28);
  lcd.sendString ("D=");	
  lcd.setPosition (0, 30);
  lcd.data ('.');
}

void getMainScreen ()
{
  lcd.command (clear_counter);
}

void getPidScreen ()
{
  lcd.Shift (Hd44780::Window, Hd44780::Left, 16);
}

void changeLpFlag ()
{
  if (flag.encLongPress) 
  {
    flag.encLongPress = 0;
    lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
    lcd.data (' ');
  }
  else flag.encLongPress = 1;
}

void changeSpFlag ()
{
  if (!flag.encLongPress)
  {
    if (flag.encShortPress) 
    {
    flag.encShortPress = 0;
    flag.screens = 0;
    }
    else 
    {
      flag.encShortPress = 1;
      flag.screens = 1;
    }
  }
	
  else if (flag.encLongPress&&flag.screens)//screenPid
  {
    lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
    lcd.data (' ');
    flag.encShortPress++;
    if (flag.encShortPress>2) flag.encShortPress = 0;
  }
  else if (flag.encLongPress&&flag.screens) //mainScreen
  {
    lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
    lcd.data (' ');
    if (flag.encShortPress) flag.encShortPress = 0;
    else flag.encShortPress = 1;
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
}

void timer4_init ()
{
  timer4.setPsc (Btimer::div128);
  timer4.setArr (125);
  enableInterrupts();
  timer4.interrupt (true);
  timer4.start ();
}

void scan_enc ()
{
  uint8_t new_;
  new_ = (1 << encA.state())|encB.state();
  switch (encod.state)
  {
    case 2:
    {
    if (new_ == 3) encod.count++;
    if (new_ == 0) encod.count--;
    break;
    }
    case 0:
    {
    if (new_ == 2) encod.count++;
    if (new_ == 1) encod.count--;
    break;
    }
    case 1:
    {
    if (new_ == 0) encod.count++;
    if (new_ == 3) encod.count--;
    break;
    }		
    case 3:
    {
    if (new_ == 1) encod.count++;
    if (new_ == 2) encod.count--;
    break;
  }				
  }
  encod.state = new_;
}
