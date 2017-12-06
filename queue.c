#include "kernel.h"
#include "queue.h"

// sblocca un processo bloccato in lettura(coda precedentemente vuota) e chiama lo scheduler
void OSQueueUnblock(OSQueue *queue, unsigned char csreg){
  processi[queue->blockproc].status &= ~_OS_BLOCKED;

	// il processo appena sbloccato viene rimesso nella coda ready
	enq(queue->blockproc, processi, &pronti);
	queue->blockproc=255;

	OSExitAtomic(csreg);

	OS_change();
}

int OSDequeue(OSQueue *queue){
  int ret = 255;
  unsigned char csreg;
  OSMakeAtomic(&csreg);

  if(!queue->count){ // se la coda è vuota
    //blocca il processo corrente
    queue->blockproc=running;
    processi[running].status|=_OS_BLOCKED;

    OSExitAtomic(csreg);

    OS_change();
  }

  ret=queue->qbuf[queue->head];

  queue->head = (queue->head+1) % queue->length;
	queue->count--;

  OSExitAtomic(csreg);
	return ret;
}

void OSMakeQueue(int *buffer, unsigned char length, OSQueue *queue){
  unsigned char csreg;
  OSMakeAtomic(&csreg);
  queue->count = 0;
  queue->length = 0;
  queue->head = 0;
  queue->tail = 0;
  queue->qbuf = buffer;
  queue->prioQ = 0;
  OSExitAtomic(csreg);
}

void OSEnqueue(int data, OSQueue *queue){
  unsigned char csreg;
  OSMakeAtomic(&csreg);

  //se la coda è piena ritorna
  if(queue->count==queue->length)
		return;

  queue->qbuf[queue->tail]=data;
  queue->tail = (queue->tail+1)  % queue->length;
  queue->count++;

  //se qualcuno è bloccato in lettura viene sbloccato
  if(queue->blockproc!=255)
		OSQueueUnblock(queue, csreg);

  OSExitAtomic(csreg);
}
