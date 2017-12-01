#ifndef MUTEX_H_
#define MUTEX_H_

#include "ArdOSConfig.h"
#include "task.h"
#include "kernel.h"

typedef struct typemutex
{
	unsigned char val;
	unsigned char procList[OSMAX_TASKS];
	tQueue blocked; //definita in task.h
} OSMutex;

void OSCreaMutex(OSMutex *mutex);
void OSMutexWait(OSMutex *mutex);
void OSMutexPost(OSMutex *mutex);

#endif /* MUTEX_H_ */
