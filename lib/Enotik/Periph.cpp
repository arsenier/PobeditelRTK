//************************************************************//
//  Разработчик: Семухин Богдан
//************************************************************//
#include "Periph.h"

DC contact1(1);
DC contact2(2);
DC contact3(3);
DC contact4(4);

Buzzer buzzer;

DC::DC (uint8_t number)
{
  switch (number)
  {
    case (1):
      pinDC = 25;
      break;
    case (2):
      pinDC = 24;
      break;
	case (3):
      pinDC = 23;
      break;
	case (4):
      pinDC = 22;
      break;
  }
  pinMode(pinDC, INPUT);

}

bool DC::Read (void)
{ 
 return digitalRead(pinDC);
}

void Buzzer::bzz (uint8_t PWM_BZZ)
{
	tone(pinBuzzer,PWM_BZZ);
}
void Buzzer::silence(void)
{
  noTone(pinBuzzer);
}



