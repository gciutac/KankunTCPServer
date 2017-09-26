#include "tcpsrv.h"

/**************************************************************
TCPIP Server listen on Port
***************************************************************/
int ListenOnPort()
{
	int lerrno = 0;
	int on = 1;

	WRITE_DEBUG("\nListenOnPort - Enter\n");

	/* Create Socket */
	if ((Socket=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		lerrno = errno;
		WRITE_DEBUG("\nTCPIP, Unable to Create Socket");
		return FALSE;
	}

	/* Binding Socket, Any Local Address */
	/* but specified port */
	   memset(&servaddr,0,sizeof(servaddr));
	   servaddr.sin_family = AF_INET;
	   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	   servaddr.sin_port=htons(PortNumber);

	if (bind(Socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		WRITE_DEBUG("\nTCPIP, Unable to Bind Socket");
		return FALSE;
	}

	/* Listen */
	if (listen(Socket, 10) == -1)
	{
		WRITE_DEBUG("\nTCPIP, Unable to Listen");
		return FALSE;
	}

	WRITE_DEBUG("\nTCPIP, Port Listening*");
	return TRUE;
}


/**************************************************************
TCPIP Server wait for connection
***************************************************************/
int ConnectPort()
{
	int  lerrno = 0;

	WRITE_DEBUG("\nConnectPort - Enter \n");

	clilen = sizeof( cliaddr );

	while (TRUE)
	{
		if ((connfd = accept(Socket,
			(struct sockaddr *)&cliaddr, &clilen)) == -1)
		{

			lerrno = errno;

			/* If Interrupted Continue */
			if (lerrno == EINTR)
			{
				WRITE_DEBUG("\nTCPIP, Port Connecting*");
				continue;
			}

			WRITE_DEBUG("Unable to Accept, %s", strerror(lerrno));

			return FALSE;

		}

		break;

	}

	WRITE_DEBUG("\nTCPIP, Port Connected (Server)*");
	return TRUE;

}


/**************************************************************
TCPIP Close Port
***************************************************************/
int ClosePort()
{
	WRITE_DEBUG("\nClosePort - Enter\n");

	close(Socket);

	WRITE_DEBUG("\nTCPIP, Port Closed*");

	return TRUE;
}


/**************************************************************
TCPIP Read From Port
***************************************************************/
int ReadFromPort(char *oReadMessage, int MaxLength, int *oReadMessageLen)
{
	int n;
	int MsgLen;
	char MsgLenBuff[3];
	char ReadBuffer[1000];

	WRITE_DEBUG("ReadFromPort - Enter\n");

	memset(MsgLenBuff, 0, sizeof(MsgLenBuff));

	memset(ReadBuffer, 0, sizeof(ReadBuffer));
	n = recvfrom(connfd, ReadBuffer, sizeof(ReadBuffer)-1, 0, (struct sockaddr *)&cliaddr, &clilen);

	if (n < 0)
    {
    	WRITE_DEBUG("ERROR reading from socket\n");
		return -1;
    }

	else if (n == 0)
    {
    	WRITE_DEBUG("Client disconnected\n");
		return 0;
    }
	else
	{
		if (n > MaxLength || n < MSG_MIN_LENGTH)
		{
			WRITE_DEBUG("Invalid length message received [%d]\n", n);
			return -2;
		}

		/* Check message length */
		memcpy(MsgLenBuff, ReadBuffer, 2);
		if (atoi(MsgLenBuff) != strlen(ReadBuffer))
		{
			WRITE_DEBUG("Invalid message format received [%s]\n", ReadBuffer);
			return -3;
		}

		*oReadMessageLen = n-2;
		memcpy (oReadMessage, ReadBuffer+2, *oReadMessageLen);

		WRITE_DEBUG("ReadFromPort - Exit [%s]\n", oReadMessage);
		return 1;
	}

	return 0;
}

/**************************************************************
TCPIP Send To Port
***************************************************************/
int SendToPort(int SendMsgType, int SendResponseType)
{
	int offset = 0;
	char MsgBuff[MSG_MAX_LENGTH+1-2];
	char SendPortMessage[MSG_MAX_LENGTH+1];
	int  SendPortMessageLen;
	char MessageId[11];

	WRITE_DEBUG("SendToPort - Enter [%d] [%d]\n", SendMsgType, SendResponseType);

	memset(MsgBuff, 0, sizeof(MsgBuff));
	memset(SendPortMessage, 0, sizeof(SendPortMessage));
	memset(MessageId, 0, sizeof(MessageId));

	sprintf(MsgBuff, "%04d",SendMsgType);
	offset = offset +4;
	WRITE_DEBUG("SendToPort - Send Msg Type [%s]\n", MsgBuff);

	switch(SendResponseType)
	{
	case POWER_ON_SUCCESS:
	case POWER_OFF_SUCCESS:
		memcpy(MsgBuff+offset, ReadMsgStr.MsgId, 10);
		offset = offset + 10;
		memcpy(MsgBuff+offset, "00", 2);
		offset = offset + 2;
		break;

	case INVALID_FORMAT:
		GenerateMsgId(MessageId);
		memcpy(MsgBuff+offset, MessageId, 10);
		offset = offset +10;
		memcpy(MsgBuff+offset, "05", 2);
		offset = offset + 2;
		memcpy(MsgBuff+offset, INVALID_FORMAT_MSG, strlen(INVALID_FORMAT_MSG));
		offset = offset + strlen(INVALID_FORMAT_MSG);

		break;
	case STATUS_POWER_ON:
		memcpy(MsgBuff+offset, ReadMsgStr.MsgId, 10);
		offset = offset + 10;
		memcpy(MsgBuff+offset, "00", 2);
		offset = offset + 2;
		memcpy(MsgBuff+offset, STATUS_POWER_ON_MSG, strlen(STATUS_POWER_ON_MSG));
		offset = offset + strlen(STATUS_POWER_ON_MSG);
		break;

	case STATUS_POWER_OFF:
		memcpy(MsgBuff+offset, ReadMsgStr.MsgId, 10);
		offset = offset + 10;
		memcpy(MsgBuff+offset, "00", 2);
		offset = offset + 2;
		memcpy(MsgBuff+offset, STATUS_POWER_OFF_MSG, strlen(STATUS_POWER_OFF_MSG));
		offset = offset + strlen(STATUS_POWER_OFF_MSG);
		break;

	case STATUS_INVALID:
		GenerateMsgId(MessageId);
		memcpy(MsgBuff+offset, MessageId, 10);
		offset = offset +10;
		memcpy(MsgBuff+offset, "05", 2);
		offset = offset + 2;
		memcpy(MsgBuff+offset, STATUS_INVALID_MSG, strlen(STATUS_INVALID_MSG));
		offset = offset + strlen(STATUS_INVALID_MSG);
		break;

	case STATUS_READ_FAILED:
		GenerateMsgId(MessageId);
		memcpy(MsgBuff+offset, MessageId, 10);
		offset = offset +10;
		memcpy(MsgBuff+offset, "05", 2);
		offset = offset + 2;
		memcpy(MsgBuff+offset, STATUS_READ_FAILED_MSG, strlen(STATUS_READ_FAILED_MSG));
		offset = offset + strlen(STATUS_READ_FAILED_MSG);
		break;

	case STATUS_FILE_NOT_FOUND:
		GenerateMsgId(MessageId);
		memcpy(MsgBuff+offset, MessageId, 10);
		offset = offset +10;
		memcpy(MsgBuff+offset, "05", 2);
		offset = offset + 2;
		memcpy(MsgBuff+offset, STATUS_FILE_NOT_FOUND_MSG, strlen(STATUS_FILE_NOT_FOUND_MSG));
		offset = offset + strlen(STATUS_FILE_NOT_FOUND_MSG);
		break;

	case STATUS_UNKOWN_ERROR:
		GenerateMsgId(MessageId);
		memcpy(MsgBuff+offset, MessageId, 10);
		offset = offset +10;
		memcpy(MsgBuff+offset, "05", 2);
		offset = offset + 2;
		memcpy(MsgBuff+offset, STATUS_UNKOWN_ERROR_MSG, strlen(STATUS_UNKOWN_ERROR_MSG));
		offset = offset + strlen(STATUS_UNKOWN_ERROR_MSG);
		break;

	default:
		break;

	}

	WRITE_DEBUG("SendToPort - Msg Buff [%s]\n", MsgBuff);

	/*Add length */
	SendPortMessageLen = offset + 2;
	sprintf(SendPortMessage, "%02d",SendPortMessageLen);

	strcat(SendPortMessage, MsgBuff);
	WRITE_DEBUG("SendToPort - Send Msg [%s]\n", SendPortMessage);
	sendto(connfd, SendPortMessage, SendPortMessageLen, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

	return TRUE;
}


int GenerateMsgId(char *oMsgId)
{
	 time_t result;
	 result = time(NULL);

	 WRITE_DEBUG("GenerateMsgId - Enter\n");

	 sprintf(oMsgId, "%010d", (int)result);

	 WRITE_DEBUG("GenerateMsgId - Exit [%s]\n", oMsgId);

	 return TRUE;
}


