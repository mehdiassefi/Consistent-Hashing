#include "stdafx.h"
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 50
using namespace std;
#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <winsock2.h>
#include <process.h>
#include <thread>
#pragma comment(lib,"ws2_32.lib" )
int IN_PORT;
char hashvalue[10];
int ID;
char  msg[10];
int sndThrd()
{

	WSADATA wsaData;
	int iResult;
	sockaddr_in addr;
	SOCKET sock;
	char hash[10];
	msg[0] = 'e';
	msg[1] = (ID / 10) + 48;
	msg[2] = (ID % 10) + 48;
	msg[3] = (IN_PORT / 100) + 48;
	msg[4] = ((IN_PORT % 100) / 10) + 48;
	msg[5] = (IN_PORT % 10) + 48;
	msg[6] = hashvalue[0];
	msg[7] = hashvalue[1];
	msg[8] = hashvalue[2];
	msg[9] = hashvalue[3];


	//cout << "Home:\\Cleint>Please enter the HASH value for the name server: ";
	//cin >> hash;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult)
	{
		printf("WSA startup failed");
		return 0;
	}
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("Invalid socket");
		return 0;
	}
	while (1)
	{
		iResult = connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in));
		int i = 0;
		if (iResult)
		{
			system("cls");
			cout << "Home:\\Cleint>Connecing...";
			for (int cnt = 0; cnt <= (i % 3); cnt++)cout << ".";
		}
		else
		{
			cout << "Connection succed!";
			cout << "\nHome:\\Name Server>";
			break;
		}
	}
	char ptr[100];
	send(sock, msg, sizeof(msg), 0);
	while (1)
	{
		cin >> ptr;
		cout << "Home:\\Name Server>";
		if (!strcmp(ptr, "exit"))
		{
			send(sock, ptr, sizeof(ptr), 0);
			Sleep(1000);
			exit;
		}
		send(sock, ptr, sizeof(ptr), 0);
	}
	closesocket(sock);
	return 0;
}
int rcvThrd()
{
	WSADATA wsaData;
	int iResult;
	sockaddr_in addr;
	SOCKET sock, client;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(IN_PORT);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult)
	{
		printf("WSA startup failed");
		return 0;
	}
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("Invalid socket");
		return 0;
	}
	iResult = ::bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (iResult)
	{

		printf("bind failed %u", GetLastError());

		return 0;
	}
	iResult = listen(sock, SOMAXCONN);
	if (iResult)
	{

		printf("iResult failed %u", GetLastError());

		return 0;
	}
	char chunk[200];
	while (1)
	{
		client = accept(sock, 0, 0);
		if (client == INVALID_SOCKET)
		{
			printf("invalid client socket", GetLastError());
			continue;
		}
		recv(client, chunk, 200, 0);
		cout << "Message from name server " << chunk << "\n";
		cout << "Home:\\Name Server>";
	}
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Home:\\Name Server>What is the port number?";
	cin >> IN_PORT;
	cout << "Home:\\Name Server>Enter the HASH value: ";
	cin >> hashvalue;
	cout << "Home:\\Name Server>What is the ID?";
	cin >> ID;
	cout << "Home:\\Name Server>";
	thread thread_1 = thread(sndThrd);
	thread thread_2 = thread(rcvThrd);
	thread_1.join();
	thread_2.join();
}