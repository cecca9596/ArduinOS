#include <kernel.h>

void task1(void *p){
  while(1){
   digitalWrite(13,HIGH);
   Serial.println("alto");
   delay(1000);
   OS_change(); 
  }
}

void task2(void *p){
  while(1){
    digitalWrite(13,LOW);
    Serial.println("basso");
    delay(1000);
    OS_change();
  }
}



void setup(){
 Serial.begin(9600);
 pinMode(13,OUTPUT);
 OS_init(2);
 OS_crea_processo(task1);
OS_crea_processo(task2);

OS_run();
}
void loop(){
}
