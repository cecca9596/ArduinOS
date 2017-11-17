#include "kernel.h"

void processo1(){
	while(1){
		//cose
		//
		//
		//
		OS_change();
	}
}

void processo2(){
	while(2){
		//altre cose
		//
		//
		//
		OS_change();
	}
}
void setup(){
	OS_init(2);
	
	OS_crea_processo(1,processo1);
	
	OS_crea_processo(2,processo2);
	
	OS_run();
}
void loop(){
	//vuoto
}
