//http://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancedmulticast9a.html
#define  _WINSOCK_DEPRECATED_NO_WARNINGS


#ifndef _RESOLVE_H_
#define _RESOLVE_H_
#ifdef _cplusplus
extern "C" {
#endif

	int              PrintAddress(SOCKADDR *sa, int salen);
	int              FormatAddress(SOCKADDR *sa, int salen, char *addrbuf, int addrbuflen);
	struct addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto);

#ifdef _cplusplus
}
#endif
#endif
