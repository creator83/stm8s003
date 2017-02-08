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
#include "pin.h"

//Should comment after 1st init
//============================
/*const uint8_t highVal = 28;
const uint8_t lowVal = 18;
const uint8_t dryVal = 4;
const uint8_t periodVal = 4;*/
//============================

uint8_t *highValEeprom, *lowValEeprom, *dryValEeprom, *periodValEeprom;


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
Button set (Gpio::C, 3, Button::twoAction);
Button plus (Gpio::B, 5, Button::oneAction);
Button minus (Gpio::B, 4, Button::oneAction);
Buffer value;
Adc sensor(Adc::channel2);
Atimer adcTrigger (16000);
Pin triac (Gpio::D, 1, Gpio::lowSpeed);


struct flags
{
  unsigned setLongPress : 1;
  unsigned setShortPress : 2;
  unsigned screens :2;
  unsigned dry :1;
  unsigned alarm : 1;
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
void clearCursor ();
void setCursor ();

uint16_t adcData [10];

INTERRUPT_HANDLER(adc, ADC1_EOC_vector)
{
  static uint8_t i=0;
  uint16_t result=0;
  static uint16_t dryCounter = 0;
  sensor.clearEoc ();
  adcTrigger.clearFlag ();
  TIM1->SR1 &= ~TIM1_SR1_TIF ;
  sensor.getBuffer (adcData);
  for (uint8_t i=0;i<10;++i)
  {
    result += adcData[i];
  }
  currPress.value = result/50;
  ++i;
  //draw value
  //main screen
  if (i>2)
  {
    lcd.setPosition (currPress.pos.row, currPress.pos.coloumn);
    value.parsDec16 (currPress.value, 2);
    lcd.sendString (1, value.getElement(3));
    lcd.data ('.');
    lcd.sendString (1, value.getElement(4));
  }

  //определение сухого хода
  if (currPress.value<dryPress.value&&!flag.dry)
  {
    flag.dry = 1;
    triac.clear ();
  } 
  if (currPress.value>dryPress.value) flag.dry = 0;
  
  //работа сухого хода
  if (flag.dry) 
  {
    ++dryCounter;
    if (dryCounter>(period.value*600)&&!flag.alarm)
    {
      triac.set ();
      flag.alarm = 1;
      dryCounter = 0;
    }
    else if (dryCounter>200&&flag.alarm)
    {
      triac.clear ();
      flag.alarm = 0;
      dryCounter = 0;
    }
  }
    //включение и выключение по верхнему и нижнему уровней
  if (!flag.dry)
  {
    if (currPress.value<lowPress.value)
    {
      triac.set ();
    }
    else if (currPress.value>highPress.value)
    {
      triac.clear ();
    }      
  }
}

INTERRUPT_HANDLER(TIM4_OVR_UIF, TIM4_OVR_UIF_vector)
{
  timer4.clearFlag();
  
  
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
}

void timer4_init ();

int main()
{
  FLASH->DUKR = 0xAE;
  FLASH->DUKR = 0x56;
  highValEeprom = (uint8_t*)0x004000; 
  lowValEeprom = (uint8_t*)0x004001; 
  dryValEeprom = (uint8_t*)0x004002; 
  periodValEeprom = (uint8_t*)0x004003; 
 
  //Should comment after 1st init
  //============================
 /* *highValEeprom = highVal; 
  *lowValEeprom = lowVal; 
  *dryValEeprom = dryVal; 
  *periodValEeprom = periodVal; */
  //============================
  
  mainScreen ();
  set1Screen ();
  set2Screen ();
  set.setLongLimit (1000);
  set.setShortLimit (10); 
  plus.setShortLimit (3);
  minus.setShortLimit (3);
  
  set.setlongPressAction (longSetPress);
  set.setshortPressAction (shortSetPress);
  
  plus.setshortPressAction (plusPress);
  minus.setshortPressAction (minusPress);
  
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
  currPress.pos.row = 1;
  highPress.value = *highValEeprom;
  highPress.highLimit = 40;
  highPress.lowLimit = 20;
  highPress.eepromPtr = highValEeprom;
  highPress.pos.coloumn = 12;
  highPress.pos.row = 0;
  lowPress.value = *lowValEeprom;
  lowPress.highLimit = 25;
  lowPress.lowLimit = 10;
  lowPress.pos.coloumn = 12;
  lowPress.pos.row = 1;
  lowPress.eepromPtr = lowValEeprom;
  dryPress.value = *dryValEeprom;
  dryPress.highLimit = 9;
  dryPress.lowLimit = 0;
  dryPress.pos.coloumn = 20;
  dryPress.pos.row = 0;
  dryPress.eepromPtr = dryValEeprom;
  period.value = *periodValEeprom;
  period.pos.coloumn = 20;
  period.pos.row = 1;
  period.lowLimit = 1;
  period.highLimit = 20;
  period.eepromPtr = periodValEeprom;
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
  lcd.setPosition (0, 17);
  lcd.sendString ("#P=");
  lcd.setPosition (1, 18);
  lcd.sendString ("T=");
}

void longSetPress ()
{
  if (flag.setLongPress) 
  {
    flag.setLongPress = 0;
    clearCursor ();
  }
  else 
  {
    if (ScreenCursor [flag.screens][flag.setShortPress] != 0)
    {
      flag.setLongPress = 1;
      flag.setShortPress = 0;
      setCursor ();
    }
  }
}

void shortSetPress ()
{
  if (!flag.setLongPress) 
  {
    ++flag.screens;
    if (flag.screens>2)flag.screens = 0;
    lcd.setShiftPosition (screens [flag.screens]);
    if (flag.screens)
    {     
      if (flag.screens ==2)
      {
        lcd.setPosition (ScreenVal[flag.screens][0]->pos.row, ScreenVal[flag.screens][0]->pos.coloumn);
        value.parsDec16 (ScreenVal[flag.screens][0]->value, 2);
        lcd.sendString (1, value.getElement(3));
        lcd.data ('.');
        lcd.sendString (1, value.getElement(4));
        lcd.setPosition (ScreenVal[flag.screens][1]->pos.row, ScreenVal[flag.screens][1]->pos.coloumn);
        value.parsDec16 (ScreenVal[flag.screens][1]->value, 2);
        lcd.sendString (1, value.getElement(3));
      }
      else
      {
        for (uint8_t i=0;i<2;++i)
        {   
          lcd.setPosition (ScreenVal[flag.screens][i]->pos.row, ScreenVal[flag.screens][i]->pos.coloumn);
          value.parsDec16 (ScreenVal[flag.screens][i]->value, 2);
          lcd.sendString (1, value.getElement(3));
          lcd.data ('.');
          lcd.sendString (1, value.getElement(4));
        }
      }
    }
  }
  else
  {
    clearCursor ();
    flag.setShortPress ^= 1;
    setCursor ();
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
      {
        --ptr->value;
        *ptr->eepromPtr = ptr->value;
      }
      lcd.setPosition (ScreenVal[flag.screens][flag.setShortPress]->pos.row, ScreenVal[flag.screens][flag.setShortPress]->pos.coloumn);
      value.parsDec16 (ScreenVal[flag.screens][flag.setShortPress]->value, 2);
      if (flag.screens==2&&flag.setShortPress==1)
      {
        lcd.sendString (2, value.getElement(3));
      }
      else
      {     
        lcd.sendString (1, value.getElement(3));
        lcd.data ('.');
        lcd.sendString (1, value.getElement(4));
      }
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
      {
        ++ptr->value;
        *ptr->eepromPtr = ptr->value;
        
      }
      lcd.setPosition (ScreenVal[flag.screens][flag.setShortPress]->pos.row, ScreenVal[flag.screens][flag.setShortPress]->pos.coloumn);
      value.parsDec16 (ScreenVal[flag.screens][flag.setShortPress]->value, 2);
      if (flag.screens==2&&flag.setShortPress==1)
      {
        lcd.sendString (2, value.getElement(3));
      }
      else
      {     
        lcd.sendString (1, value.getElement(3));
        lcd.data ('.');
        lcd.sendString (1, value.getElement(4));
      }
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

void clearCursor ()
{
  lcd.setPosition (ScreenCursor[flag.screens][flag.setShortPress]->row, ScreenCursor[flag.screens][flag.setShortPress]->coloumn);
  lcd.data (' ');
}

void setCursor ()
{
  lcd.setPosition (ScreenCursor[flag.screens][flag.setShortPress]->row, ScreenCursor[flag.screens][flag.setShortPress]->coloumn);
  lcd.data (cursor);
}
