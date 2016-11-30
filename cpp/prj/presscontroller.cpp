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
#include "adc.h"

//pid value


const uint8_t setPin = 1;
const uint8_t plusPin = 2;
const uint8_t minusPin = 2;

const uint8_t highVal = 28;
const uint8_t lowVal = 18;
const uint8_t dryVal = 4;
const uint8_t periodVal = 4;


uint16_t adcValue [8] = {0};

const char lowChar[8] =
{ 
0x04,
0x04,
0x04,
0x04,
0x04,
0x15,
0x0E,
0x04,
};

const char highChar[8] =
{ 
0x04,
0x0E,
0x15,
0x04,
0x04,
0x04,
0x04,
0x04,
};

enum newChar {up, down, cursor};
typedef void (*PtrF)();

Tact frq;
Hd44780 lcd;
Btimer timer4;
Button set (Gpio::A, setPin);
Button plus (Gpio::A, plusPin);
Button minus (Gpio::A, minusPin);
Buffer value;
Adc sensor (Adc::channel3);


struct flags
{
  unsigned setLongPress : 1;
  unsigned setShortPress : 1;
  unsigned PlusPress: 1;
  unsigned screens :2;
  unsigned shift :1;
}flag;

struct position
{
  uint8_t row;
  uint8_t coloumn;
}highPressCursor, lowPressCursor, dryPressCursor, periodCursor;

position * ScreenCursor [3][2] = {
  {0,0},
{&highPressCursor, &lowPressCursor}, 
{&dryPressCursor, &periodCursor}
};

struct data
{
  uint16_t value;
  position pos;	
}currPress, highPress, lowPress, dryPress, period;




data * ScreenVal [3] [2]= {
 {&currPress},
 {&highPress, &lowPress},
 {&dryPress, &period}
};


void mainScreen ();
void pidScreen ();
void changeScreen ();
void getMainScreen ();
void getSet1Screen ();
void getSet2Screen ();
PtrF screenF [3] = {&getMainScreen, &getSet1Screen, &getSet2Screen};

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
    currPress.value = tempAdc >> 3;
    counter.adc = 0;
  } 
  
}
void timer4_init ();

int main()
{
  mainScreen ();
  pidScreen ();
  set.setLongLimit (100);
  set.setShortLimit (3); 
  plus.setShortLimit (3);
  minus.setShortLimit (3);
  
  set.setlongPressAction (changeLpFlag);
  set.setshortPressAction (changeSpFlag);
  value.setFont (Buffer::Array_char);
  initHeater ();
  initFun ();
  initPosition ();
  initDataPosition ();
  timer4_init ();
  
  while (1)
  {
    
  }
}

void initPosition ()
{
  highPressCursor.coloumn = 8;
  highPressCursor.row = 0;
  lowPressCursor.coloumn = 8;
  lowPressCursor.row = 1;
  dryPressCursor.coloumn = 16;
  dryPressCursor.row = 0;
  periodCursor.coloumn = 26;
  periodCursor.row = 0;
}

void initDataPosition ()
{
  highPress.value = highVal;
  highPress.pos.coloumn = 11;
  highPress.pos.row = 0;
  lowPress.value = lowVal;
  lowPress.pos.coloumn = 3;
  lowPress.pos.row = 1;
  dryPress.value = dryVal;
  dryPress.pos.coloumn = 12;
  dryPress.pos.row = 1;
  period.value = periodVal;
  period.pos.coloumn = 18;
  period.pos.row = 0;
}

void mainScreen ()
{
  lcd.newChar (highChar, up);
  lcd.newChar (lowChar, down);
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
  if (flag.setLongPress) 
  {
    flag.setLongPress = 0;
    clearCursors ();
  }
  else 
  {
    flag.setLongPress = 1;
    flag.setShortPress = 0;
  }
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