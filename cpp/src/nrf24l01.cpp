#include "nrf24l01.h"

uint8_t Nrf24l01::self_addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

uint8_t Nrf24l01::remote_addr[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};

Nrf24l01::Nrf24l01 ()
:spi1 (Spi::div64) , cs (nrf24Def::csPort, nrf24Def::csPin, Gpio::Low) , ce (nrf24Def::cePort, nrf24Def::cePin, Gpio::Low), irq (intrpt::A , nrf24Def::irqPin, intrpt::falling)
{
  cs.set ();
  chan = 3;
  
  //===Standby-1 mode===//
  delay_ms (15);
  changeBit (CONFIG, PWR_UP, 1);
  delay_ms (2);
  writeRegister (RX_PW_P0, 1);
  rxState ();
  /*
  if (init ())
  {
    delay_ms (15);
    change_bit (CONFIG, PWR_UP, 1);
    delay_ms (3);
    startup = true;
  }
  else startup = false;*/
}

void Nrf24l01::rxState ()
{
  //������������ � RX Mode
  changeBit (CONFIG, PRIM_RX, 1);
  ce.set();
  delay_us(140);
}

void Nrf24l01::txState ()
{
  ce.clear ();
  changeBit (CONFIG, PRIM_RX, 0);
  ce.set ();
  delay_us(15);
  ce.clear ();
  delay_us(140);
}

void Nrf24l01::command (uint8_t com)
{
  cs.clear ();
  spi1.putData(com);
  while (!spi1.flagRxne());
  uint8_t status = spi1.getData();
}

uint8_t Nrf24l01::readRegister (uint8_t reg)
{
  cs.clear ();
  spi1.putData(R_REGISTER|reg);
  while (!spi1.flagRxne());
  uint8_t status = spi1.getData();
  while (!spi1.flagTxe());
  spi1.putData (NOP); 
  while (!spi1.flagRxne());
  uint8_t reg_val = spi1.getData();
  while (spi1.flagBsy ());
  cs.set ();
  return reg_val;   
}

void Nrf24l01::writeRegister (uint8_t reg , uint8_t val)
{
  cs.clear();
  spi1.putData (W_REGISTER|reg);
  while (!spi1.flagRxne());
  uint8_t status = spi1.getData();
  while (!spi1.flagTxe());
  spi1.putData (val); 
  while (spi1.flagBsy ());
  cs.set ();
}

void Nrf24l01::changeBit (uint8_t reg, uint8_t bit, bool state)
{
  uint8_t val = readRegister (reg);
  
  val = val&(~(1 << bit));
  val = val|(state << bit);
  writeRegister (reg, val);
}

void Nrf24l01::sendByte (uint8_t val)
{
  command (W_TX_PAYLOAD);
  //while (!spi1.flagTxe());
  spi1.putData (val); 
  while (spi1.flagBsy ());
  cs.set ();
  txState ();
  uint8_t temp = readRegister (STATUS);
  writeRegister (STATUS, temp);
  rxState ();
}

bool Nrf24l01::init ()
{
   for(uint8_t i = 0;i<100;++i) 
   {
   //writeRegister(CONFIG, 
    writeRegister(CONFIG, (/*1 << EN_CRC) | (1 << CRCO) |*/ (1 << PRIM_RX))); // ���������� �������
    if (readRegister(CONFIG) == ((/*1 << EN_CRC) | (1 << CRCO) |*/ (1 << PRIM_RX))) )
                           
    {
      count = i;
      return true;
    }
      // ���� ����� 100 ������� �� ������� �������� ��� �����, �� ������� � �������
    delay_ms(1);
   }
  return false;
}

uint8_t Nrf24l01::receiveByte ()
{
  cs.clear();
  spi1.putData (R_RX_PAYLOAD);
  while (!spi1.flagRxne());
  uint8_t status = spi1.getData();
  while (!spi1.flagTxe());
  spi1.putData (NOP); 
  while (!spi1.flagRxne());
  uint8_t value = spi1.getData();
  cs.set ();
  return value;
}



