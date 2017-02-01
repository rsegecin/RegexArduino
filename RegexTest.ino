#include "Interpreter.h"
#include "SerialInterpreter.h"
#include "TimeCounter.h"

char buffer[200];

ISR(TIMER1_COMPA_vect)
{
	TimeCounter.OnInterrupt();
}

void serialEvent()
{
	SerialInterpreter.OnInterrupt(Serial.read());
}

void setup()
{
	sei();
	Serial.begin(115200);
	
	SerialInterpreter.AddCommand("regex", SerialInterpreterClass::eSerialCommands::nRegex);

	SerialInterpreter.Send("Welcome");
	TimeCounter.DelayMili(1000);

	Interpreter.init(&SerialInterpreter);
}

void loop()
{
	SerialHandler();
}

void SerialHandler()
{
	char * lcParametro0 = nullptr;
	char * lcParametro1 = nullptr;

	char result;

	switch (SerialInterpreter.MessageCommand)
	{
	case SerialInterpreterClass::nRegex:
	{
		lcParametro0 = SerialInterpreter.GetParameter(0);
		lcParametro1 = SerialInterpreter.GetParameter(1);
		
		sprintf(buffer, "String: %s\nRegex: %s", lcParametro0, lcParametro1);
		SerialInterpreter.Send(buffer);

		//Interpreter.Process(lcParametro0, lcParametro1);
		MatchState ms(lcParametro0);

		ms.Match(lcParametro1);

		if (ms.level > 0)
		{
			sprintf(buffer, "Matches %u", ms.level);
			SerialInterpreter.Send(buffer);

			for (uint8_t i = 0; i < ms.level; i++)
			{
				ms.GetCapture(buffer, i);
				SerialInterpreter.Send(buffer);
			}
		}
		else
		{
			SerialInterpreter.Send("No matches found.");
		}
		
		SerialInterpreter.ClearBuffer();
		break;
	}
	default:
		break;
	}
}
