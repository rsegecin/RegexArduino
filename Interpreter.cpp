#include "Interpreter.h"

void InterpreterClass::init(SerialInterpreterClass * serial)
{
	lSerialInterpreter = serial;
}

void InterpreterClass::Process(char * str, char * regex)
{
	MatchState ms(str);

	lSerialInterpreter->Send("Processing");

	bind_regex_member<InterpreterClass, &InterpreterClass::MatchAddressCallback, 0> b(this);
	ms.GlobalMatch(regex, b);
}

void InterpreterClass::MatchAddressCallback(const char * match, const unsigned int length, const MatchState & ms)
{
	if (ms.level > 0)
	{
		sprintf(buffer, "Matches %u", ms.level);
		lSerialInterpreter->Send(buffer);

		for (uint8_t i = 0; i < ms.level; i++)
		{
			ms.GetCapture(buffer, i);
			lSerialInterpreter->Send(buffer);
		}
	}
	else
	{
		lSerialInterpreter->Send("No matches found.");
	}
}

InterpreterClass Interpreter;
