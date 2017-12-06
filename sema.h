#ifndef SEMA_H_
#define SEMA_H_

#include "kernel.h"

typedef struct sem_s
{
	// Sem flags
	unsigned int semaval, binario;
	unsigned char listaProcessi[OSMAX_TASKS];
	CODA_PROCESSI codaProcessi; //definita in kernel.h
} TOSSema;

void OSSem_init(TOSSema *sema, unsigned int initval, unsigned char binario);
void OSSem_wait(TOSSema *sema);
void OSSem_post(TOSSema *sema);

#endif /* SEMA_H_ */
