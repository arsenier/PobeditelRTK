/**************************************************************************************
  Header. Содержит перечисление всех переменных и методов класса EnotikMaster.
  Разработчик: Семухин Богдан
**************************************************************************************/
#pragma once
#include <avr/io.h>
#include <Arduino.h>
#include "Enotik_message_format.h"
#include "parser.h"

//1- регистрация опкода
//2- вызов функции парсером
//


class EnotikMaster
{
    friend void serialEvent1();

  public:
    //задаем макксимальное количество регистрируемых opcode-ов и их обработчиков
    static const byte opcode_handlers_max = 30;
    //создание типа данных указатель на метод, принимающий 2 значащих байта.
    typedef void (*OpcodeHandler)(byte dataLow, byte dataHigh);

    void bind(byte opcode, OpcodeHandler handler);
    void on_message_received(byte opcode, byte dataLow, byte dataHigh);
    void work();
    void init_slave(byte slave_id);
    void init_master(byte slave_id);
    void send_message(byte opcode, byte dataLow, byte dataHigh);
    void set_answer_time(unsigned int answer_time);

  private:

    unsigned int time_answer = 1000;
    //выделяем память под пакет данных.
    byte message_data[ENOTIK_MESSAGE_LENGTH];
    void send_request();
    byte id = 0;
    //локальные переменные класса. Необходимы чтобы не вызывать метод обработки пакета в обработчике прерывания.
    byte last_opcode = 0;
    byte last_dataLow = 0;
    byte last_dataHigh = 0;
    unsigned long int last_answer_time = 0;
    bool is_answer_received = false;
    byte cur_opcode_count = 0;

    Parser parser;
    //описываем структуру, в которой хранится указатель на метод обработчика пакета и сам опкод.
    struct Item
    {
      byte opcode;
      OpcodeHandler handler;
    };
    //создаем массив таких структур.
    Item handlers[opcode_handlers_max];                                 //массив структур
};

extern EnotikMaster Enotik;


