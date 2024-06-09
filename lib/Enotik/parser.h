//************************************************************//
//  Разработчик: Семухин Богдан
//************************************************************//
//1 - ответ от Slave. Старший бит в байте id
//0 - пакет от мастера
#pragma once
#include <avr/io.h>
#include <Arduino.h>
#include "Enotik_message_format.h"



class Parser
{
  public:
    void parse (byte data);
    void init_master (byte id_master);
    void init_slave  (byte id_slave); 


  private:
    byte id = 0;
    byte count_data = 0;
    byte message[ENOTIK_MESSAGE_LENGTH] = {0};

    void shiftLeft(byte amount);

};



