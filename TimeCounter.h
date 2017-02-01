#ifndef _RTCTIMER_h
#define _RTCTIMER_h

#include <avr/io.h>
#include <stdint.h>
#include "Communs.h"

#define DEBUG_PIN				PB1

class TimeCounterClass
{
public:
	struct sTimeSpan
	{
		volatile uint32_t Ticks;
		volatile uint16_t MiliSec;
		volatile uint8_t Seconds;
		volatile uint8_t Minutes;
		volatile uint8_t Hours;
		volatile uint16_t Days;
	};
	
	TimeCounterClass();

	volatile sTimeSpan TimeSpan;

	void OnInterrupt();
	void DelayMili(uint16_t pMili);
	void DelayMili(uint8_t pMili, bool &pFlag);
	void DelayMili(uint8_t pMili, bool &pFlag, void (*doWhile)(void));
	void ResetTimer();

private:
	bool tmpTest;
	uint32_t tmpTicks;
};

extern TimeCounterClass TimeCounter;

#endif

