#ifndef SerialInterpreter_h
#define SerialInterpreter_h

#define BAUD 19200
#define DEF_MSG_SIZE 200

#define SERIAL_RX				PD0
#define SERIAL_TX				PD1

#include <avr/io.h>
#include <util/setbaud.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "Communs.h"

class SerialInterpreterClass
{
public:
	enum eSerialCommands
	{
		nRegex, nNenhum
	};

	SerialInterpreterClass();

	volatile bool MessageReady = false;   // Serial message's flag
	volatile eSerialCommands MessageCommand = nNenhum;   // Tells what command should be executed
	char StrParameters[DEF_MSG_SIZE];

	void ClearBuffer(void);
	char *GetParameter(unsigned char index);
	void AddCommand(char * strCmdParam, eSerialCommands nCmdParam);
	void Send(const char *s);
	void OnInterrupt(char charParam);

private:
	char * usart_commands[nNenhum];
	void usart_tx(char c);
};

extern SerialInterpreterClass SerialInterpreter;

#endif