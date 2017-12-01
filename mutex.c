#include "mutex.h"

void OSCreaMutex(OSMutex *mutex){
  OSMakeAtomic();
  mutex->val = 1;
  initQ(&(mutex->procList, OSMAX_TASKS, &mutex->blocked); //inizializza la coda dei processi blocked
  OSExitAtomic();
}

void OSMutexWait(OSMutex *mutex){
  OSMakeAtomic();
  if(!mutex < val){
    procEnq(running, processi, &mutex->blocked);
    processi[running].status |= _OS_BLOCKED;
    OSExitAtomic();
		OSSwap();
  }
  else
    mutex->val = 0;

  OSExitAtomic();
}

void OSMutexPost(OSMutex *mutex){
  OSMakeAtomic();
  unsigned char wakeProc = procDeq(&mutex->blocked);

  if(wakeProc!=255){
    processi[wakeProc].status &= _OS_BLOCKED;
    procEnq(wakeProc, processi, &pronti);
    OSExitAtomic();
    OSPrioSwap();
  }
  else
    mutex->val = 1;

  OSExitAtomic();
}
