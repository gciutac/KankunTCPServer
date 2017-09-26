/*
 * knkonoffsrv.c
 *
 *  Created on: Dec 21, 2014
 *      Author: GG
 */

#include "knkonoffsrv.h"

int main(int argc, char**argv)
{
   pid_t    ChildPId;
   char 	PortBuffer[MSG_MAX_LENGTH+1];
   int 		PortBufferLen;
   
   int 		returnStatus;
   int 		MsgLen;
   int 		MsgType;
   char 	DummyBuf[50];

   returnStatus = 0;
   memset(DummyBuf, 0, sizeof(DummyBuf));
   PortConnected = FALSE;
   
   /* Read Input Parameters */
   PortNumber = atoi(argv[1]);
   strcpy(PowerControlFile, argv[2]);
   DebugFlag = atoi(argv[3]);
   
   WRITE_DEBUG("[PortNumber=%d] [PowerControlFile=%s] [DebugFlag = %d]\n",PortNumber, PowerControlFile, DebugFlag);

   /* Initialize signal handlers */
   CatchSignals();

   /* Initialize port */
   if(!ListenOnPort())
	{
		WRITE_DEBUG("Listen On Port failed\n");
		exit(2);
	}

   /* Server Main Loop */
	while (TRUE)
   {
		/* Wait for client connection */
		PortConnected = ConnectPort();
		if(!PortConnected)
		{
			WRITE_DEBUG("\nConnect to port FAILED [%i], Closing...", PortConnected);
			ClosePort();
			usleep(10 * 100000);
			continue;
		}

      if ((ChildPId = fork()) == 0)
      {
    	  ClosePort();

    	  /* Child Main Loop */
         while (TRUE)
         {
        	memset(PortBuffer, 0, sizeof(PortBuffer));
        	returnStatus = ReadFromPort(PortBuffer, MSG_MAX_LENGTH, &PortBufferLen);

        	switch (returnStatus)
        	{
        	case 1:
        		WRITE_DEBUG("Message Received. Length[%d] [%s]\n", PortBufferLen, PortBuffer);
        		returnStatus = ParseInputMessage(PortBuffer, PortBufferLen);
        		if (returnStatus == 1)
        		{
        			WRITE_DEBUG("MessageType:[%d]\n", ReadMsgStr.MsgType);
        		}
        		else
        		{
					SendToPort(9110, INVALID_FORMAT);
					break;
        		}

    			/* Check the message type and take the appropriate action */
    			/*--------------------------------------------------------*/
    			switch(ReadMsgStr.MsgType)
    			{
    				case 9100:
    					TurnPowerOn();
    					SendToPort(9110, POWER_ON_SUCCESS);
    					break;

    				case 9120:
    					TurnPowerOn();
    					break;

    				case 9400:
    					TurnPowerOff();
    					SendToPort(9410, POWER_OFF_SUCCESS);
    					break;

    				case 9420:
    					TurnPowerOff();
    					break;

    				case 9500:
    					break;

    				case 9800:
    						returnStatus= GetPowerOnOffStatus();

    						switch(returnStatus)
    						{
    							case 0:
    								SendToPort(9810, STATUS_POWER_OFF);
    								break;
    							case 1:
    								SendToPort(9810, STATUS_POWER_ON);
    								break;
    							case -3:
    								SendToPort(9810, STATUS_INVALID);
    								break;
    							case -2:
    								SendToPort(9810, STATUS_READ_FAILED);
    								break;
    							case -1:
    								SendToPort(9810, STATUS_FILE_NOT_FOUND);
    								break;
    							default:
    								SendToPort(9810, STATUS_UNKOWN_ERROR);
    								break;
    						}

    						break;
    				default:
    					SendToPort(9110, INVALID_FORMAT);
    					break;
    			}
        		break;

        	case 0:
            	WRITE_DEBUG("Client disconnected");
				close(connfd);
				exit(4);
        		break;
        	case -1:
            	WRITE_DEBUG("ERROR reading from socket");
				exit(3);
        		break;
        	case -2:
        	case -3:
        		SendToPort(9110, INVALID_FORMAT);
        		break;
        	default:
        		break;
        	}
         }
      }
      close(connfd);
   }
}


void CatchSignals()
{
	struct sigaction	act;

	WRITE_DEBUG("CatchSignals - Enter\n");

	act.sa_sigaction = CatchCHLD;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &act, NULL);

	WRITE_DEBUG("CatchSignals - Exit\n");
}

/*****************************************************************
Set SigCHLD flag
******************************************************************/
void CatchCHLD(int dummy, siginfo_t *ChildInfo, void *dummy1)
{
	struct sigaction	act;
	int			i;
	int			deadpid;
	int			stat_log;

	WRITE_DEBUG("CatchCHLD - Enter\n");

	/* ReSet the signal to catch the death of a child process */
	act.sa_sigaction = CatchCHLD;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &act, NULL);

	/* If a non valid pid is given, just return */
	if (ChildInfo->si_pid <= 0)
		return;

	/* Make a wait call on the dead child in order to avoid zombie processes */
	deadpid = waitpid(ChildInfo->si_pid, &stat_log, WNOHANG);

	/* return if no pid given */
	if (deadpid == 0)
		return;

	/* If the child process exited ubnormally, clean up the mess */
	if (ChildInfo->si_code != CLD_EXITED)
	{
		kill(ChildInfo->si_pid, SIGKILL);
		return;
	}

	WRITE_DEBUG("CatchCHLD [%i] Exit.", ChildInfo->si_pid);
	return;
}




