#include <MicroTone.h>
//you can uncomment these two line if the delay() function cannot work properly
//#include <util/delay.h>
//#define delay _delay_ms;

void setup()
{
  MicroTone.begin();//if the frequency is too low give this function a "2" or "1" to fix that!
}

void loop()
{
  for (uint8_t sweep = 0; sweep < 10; sweep++)
  {
    for (uint8_t i = 55; i < 200; i++)
    {
      MicroTone.write(i);//like any write function the parameter of this function MUST be an 8-bit value (0-255)
      delay(1);
    }
    for (uint8_t j = 200; j > 55; j--)
    {
      MicroTone.write(j);
      delay(1);
    }
  }
  for (uint8_t Z = 0; Z < 6; Z++)//boooooobeeeeeeebooooooobeeeeeeebooooooobeeeeeee...
  {
    MicroTone.write(100);
    delay(500);
    MicroTone.write(60);
    delay(500);
  }
  for(uint8_t r =0 ;r<6;r++)// beep - beep - beep - beep - beep - beep...
  {
    MicroTone.write(50);
    delay(200);
    MicroTone.stop();
    delay(200);
  }
  for (uint8_t c = 0; c < 10; c++)
    for (uint8_t sweepBack = 55; sweepBack < 230; sweepBack++)
    {
      MicroTone.write(sweepBack);
      delay(2);
      delayMicroseconds(500);
    }
}