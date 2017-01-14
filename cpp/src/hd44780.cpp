#include "hd44780.h"

#ifdef bit8
Hd44780::Hd44780()
:d0 (hd44780Def::d0port, hd44780Def::d0pin, Gpio::lowSpeed), d1 (hd44780Def::d1port, hd44780Def::d1pin, Gpio::lowSpeed), d2 (hd44780Def::d2port, hd44780Def::d2pin, Gpio::lowSpeed), d3 (hd44780Def::d3port, hd44780Def::d3pin, Gpio::lowSpeed), 
d4 (hd44780Def::d4port, hd44780Def::d4pin, Gpio::lowSpeed), d5 (hd44780Def::d5port, hd44780Def::d5pin, Gpio::lowSpeed), d6 (hd44780Def::d6port, hd44780Def::d6pin, Gpio::lowSpeed), d7 (hd44780Def::d7port, hd44780Def::d7pin, Gpio::lowSpeed), 
e (hd44780Def::eport, hd44780Def::epin, Gpio::lowSpeed), rs (hd44780Def::rsport, hd44780Def::rspin, Gpio::lowSpeed), rw (hd44780Def::rwport, hd44780Def::rwpin, Gpio::lowSpeed)
{
  d[0] = &d0;
  d[1] = &d1;
  d[2] = &d2;
  d[3] = &d3;
  d[4] = &d4;
  d[5] = &d5;
  d[6] = &d6;
  d[7] = &d7;  
  
  init();
  position=0;
}


void Hd44780::init ()
{
  rs.clear();	//запись комманд
  rw.clear();	//запись в ЖКИ
	//8bit
  delay_ms (16);
  tetra (0x08);
  delay_ms (5);
  tetra (0x38);
  delay_us (110);
  tetra (0x38);
  delay_ms (1);
  tetra (0x38);
  delay_ms (1);
  clearDram();
  delay_ms (1);
  tetra (0x0C);
  delay_ms (1);
  tetra (0x06);
  delay_ms (1);
}

void Hd44780::setData (uint8_t value)
{
  for (uint8_t i =0;i<8;++i)
  {
    if (value&1<<i) d[i]->set ();
    else d[i]->clear ();
  }
}

void Hd44780::command (uint8_t com)
{
  checkBusy ();
  rs.clear();
  tetra (com);
}

void Hd44780::data (char b)
{ 
  checkBusy();
  rs.set ();
  tetra (b);
}

void Hd44780::checkBusy ()
{
  d7.setIn (Gpio::Pullup);
  rw.set ();
  rs.clear();
  uint8_t state;
  do
  {
    e.set();
    delay_us(2);
    state = d7.state();
    e.clear();
  }
  while (state);
  d7.setOut ();
  rw.clear();
}

#else
Hd44780::Hd44780()
:d4 (hd44780Def::d4port, hd44780Def::d4pin, Gpio::lowSpeed), d5 (hd44780Def::d5port, hd44780Def::d5pin, Gpio::lowSpeed), d6 (hd44780Def::d6port, hd44780Def::d6pin, Gpio::lowSpeed), d7 (hd44780Def::d7port, hd44780Def::d7pin, Gpio::lowSpeed), 
e (hd44780Def::eport, hd44780Def::epin, Gpio::lowSpeed), rs (hd44780Def::rsport, hd44780Def::rspin, Gpio::lowSpeed), rw (hd44780Def::rwport, hd44780Def::rwpin, Gpio::lowSpeed)
{

  d[0] = &d4;
  d[1] = &d5;
  d[2] = &d6;
  d[3] = &d7;

  init();
  position=0;
}


void Hd44780::init ()
{
  delay_ms (15);
  tetra (0x03);
  delay_ms (5);
  tetra (0x03);
  delay_us (110);
  tetra (0x03);
  delay_ms (1);
  tetra (0x02);
  delay_ms (1);

  // 2 strings
  initCommand (0x28);
  delay_ms (1);
  //turn on display
  initCommand (0x0C);
  delay_ms (1);
  initCommand (0x06);
  delay_ms (1);
  clear();
}

void Hd44780::setData (uint8_t value)
{
  for (uint8_t i =0;i<4;++i)
  {
    if (value&1<<i) d[i]->set ();
    else d[i]->clear ();
  }
}

void Hd44780::command (uint8_t com)
{
  checkBusy ();
  rs.clear();
  tetra (com>>4);
  delay_us(50);
  tetra (com);
}

void Hd44780::data (char b)
{ 
  checkBusy();
  rs.set ();
  tetra (b>>4);
  delay_us(2); 
  tetra (b);
  delay_us(2);
}

void Hd44780::checkBusy ()
{
  d7.setIn (Gpio::Pullup);
  rw.set ();
  rs.clear();
  uint8_t state;
  do
  {
    e.set();
    delay_us(2);
    state = d7.state();
    e.clear();
    delay_us(1);
    e.set();
    delay_us(2);
    e.clear();
  }
  while (state);
  d7.setOut ();
  rw.clear();
}

#endif

void Hd44780::tetra (uint8_t t)
{
  e.set ();
  setData (t);
  delay_us (2);
  e.clear ();
}




void Hd44780::initCommand (uint8_t com)
{
  rw.clear();
  rs.clear();
  e.clear();
  tetra (com>>4);
  delay_us(50);
  tetra (com);
}



void Hd44780::sendString (const char *str)
{
  while (*str) data (*str++);
}

void Hd44780::sendString (uint8_t n, const char *str)
{
  for (uint8_t i=0;i<n;++i) data (*str++);
}

void Hd44780::clearDram ()
{
  command (clear_dram);
  delay_us (1500);
}

void Hd44780::setPosition (uint8_t col, uint8_t row)
{
  uint8_t addr = second_col*col + row;
  command (addr|set_dram_addr);
}



void Hd44780::newChar (const char *ch, uint8_t addr)
{
  command ((set_cgram_addr+addr*8));
  for (uint8_t i=0;i<8;++i, ch++) data (*ch);
}

void Hd44780::setShiftPosition (uint8_t pos)
{
  command (clear_counter);
  delay_ms (2);
  for (uint8_t i=0; i< pos;++i)command (shift|Left|Window);
  position = pos;
}
  
uint8_t & Hd44780::getShiftPosition ()
{
  return position;
}
