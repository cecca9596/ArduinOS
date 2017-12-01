#include "queue.h"

// sblocca un processo bloccato in lettura(coda precedentemente vuota) e chiama lo scheduler
void OSQueueUnblock(tQueue *queue){
  processi[queue->blockproc].status &= ~_OS_BLOCKED;

	// il processo appena sbloccato viene rimesso nella coda ready
	procEnq(queue->blockproc, processi, &pronti);
	queue->blockproc=255;

	OSExitAtomic();

	OS_change();
}

int OSDequeue(tQueue *queue){
  int ret = 255;
  OSMakeAtomic();

  if(!queue->count){ // se la coda è vuota
    //blocca il processo corrente
    queue->blockproc=running;
    processi[running].status|=_OS_BLOCKED;

    OSExitAtomic();

    OS_change();
  }

  OSMakeAtomic();

  ret=queue->qbuf[queue->head];

  queue->head = (queue->head+1) % queue->len;
	queue->count--;

  OSExitAtomic();
	return ret;
}

void OSMakeQueue(int *buffer, unsigned char length, tQueue *queue){
  OSMakeAtomic();
  queue->count = 0;
  queue->length = 0;
  queue->head = 0;
  queue->tail = 0;
  queue->qbuf = buffer;
  queue->prioQ = 0;
  OSExitAtomic();
}

void OSEnqueue(int data, tQueue *queue){
  OSMakeAtomic();

  //se la coda è piena ritorna
  if(queue->count==queue->len)
		return;

  queue->qbuf[queue->tail]=data;
  queue->tail = (queue->tail+1)  % queue->len;
  queue->count++;

  //se qualcuno è bloccato in lettura viene sbloccato
  if(queue->blockproc!=255)
		OSQueueUnblock(queue);

  OSExitAtomic();
}
