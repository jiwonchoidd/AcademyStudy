#include "stdafx.h"
#include "TAsyncSelect.h"

int PrintAddress(SOCKADDR *sa, int salen)
{
	char    host[NI_MAXHOST],
		serv[NI_MAXSERV];
	int     hostlen = NI_MAXHOST,
		servlen = NI_MAXSERV,
		rc;

	rc = getnameinfo(
		sa,
		salen,
		host,
		hostlen,
		serv,
		servlen,
		NI_NUMERICHOST | NI_NUMERICSERV
		);
	if (rc != 0)
	{
		fprintf(stderr, "%s: getnameinfo failed: %d\n", __FILE__, rc);
		return rc;
	}

	// If the port is zero then don't print it
	if (strcmp(serv, "0") != 0)
	{
		if (sa->sa_family == AF_INET)
			printf("[%s]:%s", host, serv);
		else
			printf("%s:%s", host, serv);
	}
	else
		printf("%s", host);

	return NO_ERROR;
}

int FormatAddress(SOCKADDR *sa, int salen, char *addrbuf, int addrbuflen)
{
	char    host[NI_MAXHOST],
		serv[NI_MAXSERV];
	int     hostlen = NI_MAXHOST,
		servlen = NI_MAXSERV,
		rc;

	rc = getnameinfo(
		sa,
		salen,
		host,
		hostlen,
		serv,
		servlen,
		NI_NUMERICHOST | NI_NUMERICSERV
		);
	if (rc != 0)
	{
		fprintf(stderr, "%s: getnameinfo failed: %d\n", __FILE__, rc);
		return rc;
	}
	if ((strlen(host) + strlen(serv) + 1) > (unsigned)addrbuflen)
		return WSAEFAULT;
	if (sa->sa_family == AF_INET)
		sprintf(addrbuf, "%s:%s", host, serv);
	else if (sa->sa_family == AF_INET6)
		sprintf(addrbuf, "[%s]:%s", host, serv);
	else
		addrbuf[0] = '\0';

	return NO_ERROR;
}

struct addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto)
{
	struct addrinfo hints,
		*res = NULL;
	int             rc;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = ((addr) ? 0 : AI_PASSIVE);
	hints.ai_family = af;
	hints.ai_socktype = type;
	hints.ai_protocol = proto;

	if( addr == NULL)
	{
		rc = getaddrinfo(
			NULL,
			port,
			&hints,
			&res
		);
	}
	else
	{
		rc = getaddrinfo(
			addr,
			port,
			&hints,
			&res
		);
	}
	if (rc != 0)
	{
		printf("Invalid address %s, getaddrinfo failed: %d\n", addr, rc);
		return NULL;
	}
	return res;
}
