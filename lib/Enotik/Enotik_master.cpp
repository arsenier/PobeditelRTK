/**************************************************************************************
В данном файле описан класс enotik_master. 
Разработчик: Семухин Богдан
 **************************************************************************************/
#include "Enotik_master.h"
#include "crc8.h"

EnotikMaster Enotik;


void serialEvent1()
{
  while (Serial3.available())
  {
    Enotik.parser.parse(Serial3.read());
  }
}


/**************************************************************************************
Метод bind класса EnotikMaster необходим для установки соотвествия опкода и пользовательским методом.
Принимаемые аргументы:

byte opcode - сам опкод, при обработке которого должен выполнятся метод пользователя

OpcodeHandler handle - указатель на пользовательский метод. Сам метод должен иметь два
получаемых аргумента - 2 полученных байта пакета dataLow и dataHigh
**************************************************************************************/


void EnotikMaster:: bind(byte opcode, OpcodeHandler handle)
{
  if(cur_opcode_count<opcode_handlers_max)
  {
    handlers[cur_opcode_count].handler=handle;
    handlers[cur_opcode_count].opcode=opcode;
    cur_opcode_count++;
  }
}



/**************************************************************************************
Метод on_message_received класса EnotikMaster необходим для локального сохранения трех
значащих байт пакета: opcode, dataLow, dataHigh
Принимаемые аргументы:

byte opcode - сам опкод, при обработке которого должен выполнятся метод пользователя

dataLow - первый значащий байт.

dataHigh - второй значащий байт.

**************************************************************************************/
void EnotikMaster:: on_message_received(byte opcode,byte dataLow,byte dataHigh)
{
  last_opcode=opcode;
  last_dataLow=dataLow;
  last_dataHigh=dataHigh;
  is_answer_received=true;
  last_answer_time=millis();
}

/**************************************************************************************
Метод work класса EnotikMaster необходим для вызова метода обработки полученных данных.
Запускает процесс обработки opcode и значащих байт методами пользователя.

Принимаемые аргументы: - 

**************************************************************************************/
void EnotikMaster:: work()
{
  //Часть метода, определяющая что делать при потере связи.
  if(millis()-last_answer_time>time_answer) 
  {
  }
  //Выход из метода, если не было полученных байт.
  if(is_answer_received==false)
  {
    return;
  }
  //Цикл поиска нужного опкода в массиве структур опкод-указатель на функцию
  for(byte i=0;i<opcode_handlers_max;i++)
  {
    if(last_opcode==handlers[i].opcode)
    {
      handlers[i].handler(last_dataLow,last_dataHigh);
      is_answer_received=false;
      return;
    }
  }
}

/**************************************************************************************
Метод send_request класса EnotikMaster необходим для формирования и отправки конечного 
пакета другому устройству. 

Принимаемые аргументы: -

**************************************************************************************/
void EnotikMaster:: send_request()
{
  message_data[ENOTIK_ID_OFFSET]=id;
  message_data[ENOTIK_CRC8_OFFSET]=crc8(message_data, ENOTIK_MESSAGE_LENGTH-1);
  Serial3.write(message_data,ENOTIK_MESSAGE_LENGTH);
  
}
/**************************************************************************************
Метод init_slave класса EnotikMaster необходим для инициализации устройства как slave
и присвоения устройству своего id. Slave id отличается от id мастера 1 в старшем бите 
байта id.
Метод init_master работает по тому же принципу, отличия в старшем бите байта id.

Принимаемые аргументы:

byte slave_id - назначаемый пользователем id подчиненного устройства.

**************************************************************************************/

void EnotikMaster:: init_slave(byte slave_id)
{ 
  // слейв принимает запросы
  parser.init_slave(slave_id);  

  // но отправляет ответы
  id = slave_id | ENOTIK_DIRECTION_MASK_ANSWER;
}

void EnotikMaster:: init_master(byte slave_id)
{
  // мастер принимает ответы
  parser.init_master(slave_id );  

  // но отправляет ответы
  id = slave_id;
}


/**************************************************************************************
Метод send_message класса EnotikMaster необходим для формирования значащей части отпра-
вляемой части пакета. 

Принимаемые аргументы:

byte opcode - сам опкод, при обработке которого должен выполнятся метод пользователя

dataLow - первый значащий байт.

dataHigh - второй значащий байт.

**************************************************************************************/
void EnotikMaster:: send_message(byte opcode,byte dataLow,byte dataHigh)
{
  message_data[ENOTIK_OPPCODE_OFFSET]=opcode;
  message_data[ENOTIK_DATALOW_OFFSET]=dataLow;
  message_data[ENOTIK_DATAHIGH_OFFSET]=dataHigh;
  //после формирования значащего пакета вызываем метод формирования полного пакета
  send_request();
}
/**************************************************************************************
Метод set_answer_time класса EnotikMaster необходим для установки максимального времени
отклика устройства. 
Принимаемые аргументы:

unsigned int answer_time - значение времени в миллисекундах.

**************************************************************************************/
void EnotikMaster:: set_answer_time(unsigned int answer_time)
{
  time_answer=answer_time;
}

