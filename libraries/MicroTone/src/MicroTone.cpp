#include "Arduino.h"
#include "MicroTone.h"
//thanks to FastLED library

#if ( defined(__AVR_ATtiny13__)||defined(__AVR_ATtiny10__))
   #define ATTINY1X
   #define MAX_PRESCALE 5
   #define DEFAULT_PRESCALE 3
#elif (defined(__AVR_ATmega8__))
   #define MAX_PRESCALE 7
   #define DEFAULT_PRESCALE 4
#endif
uint8_t prescale;

void MicroToneClass::begin (void)
{
  prescale = DEFAULT_PRESCALE;
  #if (defined(ATTINY1X))
  TIMSK0 = (0 << OCIE0B) | (0 << OCIE0A) | (0 << TOIE0);
  DDRB |=(1 << DDB0);
  #elif (defined(__AVR_ATmega8__))
  prescale = 4;
  DDRB|=(1<<DDB3);
  ASSR=0<<AS2;
  #endif
}

void MicroToneClass::begin(uint8_t myPrescale)
{
	if(myPrescale > MAX_PRESCALE)myPrescale = MAX_PRESCALE;
    prescale = myPrescale;
	#if (defined(ATTINY1X))
	//default = 3
	TIMSK0 = (0 << OCIE0B) | (0 << OCIE0A) | (0 << TOIE0);
    DDRB |=(1 << DDB0);
	#elif (defined(__AVR_ATmega8__))
    DDRB |= (1<<DDB3) ;
    ASSR=0<<AS2;
	
	#endif
}


void MicroToneClass::setPrescaler (uint8_t myAnotherPrescale)
{
    if(myAnotherPrescale > MAX_PRESCALE)myAnotherPrescale = MAX_PRESCALE;
    prescale = myAnotherPrescale;
	#if (defined(ATTINY1X))
	TCCR0B = prescale;
	#elif (defined(__AVR_ATmega8__))
	TCCR2 = (1<<COM20)|(1<<WGM21)|prescale;
	#endif
}

void MicroToneClass::write(uint8_t valOfOCR)
{
  #if (defined(ATTINY1X))
  if(TCCR0B != prescale)
  {
    TCCR0A = 0b01000010;
    TCCR0B = prescale;
  }
  OCR0A = valOfOCR;
  #elif (defined(__AVR_ATmega8__))
  uint8_t currentTCCR2 = (1<<COM20)|(1<<WGM21)|prescale;
  if (TCCR2 != currentTCCR2)
  {
	TCCR2 = currentTCCR2;
  }
  OCR2 = valOfOCR;
  #endif
}


void MicroToneClass::stop(void)
{
	#if (defined(__AVR_ATtiny13__))
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	#elif (defined(__AVR_ATmega8__))
	TCCR2 = 0;
	#endif
}

MicroToneClass MicroTone;
