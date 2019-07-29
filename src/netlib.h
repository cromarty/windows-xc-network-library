#ifndef NETLIB_H
#define NETLIB_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>


#ifdef __EXPORTING
   #define CLASS_DECLSPEC    __declspec(dllexport)
#else
   #define CLASS_DECLSPEC    __declspec(dllimport)
#endif


SOCKET WINAPI CLASS_DECLSPEC net_get_tcp_stream_socket(int *wsaErr);

int WINAPI CLASS_DECLSPEC net_socket_set_linger( int onoff, int linger, SOCKET *fdSock, int *wsaErr );


int WINAPI CLASS_DECLSPEC net_socket_set_blocking_state(SOCKET *fdSock, int blocking_state, int *wsaErr);



int WINAPI CLASS_DECLSPEC sockConnect( char *szHost, u_short nPortNumber, SOCKET *fdSock, struct timeval stTimeOut );




WORD WINAPI CLASS_DECLSPEC sockClose( SOCKET fdSock );


#endif


