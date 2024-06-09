/**************************************************************************************
 В данном файле хранятся местоположения байтов в отправляемом пакете, а также длина 
 самого пакета и количество сдвигаемых бит при установке идентификационного master/slave
 бита в id устройства.
 Разработчик: Семухин Богдан
 **************************************************************************************/


#pragma once

#define ENOTIK_DIRECTION_BIT           7
#define ENOTIK_DIRECTION_MASK_REQUEST  (0<<ENOTIK_DIRECTION_BIT)
#define ENOTIK_DIRECTION_MASK_ANSWER   (1<<ENOTIK_DIRECTION_BIT)

#define ENOTIK_ID_OFFSET        0
#define ENOTIK_OPPCODE_OFFSET   1
#define ENOTIK_DATALOW_OFFSET   2
#define ENOTIK_DATAHIGH_OFFSET  3
#define ENOTIK_CRC8_OFFSET      4

#define ENOTIK_MESSAGE_LENGTH   5
