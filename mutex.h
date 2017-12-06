#ifndef MUTEX_H_
#define MUTEX_H_

#include "kernel.h"

typedef struct typemutex
{
	unsigned char val;
	unsigned char procList[OSMAX_TASKS];
	CODA_PROCESSI blocked; //definita in kernel.h
} OSMutex;

void OSCreaMutex(OSMutex *mutex);
void OSMutexWait(OSMutex *mutex);
void OSMutexPost(OSMutex *mutex);

#endif /* MUTEX_H_ */
