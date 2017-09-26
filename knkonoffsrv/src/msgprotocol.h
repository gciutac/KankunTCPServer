/*
 * msgprotocol.h
 *
 *  Created on: Dec 20, 2014
 *      Author: GG
 */

#ifndef MSGPROTOCOL_H_
#define MSGPROTOCOL_H_

#include "external.h"

int ParseInputMessage( char *InputMsg, int InputMsgLength);

typedef struct
{
	int MsgType;
	char MsgId[11];
	char MsgStatus[2];
	char AddInfo[50];
}MSGSTRUCT;

MSGSTRUCT ReadMsgStr;

#endif /* MSGPROTOCOL_H_ */
