#include "kernel.h"

//codice assemly per salvare nello stack lo stato del processo
#define SAVE_CONTEXT()\
asm volatile (\
"push r0	\n\t"\
"in r0, __SREG__	\n\t"\
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
"sts stack_pointer_temp+1, r27	\n\t"\
"sts stack_pointer_temp, r26	\n\t"\
"sei	\n\t" : :);
//codice assembly per ripristinare lo stato precedentemente salvato
#define RESTORE_CONTEXT()\
asm volatile (\
"cli	\n\t"\
"out __SP_L__, %A0	\n\t"\
"out __SP_H__, %B0	\n\t"\
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
"sei	\n\t"\
"out __SREG__, r0\n\t"\
"pop r0	\n\t": : "r" (stack_pointer_temp));

//se il mio processo viene schedulato la prima volta devo solo "poppare" lo stack pointer
#define SetStack()\
asm volatile(\
	"OUT __SP_L__, %A0	\n\t"\
	"OUT __SP_H__, %B0	\n\t": : "r" (stack_pointer_temp))
	
//se il mio processo viene schedulato la prima volta devo "poppare" il puntatore alla funzione 
#define portPushRetAddress()\
	asm volatile(\
		"mov r0, %A0	\n\t"\
		"push r0	\n\t"\
		"mov r0, %B0	\n\t"\
		"push r0	\n\t": : "r" (funzione_corrente))
		
void OS_run(){//da migliorare e  capire
	
	OS_change();
}

void OS_init(char numero_processi){
	num_proc=numero_processi;
	init_coda(array_pronti,MAX_PROC,&pronti);
}

void OS_change(){
	SAVE_CONTEXT();//salva il contenuto dei registri
	
	OS_scheduling();//seleziono il prossimo processo da eseguire
	
	OS_run_proc();//eseguo il processo
	asm("ret");//istruzione assembly che mi permette di far puntare ip alla funzione del nuovo processo
}
//function inlining quindi questa funzione viene copiata dentro OS_change
inline void OS_run_proc(){
	//se è la prima esecuzione 
	if(processi[running].status & OS_first_run){
	SetStack();//se è la prima volta che vengo eseguito devo solo "poppare"lo stack pointer iniziale
	
	   funzione_corrente= processi[running].funzione_processo;
	    
	   push_funzione();//istruzioni assembly per pushare la funzione corrente nello stack che verra chiamata grazie alla ret
   }
   //se non è la prima esecuzione
   else{
	   RESTORE_CONTEXT();
   }
	   
		
}

void OS_crea_processo(int priori,void* func){
	if(num_proc>=MAX_PROC){
		//ERRORE DA GESTIRE
	}
	//gestire errori es(se esiste processo con la stesso priori)
	//....
	//....
	//....
	//inserire nel array dei pcb il nuovo processo
	processi[num_proc].priorita=priori;//priorita del processo creato
	processi[num_proc].funzione_processo=funzione_corrente;//funzione da eseguire
	processi[num_proc].stack=(int *) calloc((size_t) stack_dim, sizeof( int));
	processi[num_proc].stack_pointer=(int*) &(processi[num_proc].stack[stack_dim-1]);
	processi[num_proc].pid=num_proc;
	
	enq();//FUNZIONE DA DEFINIRE PER ACCODARE IL PROCESSO NELLA CODA DEI PROCESSI
	num_proc++;
}

void OS_scheduling(){
	//indice del prossimo processo da essere eseguito nel array dei pcb
	char next_run=prossimo_processo(&pronti);
	
	//salvo nel pcb lo stack pointer salvato grazie a SAVE_CONTEXT
	processi[running].stack_pointer=stack_pointer_temp;
	
	//nuovo stack pointer temporaneo
	stack_pointer_temp=processi[next_run].stack_pointer;
	
	//scambio running
	running=next_run;
}
