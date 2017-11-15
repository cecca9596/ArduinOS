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
char prossimo_processo(coda_processi* coda_p){
	return coda_p->buf_proc[coda_p->testa];
}
