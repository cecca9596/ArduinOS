#include "sema.h"

void OSSem_init(TOSSema *sema, unsigned int initval, unsigned char binario){
  sema->binario = binario;

  if (!binario)
    sema->semaval = initval;
  else
    else if(initval)
      sema->semaval = 1;
    else
      sema->semaval = 0;

  //inizializza la coda dei processi in attesa al semaforo
  initQ(sema->listaProcessi, OSMAX_TASKS, &sema->codaProcessi);
}

void OSSem_wait(TOSSema *sema){
  if(sema->semaval>0)
    sema->semaval--;
  else
    //blocca processo corrente e mettilo in coda
    processi[running].status |= _OS_BLOCKED;
    prioEnq(running, processi, &sema->codaProcessi);

    //chiama lo scheduler
    OSSwap();
}

void OSSem_post(TOSSema *sema){
  unsigned char wakeProc= procDeq(&sema->codaProcessi);

  if(wakeProc != 255){
    //rimuovi il block flag
    processi[wakeProc].status &= ~(_OS_BLOCKED);

    //rimetti in coda ready il processo
    procEnq(wakeProc, processi, &pronti);

    //chiama lo scheduler
		OSPrioSwap();
  }
  else if(sema->binario)
    sema->semaval=1;
  else
    sema->semaval++;
}
