#include "sema.h"

void OSSem_init(TOSSema *sema, unsigned int initval, unsigned char binario){
  unsigned char csreg;
	OSMakeAtomic(&csreg);
  sema->binario = binario;

  if(!binario)
    sema->semaval = initval;
  else if(initval){
      sema->semaval = 1;
    }
    else{
      sema->semaval = 0;
    }
  //inizializza la coda dei processi in attesa al semaforo
  init_coda(sema->listaProcessi, OSMAX_TASKS, &sema->codaProcessi);
  OSExitAtomic(csreg);
}

void OSSem_wait(TOSSema *sema){
  unsigned char csreg;
	OSMakeAtomic(&csreg);
  if(sema->semaval>0)
    sema->semaval--;
  else{
    //blocca processo corrente e mettilo in coda
    processi[running].status |= _OS_BLOCKED;
    enq(running, processi, &sema->codaProcessi);
    OSExitAtomic(csreg);

    //chiama lo scheduler
    OS_change();
  }
  OSExitAtomic(csreg);
}

void OSSem_post(TOSSema *sema){
  unsigned char csreg;
  OSMakeAtomic(&csreg);

  unsigned char wakeProc= procDeq(&sema->codaProcessi);

  if(wakeProc != 255){
    //rimuovi il block flag
    processi[wakeProc].status &= ~(_OS_BLOCKED);

    //rimetti in coda ready il processo
    enq(wakeProc, processi, &pronti);
    OSExitAtomic(csreg);

    //chiama lo scheduler
		OS_change();
  }
  else if(sema->binario)
    sema->semaval=1;
  else
    sema->semaval++;
  OSExitAtomic(csreg);
}
