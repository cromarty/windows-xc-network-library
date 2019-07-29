#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>

#define SECTION( S ) __attribute__ ((section ( S )))

static void net_wsa_startup(void) {
    WORD wVersionRequested = MAKEWORD(2,0);
    WSADATA wsaData;
    int rc;

				rc = WSAStartup(wVersionRequested, &wsaData);
//				if (wsaData.wVersion != wVersionRequested)
//					return FALSE;

    //printf("Called WSAStartup()\n");
    return;
}

void (*funcptr1)(void) SECTION(".ctors") = net_wsa_startup;
//void (*funcptr2)(void) SECTION(".ctors") = test;


/*
* functions constructX use attribute 'constructor'
* to create prioritized entries in the .ctors
* ELF section.
*
* NOTE: priorities 0-100 are reserved
*
*/

void construct1 () __attribute__ ((constructor (101)));
//void construct2 () __attribute__ ((constructor (102)));

/*
* function definitions for constructX
*/

void construct1 () {
    //printf ("\n      construct1() constructor -- (.section .ctors) priority 101\n");
    return;
}

/*
void construct2 () {
    //printf ("\n      construct2() constructor -- (.section .ctors) priority 102\n");
    return;
}
*/

