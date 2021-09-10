//template은 하나의 '틀'을 의미한다.하나의 틀에서 만들어진 것들은 형태가 다 같다.
//이런 틀 기능을 구현할 때는 template method 패턴을 이용할 수 있다.
//유사한 서브 클래스가 존재할 때 template method 패턴을 사용하면 매우 유용하다.
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <iostream>
#include <string>
#include <sys/types.h>
/* --------------------------------------------
#include <sys/socket.h>		// Not exist in VC++
#include <netinet/in.h>		// Not exist in VC++
#include <netdb.h>			// Not exist in VC++
---------------------------------------------- */
#include <Winsock2.h>			// added in VC++, MUST Add Ws2_32.lib in LINK Option
#include <io.h>					// added in VC++
#include <memory.h>				// added in VC++
using namespace std;

#define MAX_BUF_SIZE    500

class InternetClient
{
public:
	void DoService(string serverName, int port) 
	{
		int sock;
		string req = MakeRequest();
		if ((sock = ConnectToServer(serverName, port)) < 0) return;
		SendRequest(sock, req);
		string rsp = RecvResponse(sock);
		ProcessResponse(rsp);
	}

	virtual string MakeRequest() = 0;
	virtual void ProcessResponse(string rsp) = 0;

	int ConnectToServer(string serverName, int port) 
	{
		int sock;
		struct sockaddr_in server;
		struct hostent *pHostInfo;

		if (serverName.empty())
			serverName = "localhost";

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			cout << "Error in Socket Creation" << endl;
			return -1;
		}

		pHostInfo = gethostbyname(serverName.data());
		if (pHostInfo == NULL) {
			cout << "Error in gethostbyname" << endl;
			return -1;
		}

		server.sin_family = pHostInfo->h_addrtype;
		memcpy((char*)&server.sin_addr, pHostInfo->h_addr, pHostInfo->h_length);
		server.sin_port = htons(port);

		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
		{
			cout << "Error in connect" << endl;
			return -1;
		}

		return sock;
	}

	void SendRequest(int sock, string req) 
	{
		_write(sock, req.data(), req.length());	// changed to _write in VC++
	}

	string RecvResponse(int sock) 
	{
		string rsp;
		int nread;
		char buf[MAX_BUF_SIZE + 1];
		// bzero(buf, MAX_BUF_SIZE);
		memset(buf, '\0', MAX_BUF_SIZE);		// bzero => memset in VC++

		do {
			nread = _read(sock, buf, MAX_BUF_SIZE);	// changed to _read in VC++
			if (nread < 0) {
				cout << "Error in read" << endl;
				return rsp;
			}
			else if (nread == 0) // -- EOF
				return rsp;
			else
				rsp += string(buf, nread);
		} while (nread >= MAX_BUF_SIZE);

		return rsp;
	}
};

class HTTPClient : public InternetClient 
{
public:
	string MakeRequest() 
	{
		string req;
		// -- 이용자로부터 URL 정보를 입력받아 HTTP Request 문자열을 생성
		req = "GET / HTTP/1.0\n\n";
		return req;
	}

	void ProcessResponse(string rsp) 
	{
		// -- 웹 브라우저에게 rsp 문자열을 전달
		cout << rsp << endl;
	}
};

class SMTPClient : public InternetClient 
{
public:
	string MakeRequest() 
	{
		string req;
		// -- 이용자로부터 정보를 입력받아 SMTP Request 문자열을 생성
		return req;
	}

	void ProcessResponse(string rsp) 
	{
		// -- 메일 발신 결과를 이용자에게 표시
	}
};

void main()
{
	HTTPClient hCli;
	hCli.DoService("localhost", 80);
}
