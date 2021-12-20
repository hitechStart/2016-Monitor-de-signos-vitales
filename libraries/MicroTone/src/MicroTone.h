#ifndef MicroTone_h
#define MicroTone_h
#ifdef __AVR__
  #include <avr/power.h>
#endif


class MicroToneClass
{
public:

void begin(void);
void begin(uint8_t myPrescale);
void write (uint8_t);
void stop (void);
void setPrescaler(uint8_t);



private:
uint8_t prescale;
uint8_t valOfOCR0A;
};

extern MicroToneClass MicroTone;

#endif