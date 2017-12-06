#include "kernel.h"

//variabile temporanea per lo status register
unsigned char csreg;

//variabile temporanea per lo stack pointer
unsigned long current_stack_pointer;

// puntatore alla funzione da eseguire e  argomento passato(serve solo se è la prima volta che vengo eseguito)
unsigned long funzione, argomento;

// Numero di processi richiesti
unsigned char num_proc=0;

// dimensione dello stack
unsigned char taskStackSize=50;

//numero corrente di processi
unsigned char procCount=0;

//lista dei processi(numero massimo di priocessi 10)
PCB processi[10];

// coda dei processi pronti
static unsigned char array_coda[10];
CODA_PROCESSI pronti;

//indice del array processi del processo corrente(inizializzato a 255 perchè al primo run non devo salvare lo stato del setup)
unsigned char running=255;

//variabile per settare errore
unsigned int _errno=0;

// macro per il salvataggio dello stato
#define SAVE_CONTEXT()\
asm volatile (\
	"push r0	\n\t"\
	"mov r0, %0	\n\t"\
	"cli	\n\t"\
	"push r0	\n\t"\
	"push r1	\n\t"\
	"clr r1	\n\t"\
	"push r2	\n\t"\
	"push r3	\n\t"\
	"push r4	\n\t"\
	"push r5	\n\t"\
	"push r6	\n\t"\
	"push r7	\n\t"\
	"push r8	\n\t"\
	"push r9	\n\t"\
	"push r10	\n\t"\
	"push r11	\n\t"\
	"push r12	\n\t"\
	"push r13	\n\t"\
	"push r14	\n\t"\
	"push r15	\n\t"\
	"push r16	\n\t"\
	"push r17	\n\t"\
	"push r18	\n\t"\
	"push r19	\n\t"\
	"push r20	\n\t"\
	"push r21	\n\t"\
	"push r22	\n\t"\
	"push r23	\n\t"\
	"push r24	\n\t"\
	"push r25	\n\t"\
	"push r26	\n\t"\
	"push r27	\n\t"\
	"push r28	\n\t"\
	"push r29	\n\t"\
	"push r30	\n\t"\
	"push r31	\n\t"\
	"in r26, __SP_L__	\n\t"\
	"in r27, __SP_H__	\n\t"\
	"sts current_stack_pointer+1, r27	\n\t"\
	"sts current_stack_pointer, r26	\n\t"\
	: : "r" (csreg));

	#define RESTORE_CONTEXT()\
asm volatile (\
	"cli	\n\t"\
	"out __SP_L__, %A1	\n\t"\
	"out __SP_H__, %B1	\n\t"\
	"pop r31	\n\t"\
	"pop r30	\n\t"\
	"pop r29	\n\t"\
	"pop r28	\n\t"\
	"pop r27	\n\t"\
	"pop r26	\n\t"\
	"pop r25	\n\t"\
	"pop r24	\n\t"\
	"pop r23	\n\t"\
	"pop r22	\n\t"\
	"pop r21	\n\t"\
	"pop r20	\n\t"\
	"pop r19	\n\t"\
	"pop r18	\n\t"\
	"pop r17	\n\t"\
	"pop r16	\n\t"\
	"pop r15	\n\t"\
	"pop r14	\n\t"\
	"pop r13	\n\t"\
	"pop r12	\n\t"\
	"pop r11	\n\t"\
	"pop r10	\n\t"\
	"pop r9	\n\t"\
	"pop r8	\n\t"\
	"pop r7	\n\t"\
	"pop r6	\n\t"\
	"pop r5	\n\t"\
	"pop r4	\n\t"\
	"pop r3	\n\t"\
	"pop r2	\n\t"\
	"pop r1	\n\t"\
	"pop r0	\n\t"\
	"out __SREG__, r0\n\t"\
	"mov %0, r0	\n\t"\
	"pop r0	\n\t"\
	: "=r" (csreg) : "r" (current_stack_pointer));

