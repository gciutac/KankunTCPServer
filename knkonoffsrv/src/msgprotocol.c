/*
 * msgprotocol.c
 *
 *  Created on: Dec 20, 2014
 *      Author: GG
 */

#include "msgprotocol.h"

int ParseInputMessage( char *InputMsg, int InputMsgLength)
{
	char Buf[15];

	WRITE_DEBUG("ParseInputMessage - Enter\n");
	memset(Buf, 0, sizeof(Buf));
	memset(&ReadMsgStr, 0, sizeof(ReadMsgStr));

	if (InputMsgLength < 14)
	{
		WRITE_DEBUG("ParseInputMessage - Exit [0]\n");
		return 0;
	}

	/* Parse field 1, Message Type */
	memcpy(Buf, InputMsg, 4);
	ReadMsgStr.MsgType = atoi(Buf);

	if (ReadMsgStr.MsgType <9000)
	{
		WRITE_DEBUG("ParseInputMessage - Exit [-1]\n");
		return -1;
	}

	/* Parse field 2, Message Id */
	memcpy(ReadMsgStr.MsgId, InputMsg+4, 10);

	WRITE_DEBUG("ParseInputMessage - Exit\n");
	return 1;

}


