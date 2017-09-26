#include "powerctl.h"

int TurnPowerOn()
{
  FILE * pFile;

  WRITE_DEBUG("TurnPowerOn - Enter\n");

  pFile = fopen (PowerControlFile, "w");
  if (pFile!=NULL)
  {
    fputs ("1",pFile);
    fclose (pFile);
  }
  else
  {
	WRITE_DEBUG("TurnPowerOn - Exit [0]\n");
    return 0;
  }
  
  WRITE_DEBUG("TurnPowerOn - Exit\n");
  return 1;
}

int TurnPowerOff()
{
  FILE * pFile;

  WRITE_DEBUG("TurnPowerOff - Enter\n");
  pFile = fopen (PowerControlFile, "w");
  if (pFile!=NULL)
  {
    fputs ("0",pFile);
    fclose (pFile);
  }
  else
  {
	WRITE_DEBUG("TurnPowerOff - Exit[0]\n");
    return 0;
  }
  
  WRITE_DEBUG("TurnPowerOff - Exit\n");
  return 1;
}

/* *
 * Return: -1 File Not Found
 * 		   -2 Failed To Read Status
 * 		   -3 Invalid Status
 * 			0 Power Is Off
 * 			1 Power Is On
 * */
int GetPowerOnOffStatus()
{
  FILE * pFile;
  char fileReadBuff[5];

  WRITE_DEBUG("GetPowerOnOffStatus - Enter\n");
  pFile = fopen (PowerControlFile, "r");
  if (pFile!=NULL)
  {
	  if (fgets (fileReadBuff , 4 , pFile) != NULL)
	  {
		  if (strlen(fileReadBuff) > 2
				  || (fileReadBuff[0] != '0' && fileReadBuff[0] != '1'))
		  {
			  WRITE_DEBUG("GetPowerOnOffStatus - Exit[-3]\n");
			  return -3;
		  }
		  else
		  {
			  WRITE_DEBUG("GetPowerOnOffStatus - Exit\n");
			  return fileReadBuff[0] - '0';
		  }
	  }
	  else
	  {
		  WRITE_DEBUG("GetPowerOnOffStatus - Exit[-2]\n");
		  return -2;
	  }

    fclose (pFile);
  }
  else
  {
	WRITE_DEBUG("GetPowerOnOffStatus - Exit[-1]\n");
    return -1;
  }

  WRITE_DEBUG("GetPowerOnOffStatus - Exit [0]\n");
  return 0;
}
