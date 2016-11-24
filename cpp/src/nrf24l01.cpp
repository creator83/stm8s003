#include "nrf24l01.h"

uint8_t Nrf24l01::selfAddress[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

uint8_t Nrf24l01::remoteAddress[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};

Nrf24l01::Nrf24l01 ()
:spi1 (Spi::div64) , cs (nrf24Def::csPort, nrf24Def::csPin, Gpio::Low) , ce (nrf24Def::cePort, nrf24Def::cePin, Gpio::Low), irq (nrf24Def::irqPort , nrf24Def::irqPin, Intrpt::falling)
{
  cs.set ();
  chan = 3;
  //checking
  startup = init ();
  
  //settings register
  
  writeRegister(EN_AA, (1 << ENAA_P1)); // ��������� ����������������� ������ �� ������ 1
  writeRegister(EN_RXADDR, (1 << ERX_P0) | (1 << ERX_P1)); // ��������� ������� 0 � 1
  writeRegister(SETUP_AW, SETUP_AW_5BYTES_ADDRESS); // ����� ����� ������ 5 ����
  writeRegister(SETUP_RETR, SETUP_RETR_UP_TO_2_RETRANSMIT); 
  writeRegister(RF_CH, chan); // ����� ���������� ������
  writeRegister(RF_SETUP, RF_SETUP_0DBM); // ����� �������� 1 ����/� � �������� 0dBm
  
  writeRegister(RX_ADDR_P0, remoteAddress, 5); // ������������� �������� �� ����� 0 
  writeRegister(TX_ADDR, remoteAddress, 5);

  writeRegister(RX_ADDR_P1, selfAddress, 5);
  
  writeRegister(RX_PW_P0, 0);
  writeRegister(RX_PW_P1, 32); 
  writeRegister(DYNPD, (1 << DPL_P0) | (1 << DPL_P1)); // ��������� ������������ ����� ��� ������� 0 � 1
  writeRegister(FEATURE, 0x04); // ���������� ������������ ����� ������ ������
  
  
  //===Standby-1 mode===//
  delay_ms (15);
  writeRegister (CONFIG, (1 <<PWR_UP | 1 << EN_CRC));
  delay_ms (2);
  rxState ();
  __enable_interrupt ();
}

void Nrf24l01::rxState ()
{
  //������������ � RX Mode
  writeRegister (CONFIG, (1 <<PWR_UP | 1 << EN_CRC|1 << CRCO| 1 << PRIM_RX));
  //changeBit (CONFIG, PRIM_RX, 1);
  ce.set();
  delay_us(140);
}

void Nrf24l01::txState ()
{
  ce.clear ();
  writeRegister (CONFIG, (1 <<PWR_UP | 1 << EN_CRC));
  ce.set ();
  delay_us(15);
  ce.clear ();
  delay_us(140);
}

void Nrf24l01::command (uint8_t com)
{
  cs.clear ();
  nop ();
  spi1.putData(com);
}

void Nrf24l01::comm (uint8_t com)
{
  cs.clear ();
  nop ();
  spi1.putData(com);
  while (spi1.flagBsy ());
  cs.set ();
}

uint8_t Nrf24l01::readRegister (uint8_t reg)
{
  command (R_REGISTER|reg);
  while (!spi1.flagTxe());
  spi1.putData (NOP); 
  while (!spi1.flagRxne());
  uint8_t status = spi1.getData();  
  while (!spi1.flagRxne());
  uint8_t reg_val = spi1.getData();
  while (spi1.flagBsy ());
  cs.set ();
  return reg_val;   
}

uint8_t Nrf24l01::readStatus ()
{
  command (NOP);
  uint8_t status = spi1.getData();  
  while (!spi1.flagRxne());
  while (spi1.flagBsy ());
  cs.set ();
  return status;
}

void Nrf24l01::writeRegister (uint8_t reg , uint8_t val)
{
  command (W_REGISTER|reg);
  while (!spi1.flagTxe());
  spi1.putData (val); 
  while (spi1.flagBsy ());
  cs.set ();
}

void Nrf24l01::writeRegister (uint8_t reg , uint8_t * val, uint8_t count)
{
  command (W_REGISTER|reg);
  while (count--)
  {
    while (!spi1.flagTxe());
    spi1.putData (*val++); 
    while (spi1.flagBsy ());
    cs.set ();
  }
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
  while (!spi1.flagTxe());
  spi1.putData (val); 
  while (spi1.flagBsy ());
  cs.set ();
  txState ();
  rxState ();
}

bool Nrf24l01::init ()
{
   for(uint8_t i = 0;i<100;++i) 
   {
    writeRegister(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX)); // ���������� �������
    if (readRegister(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX)))
                           
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
  command (R_RX_PAYLOAD);
  while (!spi1.flagRxne());
  uint8_t status = spi1.getData();
  while (!spi1.flagTxe());
  spi1.putData (NOP); 
  while (!spi1.flagRxne());
  uint8_t value = spi1.getData();
  while (spi1.flagBsy ());
  cs.set ();
  return value;
}



