//************************************************************//
//      PERIPH.H
//  Заголовочный файл модуля управления периферией.
//  Библиотека для работы с контроллером "Рябина2.0"
//
//  Разработчик: Богдан Семухин
//************************************************************//

#pragma once
#include <stdint.h>
#include <Arduino.h>

class DC
{
	private:	
	uint8_t pinDC;
	
	public:
	explicit DC (uint8_t number);
	bool Read (void);
};
extern DC contact1;
extern DC contact2;
extern DC contact3;
extern DC contact4;

class Buzzer
{
	private:
	uint8_t pinBuzzer=12;
	
	public:
	void bzz(uint8_t PWM_BZZ);
  void silence(void);
	
};
extern Buzzer buzzer;

