#include "kernel.h"


//mette in coda processo
void enq(int pid, PCB *tasklist, CODA_PROCESSI *q)
{
	unsigned char sreg;
	OSMakeAtomic(&sreg);

	unsigned char i;
	unsigned int iter=q->testa;
	unsigned char flag=0;
	
	if(q->num_elem >= q->lunghezza)
	{
		OSExitAtomic(sreg);
		return;
	}
			

	else
		iter=q->coda;
		
	q->coda=(q->coda+1)%q->lunghezza;
	

	
	
	q->array[iter]=pid;
	q->num_elem++;
	OSExitAtomic(sreg);
	
}
//restituisce l'indice del prossimo processo da eseguire

unsigned char prossimo_processo(CODA_PROCESSI *q)
{
     unsigned char sreg;
	OSMakeAtomic(&sreg);
	if(!q->num_elem)
	{
		OSExitAtomic(sreg);
		return 255;
	}
	else
	{
		OSExitAtomic(sreg);
		return q->array[q->testa];
	}
}
//restituisce indice prossimo processo e lo leva dalla coda pronti
unsigned char procDeq(CODA_PROCESSI *q)
{
	unsigned char sreg;
	OSMakeAtomic(&sreg);

	unsigned char ret=255;
	if(q->num_elem>0)
	{
		ret=q->array[q->testa];
		q->testa=(q->testa+1)%q->lunghezza;
		q->num_elem--;
	}
OSExitAtomic(sreg);
	return ret;
}

//inizializza coda
void init_coda(unsigned char *qbuf, unsigned char len, CODA_PROCESSI *q)
{

	unsigned char sreg;
	OSMakeAtomic(&sreg);
	unsigned char i;
		
	q->testa=0;
	q->coda=0;
	q->array=qbuf;
	q->lunghezza=len;
	q->num_elem=0;

	for(i=0; i<len; i++)
		q->array[i]=255;
	OSExitAtomic(sreg);
}
