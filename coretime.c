#include "coretime.h"
#include "system_config.h"
#include <xc.h>
#include <sys/attribs.h>   

#define ONE_SECOND_RELOAD   SYSCLK/2

uint32_t CORETIME_SECONDS;

void coretime_init(void){
    IFS0bits.CTIF = 0; // clear core timer interrupt flag
    IPC0bits.CTIP = 7;
    IPC0bits.CTIS = 3;
    
    CORETIME_SECONDS = 0;
    _CP0_SET_COUNT(0);
    _CP0_SET_COMPARE(ONE_SECOND_RELOAD);
    
    IEC0bits.CTIE = 1; // enable core timer interupt
}

uint32_t coretime_now(void){ return CORETIME_SECONDS; }

void __ISR(_CORE_TIMER_VECTOR, IPL7AUTO) coretimerInterrupt(void){
    CORETIME_SECONDS++;
    _CP0_SET_COUNT(0);
    _CP0_SET_COMPARE(ONE_SECOND_RELOAD);
    IFS0bits.CTIF = 0; // clear core timer interrupt
}