//se il processo viene schedulato la prima volta devo solo settare lo stack
#define SetStack()\
asm volatile(\
	"OUT __SP_L__, %A0	\n\t"\
	"OUT __SP_H__, %B0	\n\t"\
	: : "r" (current_stack_pointer))

//se è la prima volta che vengo schedulato devo settare la funzione da eseguire nello stack e passare l'argomento nei registri
	#define PushRetAddress()\
	asm volatile(\
		"mov r0, %A0	\n\t"\
		"push r0	\n\t"\
		"mov r0, %B0	\n\t"\
		"push r0	\n\t"\
         : : "r" (funzione))

// gestione errore

void OSSetError(unsigned int errno)
{
	_errno=errno;
	Serial.begin(115200);
	Serial.print("ERROR: ");
	Serial.println(_errno);
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
}

void OS_crea_processo( void (*func)(void *)){
		unsigned char sreg;

	OSMakeAtomic(&sreg);
//controllo se il numero di processi correnti è maggiore del numero massimo di processi
	if(procCount>=10){
		//OSSetError(OS_ERR_MAX_PROCS);
		return ;
	}



	//inserisci il nuvo processo nel array dei pcb

	processi[procCount].funz=func;

	processi[procCount].stack=(unsigned long *) calloc((size_t) 50, sizeof(unsigned long));
	processi[procCount].sp=(unsigned long) &(processi[procCount].stack[49]);
	processi[procCount].pid=procCount;
	processi[procCount].status|=OS_FIRSTRUN;


	//inserisci il processo nella coda dei pronti
	enq(procCount,processi, &pronti);
	procCount++;
OSExitAtomic(sreg);

}

void Scheduler(){
	//restituisce l'indice del prossimo processo da eseguire ma non lo rimuove
	unsigned char nextRun=prossimo_processo(&pronti);

	//controllo se la coda non è vuota
	if(nextRun != 255){
		//se c'è un processo schedulabile lo levo dalla coda
		nextRun=procDeq(&pronti);
		//controllo se nextrun è uguale a running oppure sono il setup
		if(running!=255 && nextRun !=  running)
		{
			processi[running].sp=current_stack_pointer;//salvo nel pcb lo stack pointer salvato da SAVE_CONTEXT

			// metto nella coda il processo running
				enq(running,processi, &pronti);
		}

		current_stack_pointer=processi[nextRun].sp;//salvo il nuovo stack pointer nella variabile usata da RESTORE_CONTEXT
		running=nextRun;

	}

}
//funzione inline(importante)
inline void run_processo(){
	//controllo se è la prima volta che vengo schedulato(se è la primavolta devo solo settare lo stack e la funzione e gli argomenti)
	if(processi[running].status & OS_FIRSTRUN){
		processi[running].status &= ~(OS_FIRSTRUN);
		//istruzioni assembly per settare stack stack
		SetStack();
        //setto le variabile usate da pushretaddress
		funzione=(unsigned long) processi[running].funz;

		//istruzioni assembly per settare funzione e argomento
		PushRetAddress();
		sei();
	}
	else
	{
		RESTORE_CONTEXT();
	}
}
void _OS_change(){
	//salvo il mio stato
	SAVE_CONTEXT();
	//cli();
	//chiamo lo scheduler
	Scheduler();
	//eseguo il processo
    run_processo();
}

//funzione usata a livello utente in modo cooperativo per cambiare processo
void OS_change()
{
	csreg = SREG;
	_OS_change();
	asm("ret");
}
// funzione per inizializzare le code
void OS_init(unsigned char numpro)
{	OSMakeAtomic(&csreg);
	num_proc=numpro;
    OSExitAtomic(csreg);
	init_coda(array_coda,num_proc, &pronti);
}
void OS_run()
{
OSMakeAtomic(&csreg);
	OS_change();
	OSExitAtomic(csreg);
}
