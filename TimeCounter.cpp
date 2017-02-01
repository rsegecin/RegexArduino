#include "TimeCounter.h"

TimeCounterClass::TimeCounterClass()
{
	//set timer1 interrupt at 1kHz
	TCCR1A = 0;		// set entire TCCR1A register to 0
	TCCR1B = 0;		// same for TCCR1B
	TCNT1 = 0;		//initialize counter value to 0

	// set compare match register for 8khz increments
	OCR1A = 999;	// = [(16*10^6) / (1000*8)] - 1 = (must be < 65536)
				
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS11 bits for 8 prescaler
	TCCR1B |= (1 << CS11);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	set_output(DDRB, DEBUG_PIN);

	tmpTest = true;
}

void TimeCounterClass::OnInterrupt()
{
	if (tmpTest)
	{
		tmpTest = false;
		set_bit(PINB, DEBUG_PIN);
	}
	else
	{
		tmpTest = true;
		clear_bit(PINB, DEBUG_PIN);
	}

	TimeSpan.MiliSec++;
	TimeSpan.Ticks++;

	if (TimeSpan.MiliSec >= 1000)
	{
		TimeSpan.MiliSec = 0;
		TimeSpan.Seconds++;
		if (TimeSpan.Seconds >= 60)
		{
			TimeSpan.Seconds = 0;
			TimeSpan.Minutes++;
			if (TimeSpan.Minutes >= 60)
			{
				TimeSpan.Minutes = 0;
				TimeSpan.Hours++;
				if (TimeSpan.Hours >= 24)
				{
					TimeSpan.Hours = 0;
					TimeSpan.Days++;
				}
			}
		}
	}
}

void TimeCounterClass::DelayMili(uint16_t pMili)
{
	tmpTicks = TimeSpan.Ticks + pMili;

	while (TimeSpan.Ticks <= tmpTicks);
}

void TimeCounterClass::DelayMili(uint8_t pMili, bool &pFlag)
{
	tmpTicks = TimeSpan.Ticks + pMili;

	while ((!pFlag) && (TimeSpan.Ticks <= tmpTicks));
}

void TimeCounterClass::DelayMili(uint8_t pMili, bool & pFlag, void(*doWhile)(void))
{
	tmpTicks = TimeSpan.Ticks + pMili;

	while ((!pFlag) && (TimeSpan.Ticks <= tmpTicks))
	{
		doWhile();
	}
}

void TimeCounterClass::ResetTimer()
{
	TimeSpan.MiliSec = 0;
	TimeSpan.Ticks = 0;
	TimeSpan.Seconds = 0;
	TimeSpan.Minutes = 0;
	TimeSpan.Hours = 0;
	TimeSpan.Days = 0;
}

TimeCounterClass TimeCounter;

