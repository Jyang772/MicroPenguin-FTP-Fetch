using namespace std;
#include <iostream>
#pragma warning (disable:4996)
#include <cstring>
#include <cstdio>

#include <winsock.h>
#include <windows.h>

int main() {

	const int MAX_TRIES = 10;

	char * host = "micropenguin.net";
	int port = 21;
	char * userName = "";
	char * pass = "";
	//char * testFileSource = "C:\\Windows\\notepad.exe";

	WSADATA WSAData;
	SOCKADDR_IN server;
	SOCKET sock;

	WSAStartup(MAKEWORD(2, 2), &WSAData);
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "fail";
		system("PAUSE");
		return -1;
	}

	server.sin_family = PF_INET;
	server.sin_port = htons(port);
	server.sin_addr = *((struct in_addr *)gethostbyname(host)->h_addr);
	memset(server.sin_zero, 0, 8);

	int errorCode = connect(sock, (LPSOCKADDR)&server, sizeof(struct sockaddr));
	int tries = 0;

	while (errorCode == SOCKET_ERROR) {
		if (tries >= MAX_TRIES) {
			cout << "fail 2";
			system("PAUSE");
			return -1;
		}
		errorCode = connect(sock, (LPSOCKADDR)&server, sizeof(struct sockaddr));
		tries++;
	}


	char serverMsg[2048];
	Sleep(1000);
	cout << "Waiting for server response..." << endl;
	int r = recv(sock, serverMsg, 2048, 0);
	serverMsg[r] = '\0';
	cout << endl << endl << "Server said: " << endl << serverMsg << endl << endl;

	char userB[1024] = "USER ";
	strcat(userB, userName);
	strcat(userB, "\n");
	cout << "Sending... " << userB << endl;
	cout << "sended: " << send(sock, userB, strlen(userB), 0) << " bytes" << endl;
	Sleep(1000);
	cout << "Waiting for server response..." << endl;

	serverMsg[0] = '\0';

	recv(sock, serverMsg, 2048, 0); // <-- program keeps lock here
	cout << endl << endl << "Server said: " << endl << serverMsg << endl << endl;

	char userPass[1024] = "PASS ";
	strcat(userPass, pass);
	strcat(userPass, "\n");
	send(sock, userPass, strlen(userPass), 0);

	recv(sock, serverMsg, 2048, 0); // <-- program keeps lock here
	cout << endl << endl << "Server said: " << endl << serverMsg << endl << endl;


	system("PAUSE");
	return 0;
}
