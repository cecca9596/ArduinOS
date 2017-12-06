#ifndef QUEUE_H_
#define QUEUE_H_

#include "kernel.h"

// n writers, 1 reader at time

typedef struct queue_type
{
    //#numero di elementi, lunghezza della coda, puntatori alla testa e alla coda,
    //processo bloccato sulla coda in attesa di lettura,
    //priority queue flag
    int *qbuf;
		unsigned char count, length, head, tail, blockproc, prioQ;
} OSQueue;

int OSDequeue(OSQueue *queue);

void OSMakeQueue(int *buffer, unsigned char length, OSQueue *queue);
void OSEnqueue(int data, OSQueue *queue);

#endif
