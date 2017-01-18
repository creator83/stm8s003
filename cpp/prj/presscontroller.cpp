#include "stm8s.h"
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "gtimer.h"
#include "atimer.h"
#include "btimer.h"
#include "buffer.h"
#include "button.h"
#include "adc.h"


const uint8_t highVal = 28;
const uint8_t lowVal = 18;
const uint8_t dryVal = 4;
const uint8_t periodVal = 4;

uint8_t *highValEeprom, *lowValEeprom, *dryValEeprom;


uint16_t adcValue [10] = {0};

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
Button set (Gpio::A, 5, Button::twoAction);
Button plus (Gpio::A, 4, Button::oneAction);
Button minus (Gpio::A, 3, Button::oneAction);
Buffer value;
Adc sensor(Adc::channel5);
Atimer adcTrigger (16000);


struct flags
{
  unsigned setLongPress : 1;
  unsigned setShortPress : 2;
  unsigned screens :2;
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
  uint8_t value;
  uint8_t highLimit;
  uint8_t lowLimit;
  uint8_t * eepromPtr;
  position pos;	
}currPress, highPress, lowPress, dryPress, period;




data * ScreenVal [3] [2]= {
 {&currPress, 0},
 {&highPress, &lowPress},
 {&dryPress, &period}
};

data * setupVal [3] [2]= {
 {0, 0},
 {&highPress, &lowPress},
 {&dryPress, &period}
};

uint8_t screens [3] = {0,8,16};


void mainScreen ();
void set1Screen ();
void set2Screen ();
void changeScreen ();
void getMainScreen ();
void getSet1Screen ();
void getSet2Screen ();
PtrF screenF [3] = {&getMainScreen, &getSet1Screen, &getSet2Screen};


void longSetPress ();
void shortSetPress ();
void minusPress ();
void plusPress ();


void initPosition ();
void initDataPosition ();


INTERRUPT_HANDLER(adc, ADC1_EOC_vector)
{
  sensor.clearEoc ();
  uint16_t data [10];
  uint16_t result=0;
  sensor.getBuffer (data);
  for (uint8_t i=0;i<10;++i) result += data [i];
  result<<=1;
  value.parsDec16 (result, 5);
  lcd.setPosition (1,0);
  lcd.sendString (value.getElement (0)); 
  //value.parsDec16 (*eepromPtr, 2);
  lcd.setPosition (1,6);
  lcd.sendString (value.getElement (3)); 
}

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
   
  if (counter.lcd>200)
  {
    if (prevPosition != screens[flag.screens]) lcd.setShiftPosition ( screens[flag.screens]);
   
    if (flag.setLongPress)
    {   
      //clearCursors ();
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
  FLASH->DUKR = 0xAE;
  FLASH->DUKR = 0x56;
  highValEeprom = (uint8_t*)0x004000; 
  lowValEeprom = (uint8_t*)0x004001; 
  dryValEeprom = (uint8_t*)0x004002; 
  mainScreen ();
  set1Screen ();
  set2Screen ();
  set.setLongLimit (100);
  set.setShortLimit (3); 
  plus.setShortLimit (3);
  minus.setShortLimit (3);
  
  set.setlongPressAction (longSetPress);
  set.setshortPressAction (shortSetPress);
  
  value.setFont (Buffer::Array_char);
  initPosition ();
  initDataPosition ();
  //sensor.setContiniusMode ();
  
  adcTrigger.setArr (100);
  sensor.setBuffer ();
  sensor.setTrigger (Adc::timer);
  sensor.enableInterrupt ();
  adcTrigger.start ();
  //sensor.start ();
  //timer4_init ();
  
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
  highPress.highLimit = 40;
  highPress.lowLimit = 20;
  *highValEeprom = highPress.value;
  highPress.eepromPtr = highValEeprom;
  highPress.pos.coloumn = 12;
  highPress.pos.row = 0;
  lowPress.value = lowVal;
  lowPress.highLimit = 25;
  lowPress.lowLimit = 10;
  *lowValEeprom = lowPress.value;
  lowPress.pos.coloumn = 12;
  lowPress.pos.row = 1;
  dryPress.value = dryVal;
  dryPress.highLimit = 9;
  dryPress.lowLimit = 0;
  *dryValEeprom = dryPress.value;
  dryPress.pos.coloumn = 20;
  dryPress.pos.row = 0;
  period.value = periodVal;
  period.pos.coloumn = 20;
  period.pos.row = 1;
}

void mainScreen ()
{
  lcd.setPosition (0, 0);
  lcd.sendString ("Current");
  lcd.setPosition (1, 1);
  lcd.sendString ("P=");
  lcd.setPosition (1, 6);
  lcd.data ('b');
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
  lcd.setPosition (0, 16);
  lcd.sendString ("#P=");
  lcd.setPosition (1, 17);
  lcd.sendString ("T=");
}

void longSetPress ()
{
  if (flag.setLongPress) 
  {
    flag.setLongPress = 0;
    //clearCursors ();
  }
  else 
  {
    if (ScreenCursor [flag.screens][flag.setShortPress] != 0)
    {
      flag.setLongPress = 1;
      flag.setShortPress = 0;
      //setcursor
    }
  }
}

void shortSetPress ()
{
  if (!flag.setLongPress) 
  {
    ++flag.screens;
    if (flag.screens>2)flag.screens = 0;
  }
  else
  {
    //clearcursor
    flag.setShortPress ^= 1;
    
    //setcursor
  }
}

void minusPress ()
{
  if (flag.setLongPress)
  {
    data * ptr = setupVal [flag.screens][flag.setShortPress];
    if (ptr)
    {
      if (ptr->value>ptr->lowLimit)
      --ptr->value;
      *ptr->eepromPtr = ptr->value;
    }
  }
}
void plusPress ()
{
   if (flag.setLongPress)
  {
    data * ptr = setupVal [flag.screens][flag.setShortPress];
    if (ptr)
    {
      if (ptr->value<ptr->highLimit)
      ++ptr->value;
      *ptr->eepromPtr = ptr->value;
    }
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
