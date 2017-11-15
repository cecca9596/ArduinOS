#include <avr/io.h>
#include <stdlib.h>

//numero massimo di processi
#define MAX_PROC 10;


//numero di byte dello stack
#define stack_dim  50;

//per capire se è la prima volta che il processo viene eseguito
#define OS_first_run 0b1;


//process contol block di ogni processo che contiene informazioni varie
typedef struct pcb{
	
	int pid;//numero unico del processo
	
	int priorita;//priorita del processo usato per il suo scheduling
	
	int* stack;//puntatore alla zona di memoria dedicata all'uso dello stack del processo corrente
	
	int* stack_pointer;//valore dello stack point corrente
	
	void * funzione_processo;//puntatore a funzione del processo
		
}PCB;

typedef struct coda_processi{
	char* buf_proc, testa,coda,lunghezza;
}
CODA_PROCESSI;

//funzioni per gestire la coda dei priocessi

void init_coda(char *array,char len,CODA_PROCESSI *coda);

char prossimo_processo(CODA_PROCESSI *coda);

//variabili private usate dal kernel

//stack pointer temporaneo del processo corrente
extern int stack_pointer_temp;

//coda dei processi pronti per essere schedulati
extern CODA_PROCESSI pronti;
extern char array_pronti[MAX_PROC];

//array di dimensione MAX_PROC che contiene i pcb dei vari processi
extern PCB processi[MAX_PROC];

//indice del array processi del processo in esecuzione
extern char running;

//numero di processi nel sistema
extern char num_proc;

//puntatore funzione eseguita dal processo running
extern void* funzione_corrente;

//inizializza il sistema per poter eseguire numero_proc processi
void OS_init(char numero_processi);

//fa partire il sistema facendo eseguire il primo processo
void OS_run();

//crea processo che esegue funzione con priorita priori
void OS_crea_processo(int priori,void* funzione);

//chiama la funziona OS_scheduling e OS_run_proc e esegue il processo scelto dallo scheduling
void OS_change();

//funzione che sceglie il processo da eseguire
void OS_scheduling();

//fa partire il processo 
void OS_run_proc();
