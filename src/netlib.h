#ifndef NETLIB_H
#define NETLIB_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>




//u_long  GetAddr(LPSTR);


SOCKET WINAPI __declspec (dllexport) net_get_tcp_stream_socket(void);

int WINAPI __declspec (dllexport) net_socket_set_linger( int onoff, int linger, SOCKET *fdSock );


int WINAPI __declspec (dllexport) net_socket_set_blocking_state(SOCKET *fdSock, int blocking_state);



int WINAPI __declspec (dllexport) sockConnect( char *szHost, u_short nPortNumber, SOCKET *fdSock, struct timeval stTimeOut );




WORD WINAPI __declspec (dllexport) sockClose( SOCKET fdSock );
