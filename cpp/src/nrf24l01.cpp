#include "nrf24l01.h"

uint8_t nrf24l01::self_addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

uint8_t nrf24l01::remote_addr[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};

nrf24l01::nrf24l01 ()
:spi1 (spi::div64) , pin (Gpio::C) , irq (intrpt::A , irq_ , intrpt::falling)
{
  chan = 3;
  delay_ms (15);
  pin.setOutPin (ce_);
  pin.setInPin (irq_);
  if (init ())
  {
    delay_ms (15);
    change_bit (CONFIG, PWR_UP, 1);
    delay_ms (3);
    startup = true;
  }
  else startup = false;
}

void nrf24l01::set_state (mode st)
{
  
  //переход в режим standby-1
  pin.clearPin (ce_);
  
  //переключаемся между режимами меняя PRIM_RX бит
  change_bit (CONFIG, PRIM_RX, st);
  
  //переходим в один из режимов
  pin.setPin (ce_);
  delay_us(15);
  //if(!st) pin.clearPin (ce_);
  delay_us(135);
}

void nrf24l01::rx_state ()
{
  //переходим в standby-1
  pin.setPin (ce_);
  //переключение в RX Mode
  change_bit (CONFIG, PRIM_RX, 1);
  pin.setPin (ce_);
  delay_us(140);
}

void nrf24l01::tx_state ()
{
  //переходим в standby-1
  stanby1_state();
  //переключение в TX Mode
  change_bit (CONFIG, PRIM_RX, 0);
  pin.setPin (ce_);
  delay_us(15);
  pin.clearPin (ce_);
  delay_us(135);
}

uint8_t nrf24l01::command (uint8_t cmd_)
{
  spi1.Clear_CS();
  spi1.put_data (cmd_);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  spi1.Set_CS ();
  return status;
}

uint8_t nrf24l01::w_data (uint8_t data_)
{
  spi1.Clear_CS();
  spi1.put_data (W_TX_PAYLOAD);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  spi1.put_data (data_);
  while (spi1.flag_bsy ());
  spi1.Set_CS ();
  return status;
}

uint8_t nrf24l01::w_reg (uint8_t reg , uint8_t val)
{
  spi1.Clear_CS();
  spi1.put_data (W_REGISTER|reg);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  while (!spi1.flag_txe());
  spi1.put_data (val); 
  while (!spi1.flag_rxne());
  uint8_t reg_val = spi1.get_data();
  spi1.Set_CS ();
  return reg_val;     
}

uint8_t nrf24l01::w_reg_buf (uint8_t reg , uint8_t * buf, uint8_t count_)
{
  spi1.Clear_CS();
  spi1.put_data (W_REGISTER|reg);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  while (count_--)
  {
    while (!spi1.flag_txe());
    spi1.put_data (*(buf++));
  }
  spi1.Set_CS ();
  return status;
}

uint8_t nrf24l01::r_reg_buf (uint8_t reg , uint8_t * buf, uint8_t count_)
{
  spi1.Clear_CS();
  spi1.put_data (R_REGISTER|reg);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  while (count_--)
  {
    while (!spi1.flag_txe());
    spi1.put_data (NOP);
    while (!spi1.flag_rxne());
    *(buf++) = spi1.get_data();
  }
  spi1.Set_CS ();
  return status;
}

uint8_t nrf24l01::r_reg (uint8_t reg)
{
  spi1.Clear_CS();
  spi1.put_data (R_REGISTER|reg);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  while (!spi1.flag_txe());
  spi1.put_data (NOP); 
  while (!spi1.flag_rxne());
  uint8_t reg_val = spi1.get_data();
  spi1.Set_CS ();
  return reg_val;   
}

bool nrf24l01::send_data (uint8_t * buf, uint8_t size)
{
  //переход в режим standby-1
  pin.clearPin (ce_);
  uint8_t conf = r_reg (CONFIG);
  
  // Сбрасываем бит PRIM_RX, и включаем питание установкой PWR_UP
  w_reg (CONFIG, (conf & ~(1 << PRIM_RX)) | (1 << PWR_UP));
  uint8_t status = get_status ();
  
  // Если очередь передатчика заполнена, возвращаемся с ошибкой
  if (status & (1 << TX_FULL)) return false;
  
  // Если питание не было включено, то ждём, пока запустится осциллятор
  if (!(conf & (1 << PWR_UP))) delay_ms(2); 
  
  // Запись данных на отправку
  w_reg_buf (W_TX_PAYLOAD, buf, size);
  
  // Импульс на линии CE приведёт к началу передачи
  pin.setPin (ce_);
  
  // Нужно минимум 10мкс
  delay_us(15); 
  
  pin.clearPin (ce_);
  return true;
}

void nrf24l01::send_byte (uint8_t data_)
{
  stanby1_state();
  //uint8_t status = get_status ();
  // Если очередь передатчика заполнена, возвращаемся с ошибкой
  //if (status & (1 << TX_FULL)) return false;
  //w_data (data_);
/*  w_reg (W_TX_PAYLOAD, data_);
  tx_state ();
  while (pin.pin_state (irq_));
  uint8_t s = get_status ();
  w_reg (STATUS, s);*/
  spi1.Clear_CS();
  spi1.put_data (W_TX_PAYLOAD);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  while (!spi1.flag_txe());
  spi1.put_data (data_);
  while (!spi1.flag_bsy());
  spi1.Set_CS ();
  tx_state ();
  
}

uint8_t nrf24l01::get_status ()
{
  spi1.Clear_CS();
  spi1.put_data (NOP);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  spi1.Set_CS ();
  return status; 
}

void nrf24l01::change_bit (uint8_t reg, uint8_t bit, bool state)
{
  uint8_t val = r_reg (reg);
  
  if (state) val = val|(1 << bit);
  else val = val&(~(1 << bit));
  w_reg (reg, val);
}

bool nrf24l01::init ()
{
   for(uint8_t i = 0;i<100;++i) 
   {
    w_reg(CONFIG, (/*1 << EN_CRC) | (1 << CRCO) |*/ (1 << PRIM_RX))); // Выключение питания
    if (r_reg(CONFIG) == ((/*1 << EN_CRC) | (1 << CRCO) |*/ (1 << PRIM_RX))) )
                           
    {
      count = i;
      return true;
    }
      // Если после 100 попыток не удалось записать что нужно, то выходим с ошибкой
    delay_ms(1);
   }
  return false;
}

uint8_t nrf24l01::receive_byte ()
{
  spi1.Clear_CS();
  spi1.put_data (R_RX_PAYLOAD);
  while (!spi1.flag_rxne());
  uint8_t status = spi1.get_data();
  while (!spi1.flag_txe());
  spi1.put_data (NOP); 
  while (!spi1.flag_rxne());
  uint8_t value = spi1.get_data();
  spi1.Set_CS ();
  return value;
}

uint8_t nrf24l01::check_radio ()
{
 /* if (!irq.check_int(irq_)) return 0;
  uint8_t status = get_status ();
  w_reg (STATUS, status);
  // Завершена передача успехом, или нет,
  if (status & (1 << TX_DS)) return 1;
  if (status & (1 << RX_DR)) return receive_byte ();
  else return 2;*/
}


