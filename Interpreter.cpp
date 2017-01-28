#include "Interpreter.h"

void InterpreterClass::init()
{
	MatchState ms("txtIpAddress=192.168.0.213");

	bind_regex_member<InterpreterClass, &InterpreterClass::MatchAddressCallback, 0> b(this);
	ms.GlobalMatch("txtIpAddress=(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)", b);
}

void InterpreterClass::MatchAddressCallback(const char * match, const unsigned int length, const MatchState & ms)
{
	char cap[10];
	int aux;
	bool valid = true;
	
	if (ms.level == 4)
	{
		for (byte i = 0; i < ms.level; i++)
		{
			ms.GetCapture(cap, i);
			aux = atoi(cap);
	
			if ((aux < 0) || (255 < aux))
				valid = false;
		}
	
		if (valid)
		{
	        Serial.println("found match");
		}
	}
}

InterpreterClass Interpreter;

