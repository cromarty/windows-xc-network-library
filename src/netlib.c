#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>

#include "netlib.h"



BOOL WINAPI __declspec(dllexport) LibMain(HINSTANCE hDLLInst, DWORD fdwReason, LPVOID lpvReserved) {
	WORD wVersionRequested = MAKEWORD(2,0);
	WSADATA wsaData;
	int nRet;

	switch (fdwReason) {
			case DLL_PROCESS_ATTACH:
				nRet = WSAStartup(wVersionRequested, &wsaData);
				if (wsaData.wVersion != wVersionRequested)
					return FALSE;

				break;

			case DLL_PROCESS_DETACH:
				WSACleanup();
				break;

			case DLL_THREAD_ATTACH:
				break;

			case DLL_THREAD_DETACH:
				break;
	}

	return TRUE;

} // end LibMain




SOCKET WINAPI __declspec(dllexport) net_get_tcp_stream_socket(int *wsaErr) {
    SOCKET fdSock;
    if (( fdSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        *wsaErr = WSAGetLastError();
        return INVALID_SOCKET;
    }

	return fdSock;

} // end net_get_tcp_stream_socket



int WINAPI __declspec(dllexport) net_socket_set_linger( int onoff, int linger, SOCKET *fdSock, int *wsaErr ) {
	struct linger lng;

	lng.l_onoff = onoff;
	lng.l_linger = linger;

	if ( setsockopt( *fdSock, SOL_SOCKET, SO_LINGER, (char*)&lng, sizeof(lng) ) == SOCKET_ERROR ) {
        *wsaErr = WSAGetLastError();
		return -1;
    }

	return 0;

} // end net_socket_set_linger


int WINAPI __declspec(dllexport) net_socket_set_blocking_state(SOCKET *fdSock, int blocking_state, int *wsaErr) {
	int rc;

	rc = ioctlsocket( *fdSock, FIONBIO, (u_long FAR *)&blocking_state);
	/* ioctlsocket() returns 0 on success */
	if ( rc )
		*wsaErr = WSAGetLastError();

	return rc;

} // end net_socket_set_blocking_state



int WINAPI __declspec (dllexport) sockConnect( char *szHost, u_short nPortNumber, SOCKET *fdSock, struct timeval stTimeOut ) {
	LPHOSTENT lpHostEntry;

	/* Socket structure */
	struct sockaddr_in stLclAddr;

	int wRet = SOCKET_ERROR;
	int i;
	int nWSAerror;
	int iMode = 1;
	int iError;
	int iSize = sizeof(iError);
	int iDiscard;

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);

	/* select() "file descriptor set" structures */
	fd_set stWritFDS;
	fd_set stXcptFDS;
	fd_set stReadFDS;


	int nBufs = 4096;

	*fdSock = socket( AF_INET, SOCK_STREAM, 0 );
	if (*fdSock == INVALID_SOCKET) {
		goto AppExit;
	}

///////////////////////////

	// set receive buffer size to 4k
	if ( setsockopt( *fdSock, SOL_SOCKET, SO_RCVBUF, (char*)&nBufs, sizeof(int) ) == SOCKET_ERROR ) {
		wRet = 77;
		goto AppExit;
	}



	/* Initialize the Sockets Internet Address (sockaddr_in) structure */
	memset ((LPSTR)&(stLclAddr), 0, sizeof(struct sockaddr_in));

	lpHostEntry = gethostbyname( szHost );
	wRet = WSAGetLastError();
	if ( wRet != 0) {
		goto AppExit;
	}

	/* convert port number from host byte order to network byte order */
	memcpy( &stLclAddr.sin_addr, lpHostEntry->h_addr, lpHostEntry->h_length);
	stLclAddr.sin_port   = htons ( nPortNumber );
	stLclAddr.sin_family = AF_INET; /* Internet Address Family */

	/* try to connect */

	wRet = connect( *fdSock, (struct sockaddr FAR*)&stLclAddr, sizeof(struct sockaddr_in));
	/* on non-blocking connect, we expect WSAEWOULDBLOCK error
	* but WSAEALREADY is ok too, and WSAEISCONN means we're done.
	*/
	if ( wRet == SOCKET_ERROR ) {
		nWSAerror = WSAGetLastError();
		if (nWSAerror == WSAEISCONN) {             // already connected
			wRet = 0;
			goto AppExit;
		} else {
			if ( nWSAerror != WSAEWOULDBLOCK ) {      // some other error apart from would block
				// here possibly. maybe should return SOCKET_ERROR
				wRet = nWSAerror;
				if  ( nWSAerror == 0 )
					wRet = SOCKET_ERROR;

				goto AppExit;
			}
		}
	}

	/* clear all sockets from FDS structures */
	FD_ZERO((fd_set FAR*)&(stWritFDS));
	FD_ZERO((fd_set FAR*)&(stXcptFDS));
	FD_ZERO((fd_set FAR*)&(stReadFDS));

	/* put our socket  into the socket descriptor wwrite and exception sets */
	FD_SET( *fdSock, (fd_set FAR*)&(stWritFDS));
	FD_SET( *fdSock, (fd_set FAR*)&(stXcptFDS));

	/*
	* call select() with write and exception sets.
	* writeable state indicates connection complete.
	* exception state is obviously an error
	*/
	wRet = select( -1, NULL, (fd_set FAR*)&(stWritFDS), (fd_set FAR*)&(stXcptFDS), (struct timeval FAR *)&(stTimeOut));

	if ( wRet == SOCKET_ERROR ) {     /* check return */
		 wRet = WSAGetLastError();
		goto AppExit;
	} else {
		/*
		* something other than SOCKET_ERROR
		* > 0 indicates there is socket activity, so need to check.
		*/
		if ( wRet > 0 ) {
			/* check for error (exception) first */
			if ( FD_ISSET ( *fdSock,  (fd_set FAR*)&(stXcptFDS ))) {
				/* get the exception error */
				wRet = getsockopt( *fdSock, SOL_SOCKET,  SO_ERROR,  (char*)&iError, &iSize );
				if ( wRet == SOCKET_ERROR )
					goto AppExit;

				wRet = iError;
				goto AppExit;
			}

			/*
			* here there is activity on the write set, so socket must be connected.
			* writeableness indicates completed connection.
			*/

		} else {
			// timed out
			wRet = 99;
			goto AppExit;
		}
	}

		wRet = 0;

AppExit:

	return wRet;

} // end sockConnect




WORD WINAPI __declspec (dllexport) sockClose( SOCKET fdSock ) {

	int wRet, nWSAerror;

	// Cancel any pending blocking operation
	if (WSAIsBlocking()) {
		WSACancelBlockingCall();
	}

	nWSAerror = 0;
	wRet = closesocket(fdSock);
	if (wRet == SOCKET_ERROR) {
		nWSAerror = WSAGetLastError();
		/* if not "would block" error, report it */
		if ( nWSAerror != WSAEWOULDBLOCK) {

		}
	}

	return (wRet);

}  // end sockClose


