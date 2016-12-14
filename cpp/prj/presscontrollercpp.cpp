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
#include "nrf24l01.h"
#include "shape.h"
#include "str.h"



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

uint8_t screens [3] = {0,8,16};


void mainScreen ();
void set1Screen ();
void set2Screen ();

void screen (Shape *, uint8_t l);

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
    uint16_t lcd;
    uint16_t adc;
    uint16_t button;
    
  }counter = {0,0,0};
  timer4.clearFlag();
  ++counter.lcd;
  ++counter.adc;
  ++counter.button;
  static uint8_t prevPosition=0;
  

    
    set.scanButton ();
    set.scanAction();
    
     //опрос кнопок +, - при длительном нажатии кнопки
    if (flag.setLongPress)
    {
      plus.scanButton ();
      minus.scanButton ();
      plus.scanAction();
      minus.scanAction();
    }
   
    
  
 

  if (counter.lcd>100)
  {
    if (prevPosition != screens[flag.screens]) lcd.setShiftPosition ( screens[flag.screens]);
   
    if (flag.setLongPress)
    {   
      clearCursors ();
      lcd.setPosition (ScreenCursor[flag.screens][flag.setShortPress]->row, ScreenCursor[flag.screens][flag.setShortPress]->coloumn);
      lcd.data (cursor);
    }
    //draw value
    //main screen
    lcd.setPosition (ScreenVal[0][0]->pos.row, ScreenVal[0][0]->pos.coloumn);
    value.parsDec16 (ScreenVal[0][0]->value, 3);
    lcd.sendString (value.getElement(2));
      
    for (uint8_t i=1;i<3;++i)
    {
      for (uint8_t j=0;j<2;++j)
      {
        lcd.setPosition (ScreenVal[i][j]->pos.row, ScreenVal[i][j]->pos.coloumn);
        value.parsDec16 (ScreenVal[i][j]->value, 3);
        lcd.sendString (value.getElement(2));
      }
    }    
  }
   
   if (counter.adc>100)
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
  Shape::driver = &lcd;
  Nrf24l01 radio;
  mainScreen ();
  set1Screen ();
  set2Screen ();
  set.setLongLimit (100);
  set.setShortLimit (3); 
  plus.setShortLimit (3);
  minus.setShortLimit (3);
  
  set.setlongPressAction (changeLpFlag);
  set.setshortPressAction (changeSpFlag);
  value.setFont (Buffer::Array_char);
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
  periodCursor.coloumn = 16;
  periodCursor.row = 1;
}

void initDataPosition ()
{
  currPress.value = 0;
  currPress.pos.coloumn = 3;
  currPress.pos.row = 0;
  highPress.value = highVal;
  highPress.pos.coloumn = 12;
  highPress.pos.row = 0;
  lowPress.value = lowVal;
  lowPress.pos.coloumn = 12;
  lowPress.pos.row = 1;
  dryPress.value = dryVal;
  dryPress.pos.coloumn = 20;
  dryPress.pos.row = 0;
  period.value = periodVal;
  period.pos.coloumn = 20;
  period.pos.row = 1;
}

void mainScreen ()
{
  lcd.setPosition (0, 1);
  lcd.sendString ("P=");
  lcd.setPosition (0, 6);
  lcd.data ('B');
}

void set1Screen ()
{
  lcd.newChar (highChar, up);
  lcd.newChar (lowChar, down);
  lcd.setPosition (0, 9);
  lcd.data ('P');
  lcd.data (up);
  lcd.data ('=');
  lcd.setPosition (1, 9);
  lcd.data ('P');
  lcd.data (down);
  lcd.data ('=');
}

void set2Screen ()
{
  lcd.setPosition (0, 17);
  lcd.sendString ("P#=");
  lcd.setPosition (1, 17);
  lcd.sendString ("T=");
}
void getMainScreen ()
{
  lcd.command (clear_counter);
  flag.shift = 0;
}

void getSet1Screen ()
{
  
}

void getSet2Screen ()
{
  
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
  if (!flag.setLongPress)
  {
    if (flag.setShortPress) 
    {
      ++flag.screens;
      if (flag.screens>2)flag.screens = 0;
    }
  }
	
  else if (flag.setLongPress&&flag.screens)//screenPid
  {
    flag.setShortPress ^= 1;
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
  for (uint8_t i=1;i<3;++i)
  {
    for (uint8_t j=0;i<2;++j)
    {
      lcd.setPosition (ScreenCursor[i][j]->row, ScreenCursor[i][j]->coloumn);
      lcd.data (' ');
    }
  }
}