#include "mutex.h"

void OSCreaMutex(OSMutex *mutex){
  unsigned char csreg;
  OSMakeAtomic(&csreg);
  mutex->val = 1;
  init_coda(mutex->procList, OSMAX_TASKS, &mutex->blocked); //inizializza la coda dei processi blocked
  OSExitAtomic(csreg);
}

void OSMutexWait(OSMutex *mutex){
  unsigned char csreg;
  OSMakeAtomic(&csreg);
  if(!mutex->val){
    enq(running, processi, &mutex->blocked);
    processi[running].status |= _OS_BLOCKED;
    OSExitAtomic(csreg);
		OS_change();
  }
  else
    mutex->val = 0;

  OSExitAtomic(csreg);
}

void OSMutexPost(OSMutex *mutex){
  unsigned char csreg;
  OSMakeAtomic(&csreg);
  unsigned char wakeProc = procDeq(&mutex->blocked);

  if(wakeProc!=255){
    processi[wakeProc].status &= _OS_BLOCKED;
    enq(wakeProc, processi, &pronti);
    OSExitAtomic(csreg);
    OS_change();
  }
  else
    mutex->val = 1;

  OSExitAtomic(csreg);
}
