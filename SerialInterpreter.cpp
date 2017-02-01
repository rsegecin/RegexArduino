#include "SerialInterpreter.h"

void SerialInterpreterClass::ClearBuffer(void)
{
	MessageReady = false;
	MessageCommand = nNenhum;

	memset(StrParameters, 0x00, DEF_MSG_SIZE);
}

char* SerialInterpreterClass::GetParameter(unsigned char index)
{
	uint8_t num;
	uint8_t parameters = 0;
	char *ret_aux = NULL;

	for (num = 0; num < DEF_MSG_SIZE; num++)
	{
		if (StrParameters[num] != 0x00)
		{
			if (parameters == index)
			{
				ret_aux = &StrParameters[num];
			}

			while ((StrParameters[num] != 0x00) && (num < DEF_MSG_SIZE)) { num++; }

			parameters++;
		}
	}

	return ret_aux;
}

void SerialInterpreterClass::AddCommand(char * strCmdParam, eSerialCommands nCmdParam)
{
	usart_commands[nCmdParam] = strCmdParam;
}

void SerialInterpreterClass::usart_tx(char c)
{
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = c;
	//Serial.print(c);
}

void SerialInterpreterClass::Send(const char *s)
{
	set_output(DDRD, SERIAL_TX);
	set_bit(UCSR0B, TXEN0);

	usart_tx('\r');
	usart_tx('\n');

	while (*s != '\0')
	{
		usart_tx(*s++);
	}

	usart_tx('\r');
	usart_tx('\n');

	set_input(DDRD, SERIAL_TX);
	clear_bit(UCSR0B, TXEN0);
}

void SerialInterpreterClass::OnInterrupt(char charParam)
{
	static bool flagD = false;

	static char estado = 0;
	static bool emEspera;

	static uint8_t matchingCmd;
	char * strMatchingCmd;
	static uint8_t msgPosition;
	static uint8_t msgCmdPosition;

	static uint8_t countChar = 0;
	bool tmpBit;
	uint8_t num;
	char * strCmd;

	if (MessageReady == false)
	{
		switch (estado)
		{
		case 0:
			//valores das variáveis para entrar no estado 0 (preseted)
			emEspera = false;
			matchingCmd = 0;
			msgPosition = 0;
			msgCmdPosition = 0;

			if (charParam == 0x0D)
				flagD = true;
			else if ((flagD == true) && (charParam == 0x0A))
			{
				estado = 1;
				flagD = false;
			}
			else
				flagD = false;
			break;
		case 1:
			strMatchingCmd = (char *) usart_commands[matchingCmd];

			if (emEspera == false)
			{
				num = isalnum(charParam);

				if (num != 0)
				{
					if (charParam == strMatchingCmd[msgCmdPosition])
					{
						msgCmdPosition++;
					}
					else
					{
						//Verifica parte do comando
						tmpBit = false;

						for (num = matchingCmd + 1; num < nNenhum; num++)
						{
							strCmd = (char *) usart_commands[num];
							if ((strncmp(strMatchingCmd, strCmd, msgCmdPosition) == 0) && (charParam == strCmd[msgCmdPosition]))
							{
								matchingCmd = num;
								tmpBit = true;
								break;
							}
						}

						if (tmpBit == true)
						{
							//tem comando
							msgCmdPosition++;
						}
						else
						{
							//nenhum bateu
							emEspera = true;
							countChar = 0;
							matchingCmd = 0;
							msgCmdPosition = 0;
							num = 0;
						}
					}
				}
				else
				{
					if ((charParam == ',') || (charParam == ';'))
					{
						if (msgCmdPosition == strlen(usart_commands[matchingCmd]))
						{
							estado = 2;
							msgCmdPosition = 0;
						}
						else
						{
							//Verifica todo o comando
							tmpBit = false;

							for (num = matchingCmd + 1; num < nNenhum; num++)
							{
								strCmd = (char *) usart_commands[num];
								if ((strncmp(strMatchingCmd, strCmd, msgCmdPosition) == 0) && (strlen(strCmd) == msgCmdPosition))
								{
									matchingCmd = num;
									tmpBit = true;
									break;
								}
							}

							if (tmpBit == true)
							{
								//tem comando
								estado = 2;
								msgCmdPosition = 0;
							}
							else
							{
								//nenhum bateu, deve continuar no mesmo estado pois recebeu caractere de separação
								matchingCmd = 0;
								msgCmdPosition = 0;
							}
						}
					}
					else
					{
						emEspera = true;
						countChar = 0;
						num = 0;

						if (charParam == 0x0D)
							flagD = true;
					}
				}
			}
			else
			{ // Else do estado emEspera
				if ((charParam == ',') || (charParam == ';'))
				{
					emEspera = false;
					matchingCmd = 0;
					msgCmdPosition = 0;
				}
				else
				{
					if (charParam == 0x0D)
					{
						flagD = true;
					}
					else if ((flagD == true) && (charParam == 0x0A))
					{
						if (msgPosition > 2)
						{
							if (countChar == 0)
							{
								//Verifica todo o comando
								tmpBit = false;

								for (num = matchingCmd; num < nNenhum; num++)
								{
									strCmd = (char *) usart_commands[num];
									if ((strncmp(strMatchingCmd, strCmd, msgCmdPosition) == 0) && (strlen(strCmd) == msgCmdPosition))
									{
										matchingCmd = num;
										tmpBit = true;
										break;
									}
								}

								if (tmpBit == true)
								{
									MessageCommand = (eSerialCommands) matchingCmd;
									MessageReady = true;
								}
								else
								{
									MessageCommand = nNenhum;
								}
							}
							else
							{
								MessageCommand = nNenhum;
							}

							estado = 0;
						}

						matchingCmd = 0;
						msgCmdPosition = 0;
						emEspera = false;
						flagD = false;
					}
					else
					{
						flagD = false;
					}
				}
				countChar++;
			}
			break;
		case 2:
			if ((charParam == ',') || (charParam == ';'))
			{
				StrParameters[msgCmdPosition] = 0x00;
				msgCmdPosition++;
			}
			else if (charParam == 0x0D)
			{
				StrParameters[msgCmdPosition] = charParam;
				msgCmdPosition++;
				flagD = true;
			}
			else if ((flagD == true) && (charParam == 0x0A))
			{
				flagD = false;
				StrParameters[msgCmdPosition - 1] = 0x00;
				
				MessageReady = true;
				MessageCommand = (eSerialCommands) matchingCmd;

				matchingCmd = 0;
				msgCmdPosition = 0;
				emEspera = false;
				estado = 0;
			}
			else
			{
				StrParameters[msgCmdPosition] = charParam;
				msgCmdPosition++;
				flagD = false;
			}
			break;
		}

		msgPosition++;
	}
}

SerialInterpreterClass::SerialInterpreterClass()
{
	ClearBuffer();
}

SerialInterpreterClass SerialInterpreter;