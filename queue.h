#ifndef QUEUE_H_
#define QUEUE_H_

#include "ArdOSConfig.h"
#include "task.h"
#include "kernel.h"

// n writers, 1 reader at time

typedef struct queue_type
{
    //#numero di elementi, lunghezza della coda, puntatori alla testa e alla coda,
    //processo bloccato sulla coda in attesa di lettura,
    //priority queue flag
		unsigned char count, len, head, tail, blockproc, prioQ;
} tQueue;

int OSDequeue(tQueue *queue);

void OSMakeQueue(int *buffer, unsigned char length, tQueue *queue);
void OSEnqueue(int data, tQueue *queue);
