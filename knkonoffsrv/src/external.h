#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

//#define WRITE_DEBUG (MESSAGE, ...) fprintf(stderr, "DEBUG %s:%d: " MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define WRITE_DEBUG if (DebugFlag) printf

int PortNumber;
char PowerControlFile[250];
int DebugFlag;

short PortConnected;

#endif /*EXTERNAL_H_*/


