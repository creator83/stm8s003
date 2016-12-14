#include "stm8s.h"               // Device header
#include "gpio.h"
#include "delay.h"

#ifndef HD44780_H
#define HD44780_H


namespace hd44780Def
{
  //D4
  const Gpio::Port d4port = Gpio::C;
  const uint8_t d4pin = 4;
  //D5
  const Gpio::Port d5port = Gpio::C;
  const uint8_t d5pin = 5;
  //D6
  const Gpio::Port d6port = Gpio::C;
  const uint8_t d6pin = 6;
  //D7
  const Gpio::Port d7port = Gpio::C;
  const uint8_t d7pin = 7;
  //E
  const Gpio::Port eport = Gpio::D;
  const uint8_t epin = 4;
  //RS
  const Gpio::Port rsport = Gpio::D;
  const uint8_t rspin = 6;
  //RW
  const Gpio::Port rwport = Gpio::D;
  const uint8_t rwpin = 5;
}


//COMMANDS

const uint8_t clear_dram = 0x01;
const uint8_t clear_counter = 0x02;
//shift command & option
const uint8_t shift = 0x10;


//settings display command

const uint8_t display_settings = 0x08;

const uint8_t turn_off_display = 0x08;
const uint8_t turn_on_display = 0x0C;


const uint8_t turn_off_cursor = 0x0C;
const uint8_t turn_on_blink = 0x0D;
const uint8_t turn_on_cursor = 0x0E;
const uint8_t blink_cursor = 0x0F;

const uint8_t set_dram_addr = 0x80;
const uint8_t second_col = 0x40;

const uint8_t set_cgram_addr = 0x40;

class Hd44780
{
//variables
public:
	enum Direction {Left = 0, Right= 0x04};
	enum Shifter {Cursor=0, Window=0x08};
	
protected:
private:
	uint8_t position;
        Gpio d4, d5, d6, d7, e, rs, rw;
        Gpio * d[4];
        uint8_t pins [4];
	unsigned int x_start, x_end, y_start, y_end;

//functions
public:
  Hd44780();
  void init ();
  void tetra (uint8_t t);
  void command (uint8_t com);
  void initCommand (uint8_t com);
  void data (char data) ;
  void sendString (const char *str) ;
  void sendString (uint8_t n, const char *str) ;
  void clear ();
  void setPosition (uint8_t col, uint8_t row);
  void newChar (const char *ch, uint8_t addr);
  void rsAssert ();
  void rsDisassert ();
  void eAssert ();
  void eDisassert ();
  void rwAssert ();
  void rwDisassert ();
  void checkBusy ();
  void Shift (Shifter s, Direction d, uint8_t val);
  void Shift_simple (Shifter s, Direction d, uint8_t val);
  void setShiftPosition (uint8_t pos);
  uint8_t & getShiftPosition ();
protected:
private:
  void clearData ();
  void setData (uint8_t);
};






#endif
