#include <stdlib.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#ifndef KERNEL_H_
#define KERNEL_H_

#define OSMAX_TASKS 10

//struct contente per i pcb
typedef struct pcb{
	unsigned char pid;	// Process ID
	unsigned char status; // bit 0 = first run flag, bit 1 = blocked flag
	unsigned long *stack; // puntatore allo stack
	unsigned long sp;	// Stack pointer
	void (*funz)(void*); //puntatore a funzione

} PCB;

// struct per gestire le code
typedef struct _coda{
	unsigned char *array, testa, coda, lunghezza, num_elem;
} CODA_PROCESSI;

//funzioni per gestire la coda
void init_coda(unsigned char *, unsigned char len, CODA_PROCESSI *q);

void enq(int pid,PCB* pro, CODA_PROCESSI *q);

unsigned char prossimo_processo(CODA_PROCESSI *q);

unsigned char procDeq(CODA_PROCESSI *q);

//VARIABILI PRIVATE KERNEL
extern unsigned char csreg;

//lista dei processi(numero massimo di priocessi 10)
extern PCB processi[OSMAX_TASKS];

//numero corrente di processi
extern unsigned char procCount;

extern CODA_PROCESSI pronti;

// Numero di processi richiesti
extern unsigned char num_proc;

//indice del array processi del processo corrente(inizializzato a 255 perchè al primo run non devo salvare lo stato del setup)
extern unsigned char running;


#define OS_FIRSTRUN	0b1
#define _OS_BLOCKED	0b10

void OSSetError(unsigned int);

void _OS_change(); //__attribute__ ((naked));

void OS_change(); //__attribute__ ((naked));

void OS_init(unsigned char numTasks);
void OS_run();
void OS_crea_processo( void (*rptr)(void *));

// Atomicity Control
inline void OSMakeAtomic(unsigned char *_csreg)
{
	*_csreg = SREG;
	cli();
}

inline void OSExitAtomic(unsigned char _csreg)
{
	SREG=_csreg;
}


/*#define OSMakeAtomic(a) \
	*a = SREG;	\
	cli();

#define OSExitAtomic(a) \
	SREG = a;*/

#endif
