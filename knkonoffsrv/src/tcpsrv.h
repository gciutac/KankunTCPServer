/*
 * tcpsrv.h
 *
 *  Created on: Dec 18, 2014
 *      Author: GG
 */

#ifndef TCPSRV_H_
#define TCPSRV_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include "external.h"
#include "msgprotocol.h"

#define MSG_MAX_LENGTH 100
#define MSG_MIN_LENGTH 2

#define POWER_ON_SUCCESS 		1
#define POWER_OFF_SUCCESS 		2
#define INVALID_FORMAT  		3
#define STATUS_POWER_ON			4
#define STATUS_POWER_OFF		5
#define STATUS_INVALID			6
#define STATUS_READ_FAILED		7
#define STATUS_FILE_NOT_FOUND	8
#define STATUS_UNKOWN_ERROR		9

#define INVALID_FORMAT_MSG  		"INVALID FORMAT"
#define STATUS_POWER_ON_MSG			"ON"
#define STATUS_POWER_OFF_MSG		"OFF"
#define STATUS_INVALID_MSG			"STATUS INVALID"
#define STATUS_READ_FAILED_MSG		"STATUS READ FAILED"
#define STATUS_FILE_NOT_FOUND_MSG	"STATUS FILE NOT FOUND"
#define STATUS_UNKOWN_ERROR_MSG		"STATUS UNKOWN ERROR"



int Socket, connfd, n;
struct sockaddr_in servaddr, cliaddr;
socklen_t clilen;


int ListenOnPort();
int ConnectPort();
int ClosePort();

#endif /* TCPSRV_H_ */
