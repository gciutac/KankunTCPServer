/*
 * knkonoffsrv.h
 *
 *  Created on: Dec 21, 2014
 *      Author: GG
 */

#ifndef KNKONOFFSRV_H_
#define KNKONOFFSRV_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include "external.h"
#include "external.h"
#include "powerctl.h"
#include "tcpsrv.h"
#include "msgprotocol.h"

void CatchSignals();
void CatchCHLD(int dummy, siginfo_t *ChildInfo, void *dummy1);

#endif /* KNKONOFFSRV_H_ */
