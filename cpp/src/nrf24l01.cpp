#include "nrf24l01.h"

uint8_t Nrf24l01::selfAddress[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

uint8_t Nrf24l01::remoteAddress[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};

Nrf24l01::Nrf24l01 (Pin & cs_, Pin & ce_, Intrpt & irq_)
:spi1 (Spi::div64) , cs (&cs_) , ce (&ce_), irq (&irq_){
  cs->set ();
  chan = 3;
  //checking
    //===Standby-1 mode===//
  delay_ms (15);
  cs->clear();
  spi1.transfer(0xF0);
  cs->set();
  writeRegister (CONFIG, (1 <<PWR_UP | 1 << EN_CRC));
  delay_ms (2);
  rxState ();
  readRegister (CONFIG);
  __enable_interrupt ();
  //startup = init ();
  
  //settings register
  command (FLUSH_TX);
  command (FLUSH_RX);
  uint8_t status = getStatus();
  writeRegister (STATUS, status);
  writeRegister(RX_PW_P0, 1);
  /*
  writeRegister(EN_AA, (1 << ENAA_P1)); // включение автоподтверждения только по каналу 1
  writeRegister(EN_RXADDR, (1 << ERX_P0) | (1 << ERX_P1)); // включение каналов 0 и 1
  writeRegister(SETUP_AW, SETUP_AW_5BYTES_ADDRESS); // выбор длины адреса 5 байт
  writeRegister(SETUP_RETR, SETUP_RETR_UP_TO_2_RETRANSMIT); 
  writeRegister(RF_CH, chan); // Выбор частотного канала
  writeRegister(RF_SETUP, RF_SETUP_0DBM); // выбор скорости 1 Мбит/с и мощности 0dBm
  
  writeRegister(RX_ADDR_P0, remoteAddress, 5); // Подтверждения приходят на канал 0 
  writeRegister(TX_ADDR, remoteAddress, 5);

  writeRegister(RX_ADDR_P1, selfAddress, 5);
  
  writeRegister(RX_PW_P0, 0);
  writeRegister(RX_PW_P1, 32); 
  writeRegister(DYNPD, (1 << DPL_P0) | (1 << DPL_P1)); // включение произвольной длины для каналов 0 и 1
  writeRegister(FEATURE, 0x04); // разрешение произвольной длины пакета данных
  */


}

uint8_t Nrf24l01::readData (){
  return false;
}

void Nrf24l01::rxState (){
  //переключение в RX Mode
  writeRegister (CONFIG, (1 <<PWR_UP | 1 << EN_CRC|/*1 << CRCO|*/ 1 << PRIM_RX));
  //changeBit (CONFIG, PRIM_RX, 1);
  ce->set();
  delay_us(140);
}

void Nrf24l01::txState (){
  ce->clear ();
  writeRegister (CONFIG, (1 <<PWR_UP | 1 << EN_CRC));
  ce->set ();
  delay_us(15);
  ce->clear ();
  delay_us(140);
}

uint8_t Nrf24l01::command (uint8_t com){
  cs->clear ();
  uint8_t status = spi1.transfer (com);
  while (spi1.flagBsy ());
  cs->set ();
  return status;
}

uint8_t Nrf24l01::readRegister (uint8_t reg){
  cs->clear ();
  uint8_t status = spi1.transfer (R_REGISTER|reg);
  while (!spi1.flagTxe());
  uint8_t reg_val = spi1.transfer(NOP);
  while (spi1.flagBsy ());
  cs->set ();
  return reg_val;   
}

void Nrf24l01::writeRegister (uint8_t reg , uint8_t val){
  cs->clear ();
  uint8_t status = spi1.transfer(W_REGISTER|reg);
  while (!spi1.flagTxe());
  spi1.transfer (val); 
  while (spi1.flagBsy ());
  cs->set ();
}

void Nrf24l01::writeRegister (uint8_t reg , uint8_t * val, uint8_t count){
  cs->clear ();
  uint8_t status = spi1.transfer(W_REGISTER|reg);
  while (count--)
  {
    while (!spi1.flagTxe());
    spi1.transfer (*val++);  
  }
  while (spi1.flagBsy ());
  cs->set ();
}

void Nrf24l01::changeBit (uint8_t reg, uint8_t bit, bool state){
  uint8_t val = readRegister (reg);
  val = val&(~(1 << bit));
  val = val|(state << bit);
  writeRegister (reg, val);
}

void Nrf24l01::sendByte (uint8_t val){
  cs->clear ();
  uint8_t status = spi1.transfer(W_TX_PAYLOAD);
  while (!spi1.flagTxe());
  spi1.transfer (val);
  while (spi1.flagBsy ());
  cs->set ();
  txState ();
  rxState ();
}

void Nrf24l01::sendData (uint8_t *data, uint8_t n){
  cs->clear ();
  uint8_t status = spi1.transfer(W_TX_PAYLOAD);
  while (n--)
  {
    while (!spi1.flagTxe());
    spi1.transfer (*data++);
  }
  while (spi1.flagBsy ());
  cs->set ();
  txState ();
  rxState ();  
}

uint8_t Nrf24l01::receiveByte (){
  cs->clear ();
  uint8_t status = spi1.transfer(R_RX_PAYLOAD);
  while (!spi1.flagTxe());
  uint8_t value = spi1.transfer (NOP);
  while (spi1.flagBsy ());
  cs->set ();
  return value;
}

uint8_t Nrf24l01::getStatus (){
  cs->clear ();
  uint8_t status = spi1.transfer (NOP);
  while (spi1.flagBsy ());
  cs->set ();
  return status;  
}

bool Nrf24l01::init ()
{
   for(uint8_t i = 0;i<100;++i) 
   {
    writeRegister(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX)); // Выключение питания
    if (readRegister(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX)))
                           
    {
      count = i;
      return true;
    }
      // Если после 100 попыток не удалось записать что нужно, то выходим с ошибкой
    delay_ms(1);
   }
  return false;
}





