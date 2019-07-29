#include <stdio.h>
#include <stdlib.h>

#define SECTION( S ) __attribute__ ((section ( S )))

static void net_wsa_cleanup(void) {
    WSACleanup();
    //printf("Called WSACleanup()\n");
    return;
}

void (*funcptr3)(void) SECTION(".dtors") =net_wsa_cleanup;

/*
* Functions destructX use attribute 'destructor'
    * to create prioritized entries in the .dtors
* ELF section.
*
* NOTE: priorities 0-100 are reserved
*
*/

void destruct1 () __attribute__ ((destructor (101)));
//void destruct2 () __attribute__ ((destructor (102)));

void destruct1 () {
    //printf ("\n      destruct1() destructor -- (.section .dtors) priority 101\n\n");
    return;
}

/*
void destruct2 () {
    //printf ("\n      destruct2() destructor -- (.section .dtors) priority 102\n");
    return;
}
*/

