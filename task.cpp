#include "kernel.h"
//funzione che prende in input un array la sua lunghezza e una struttura dati per gestire la coda
void init_coda(char *array,char len,CODA_PROCESSI *coda_p){
	
    unsigned char i;
		
	coda_p->testa=0;
	coda_p->coda=0;
	coda_p->buf_proc=array;
	coda_p->len=len;
	

	for(i=0; i<len; i++)
		coda_p->buf_proc[i]=255;
}
//restituisce l'indice nel array processi del prossimo processo da eseguire
char prossimo_processo(CODA_PROCESSI* coda_p){
	return coda_p->buf_proc[coda_p->testa];
}
//restituiesce l'indice nel array processi ...... e lo leva dalla coda
char procDeq(CODA_PROCESSI* coda_p){
		ret=q->buf_proc[q->head];
		q->head=(q->head+1)%q->len;
		
		return ret;
}

//accoda nella coda della coda :) il processo con il pid passato
void procEnq(int pid,PCB * array_pcb,CODA_PROCESSI* coda_p){
	int iter=coda_p->coda;		
	coda_p->coda=(coda_p->coda+1)%coda_p->len;	
	coda_p->buf_proc[iter]=pid;
}
