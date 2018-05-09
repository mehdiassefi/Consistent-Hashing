#include "stdafx.h"
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 50
using namespace std;
#define WIN32_LEAN_AND_MEAN
#define BUFFSIZE 359
#include "stdafx.h"
#include <winsock2.h>
#include <process.h>
#include <fstream>
#include <thread>
#pragma comment(lib,"ws2_32.lib" )
unsigned int __stdcall  rcv_thrd(void *data);
unsigned int __stdcall  snd_thrd(void *data);
using namespace std;
char myhash[2000][20];
char buff[BUFFSIZE];
int config[10][3] = { 0 };
void init()
{
	for (int i = 0; i < 2000; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			myhash[i][j] = '_';
		}
	}

	for (int j = 0; j < BUFFSIZE; j++)
	{
		buff[j] = '_';
	}
	ifstream fin;
	char my_character;
	int idx = 0;
	fin.open("C:\\Users\\BRG_user\\Desktop\\config.txt", ios::in);
	int i = 0, j = 0;
	while (j<BUFFSIZE || my_character != '*')
	{
		fin.get(my_character);
		if (my_character == '\n')my_character = ',';
		buff[j++] = my_character;
		if (my_character == '*')break;
	}
	int count = 0, key = 0, n = 0;
	j = 0;
	while (j<BUFFSIZE)
	{
		while (buff[j] != ',')key = key * 10 + (buff[j++] - 48);
		j++;
		while (buff[j] != ',')
		{
			myhash[key][n] = buff[j];
			n++;
			j++;
		}
		j++;
		key = 0;
		n = 0;
	}
}
int mymain()
{
	char command[20];
	int idx = 0, PORT = 222, n = 0;
	while (1)
	{
		//cin >> PORT;
		cout << "\nHome:\\Bootstrap Server>";
		for (int k = 0; k < 20; k++)command[k] = '\0';
		cin >> command;
		//cout << "Home:\\Bootstrap Server>";
		if (!strcmp(command, "lookup"))
		{
			int ptrrmp = 0;
			while (config[ptrrmp][0])ptrrmp++;
			ptrrmp--;
			cout << "enter the key: ";
			cin >> idx;
			int ptr = ptrrmp;
			while (config[ptr][2] <= idx)
			{
				cout << "Home:\\Bootstrap Server>Name Server " << config[ptr][0] << " is contacted!\n";
				ptr--;
			}
			cout << "Home:\\Bootstrap Server>";
			if (myhash[idx][n] == '_')
			{
				cout << "Not found!\n";
				cout << "Home:\\Bootstrap Server>";
				n = 0;
				continue;
			}
			else
			{
				n = 0;
				while (n<10 && myhash[idx][n] != '_')//myhash[idx][n] != '_' || myhash[idx][n] != '\0' || 
				{
					cout << myhash[idx][n++];

				}
			}

			//exit(0);
		}
		else if (!strcmp(command, "config"))
		{
			int last2 = 0;
			cout << "\n\nHere is the configuration table:\n\n";
			cout << "ID\tPort#\tHash\n";
			cout << "====================\n";
			while (config[last2][0])
			{
				cout << config[last2][0] << '\t' << config[last2][1] << '\t' << config[last2][2] << '\n';
				last2++;
			}
			//break;

		}
		else if (!strcmp(command, "insert"))
		{
			int indx;
			char val[5] = "\0";
			cout << "Enter the value:";
			cin >> val;
			cout << val;
			cout << "Home:\\Bootstrap Server>Enter the hash value:";
			cin >> indx;
			int i = 0;
			while (val[i])
			{
				myhash[indx][i] = val[i];
				i++;
			}
			val[i] = '_';
			val[i+1] = '_';
			//break;
		}
		else if (!strcmp(command, "delete"))
		{
			int indx;
			char val[5] = "\0";
			cout << "Enter the key:";
			
			cin >> indx;
			int i = 0;
			while (i<10)
			{
				myhash[indx][i] = '_';
				i++;
			}
			//break;
		}
		else cout << "command is not recognized!";
		/*char hash[10] = { "Test!" };
		WSADATA wsaData;
		int iResult;
		sockaddr_in addr;
		SOCKET sock;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
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
		iResult = connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in));
		send(sock, hash, sizeof(hash), 0); closesocket(sock);*/

	}
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{

	init();
	WSADATA wsaData;
	int iResult;
	thread thread_1 = thread(mymain);
	sockaddr_in addr;
	SOCKET sock, client;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//cout << "Home:\\Bootstrap Server>";
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
	while (client = accept(sock, 0, 0))
	{
		if (client == INVALID_SOCKET)
		{
			printf("invalid client socket", GetLastError());
			continue;
		}
		_beginthreadex(0, 0, rcv_thrd, (void*)&client, 0, 0);
		//_beginthreadex(0, 0, snd_thrd, (void*)&client, 0, 0);
	}

	thread_1.join();
	return 0;
}

unsigned int __stdcall rcv_thrd(void *data)
{
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;
	int ID = GetCurrentThreadId();
	char hash[10];
	char chunk[200];
	recv(Client, hash, 200, 0);
	int sizeofhash = 0;
	int hashvalue = 0;
	for (sizeofhash = 0; hash[sizeofhash] != '\0'; sizeofhash++);
	/*for (int i = 0; i < sizeofhash; i++)
	{
	hashvalue = (hashvalue * 10) + (hash[i] - 48);
	}*/
	//cout << "Home:\\Bootstrap Server>";
	//cout << "Name Bootstrap Server " << ID << " connected" << " with hash value of " << hashvalue << "\n";
	cout << "Name Server " << hash[1] << hash[2] << " is connected with the hash value of: " << hash[6] << hash[7] << hash[8] << hash[9] << " and port# of: " << hash[3] << hash[4] << hash[5];
	cout << "\nHome:\\Bootstrap Server>";
	int last = 0;
	while (config[last][0])last++;
	config[last][0] = ((hash[1] - 48) * 10) + (hash[2] - 48);
	config[last][1] = ((hash[3] - 48) * 100) + ((hash[4] - 48) * 10) + (hash[5] - 48);
	config[last][2] = ((hash[6] - 48) * 1000) + ((hash[7] - 48) * 100) + ((hash[8] - 48) * 10) + (hash[9] - 48);

	/*while (config[last][0])
	{
	cout << config[last][0] <<' ' << config[last][1] << ' ' << config[last][2]<<'\n';
	last++;
	}*/

	//---------------------------SORT-----------------------------
	int temp;
	for (int i = 0; i <= last; i++)
	{
		for (int j = i + 1; j <= last; j++)
		{
			if (config[i][2] < config[j][2])
			{
				temp = config[i][0];
				config[i][0] = config[j][0];
				config[j][0] = temp;
				temp = config[i][1];
				config[i][1] = config[j][1];
				config[j][1] = temp;
				temp = config[i][2];
				config[i][2] = config[j][2];
				config[j][2] = temp;
			}
		}
	}
	//---------------------------------------------------------


	while (recv(Client, chunk, 200, 0))
	{

		if (!strcmp(chunk, "exit"))
		{
			last = 0;
			while (config[last][0])last++;
			cout << "Name Server " << hash[1] << hash[2] << " left!\n";
			cout << "Home:\\Bootstrap Server>";
			int last1 = 0;
			int id_ = ((hash[1] - 48) * 10) + (hash[2] - 48);
			while (config[last1][0] != id_)last1++;
			config[last1][0] = 0;
			config[last1][1] = 0;
			config[last1][2] = 0;
			//---------------------------SORT-----------------------------
			int temp;
			for (int i = 0; i <= last; i++)
			{
				for (int j = i + 1; j <= last; j++)
				{
					if (config[i][2] < config[j][2])
					{
						temp = config[i][0];
						config[i][0] = config[j][0];
						config[j][0] = temp;
						temp = config[i][1];
						config[i][1] = config[j][1];
						config[j][1] = temp;
						temp = config[i][2];
						config[i][2] = config[j][2];
						config[j][2] = temp;
					}
				}
			}
			//---------------------------------------------------------

			break;
		}
		cout << "Message from name Bootstrap Server " << ID << ": " << chunk << "\n";
		cout << "Home:\\Bootstrap Server>";
	}
	return 0;
}

unsigned int __stdcall snd_thrd(void *data)
{
	char command[20];
	int idx = 0, PORT = 222, n = 0;
	while (1)
	{
		//cin >> PORT;
		cout << "\nHome:\\Bootstrap Server>";
		for (int k = 0; k < 20; k++)command[k] = '\0';
		cin >> command;
		//cout << "Home:\\Bootstrap Server>";
		if (!strcmp(command, "lookup"))
		{
			int ptrrmp = 0;
			while (config[ptrrmp][0])ptrrmp++;
			ptrrmp--;
			cout << "enter the key: ";
			cin >> idx;
			int ptr = ptrrmp;
			while (config[ptr][2] < idx)
			{

				cout << "Home:\\Bootstrap Server>Name Server " << config[ptr][0] << " is contacted!\n";
				ptr--;
			}
			cout << "Home:\\Bootstrap Server>";
			if (myhash[idx][n] == '_')
			{
				cout << "Not found!\n";
				cout << "Home:\\Bootstrap Server>";
				n = 0;
				continue;
			}
			else
			{
				n = 0;
				while (myhash[idx][n] != '_')
				{
					cout << myhash[idx][n++];

				}
			}

			//exit(0);
		}
		if (!strcmp(command, "config"))
		{
			int last2 = 0;
			cout << "\n\nHere is the configuration table:\n\n";
			cout << "ID\tPort#\tHash\n";
			cout << "====================\n";
			while (config[last2][0])
			{
				cout << config[last2][0] << '\t' << config[last2][1] << '\t' << config[last2][2] << '\n';
				last2++;
			}
			//break;

		}
		if (!strcmp(command, "add"))
		{
			int indx;
			char val[5] = "\0";
			cout << "Enter the value:";
			cin >> val;
			cout << val;
			cout << "Home:\\Bootstrap Server>Enter the hash value:";
			cin >> indx;
			int i = 0;
			while (val[i])
			{
				myhash[indx][i] = val[i];
				i++;
			}
			//break;
		}
		/*char hash[10] = { "Test!" };
		WSADATA wsaData;
		int iResult;
		sockaddr_in addr;
		SOCKET sock;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
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
		iResult = connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in));
		send(sock, hash, sizeof(hash), 0); closesocket(sock);*/

	}
	return 0;
}
