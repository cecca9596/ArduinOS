#include <stdlib.h>
#include <avr/interrupt.h>
#include <Arduino.h>

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

#define OS_FIRSTRUN	0b1

void OSSetError(unsigned int);

void _OS_change(); //__attribute__ ((naked));

void OS_change(); //__attribute__ ((naked));

void OS_init(unsigned char numTasks);
void OS_run();
void OS_crea_processo( void (*rptr)(void *));

#define OSMakeAtomic(a) \
	*a = SREG;	\
	cli();

#define OSExitAtomic(a) \
	SREG = a;
