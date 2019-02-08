#include <iostream>
#include <WS2tcpip.h>
#include <string>
// use std chrono for timing if needed

#pragma comment (lib, "ws2_32.lib")

using namespace std;

/* 
	Coehl Gleckner
	IGME 589.02 - Networking
	2/11/2019
	Hello Dungeon
	
	I used a tutorial from youtube to set up the basics of the UDP server and client
	Found here: https://www.youtube.com/watch?v=uIanSvWou1M
	And then expanded on it to create the rest of the application
*/ 

struct command
{
	unsigned char cmd; // 256 commands for now
	char payload[127]; // Some payload based on the command
};

struct status
{
	unsigned char status; // 256 status tpyes for now
	char payload[127]; // Some payload based
};

void main()
{
	// Startup Winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock! " << wsOk;
	}

	// Bind socket to ip address and port
	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;

	// converts from string to binary for address
	inet_pton(AF_INET, "129.21.28.31", &serverHint.sin_addr);
	//serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(32954); // Convert from little to big endian

	if (bind(clientSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}

	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, sizeof(client));

	//char buf[1024];
	command clientConnect;
	clientConnect.cmd = 'y';
	ZeroMemory(clientConnect.payload, 127);

	// Enter a loop
	while (true)
	{
		//ZeroMemory(buf, 1024);

		// wait for message
		int bytesIn = recvfrom(clientSocket, (char*)&clientConnect, 128, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client " << WSAGetLastError() << endl;
			continue;
		}

		// display message and client info
		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		if (clientConnect.cmd == 'c')
		{
			// Connected to Client
			cout << "Player has connected from " << clientIp << endl;
			cout << "Name: " << clientConnect.payload << endl;

			// Prepare the server status
			status serverStatus;
			ZeroMemory(serverStatus.payload, 127);
			serverStatus.status = 'c';
			strcpy_s(serverStatus.payload, "Welcome to the dungeon");

			// Send the status to the client
			int sendOk = sendto(clientSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&client, clientLength);

			if (sendOk == SOCKET_ERROR)
			{
				cout << "that didn't work! " << WSAGetLastError() << endl;
			}
		}

		/*
		else if (strcmp(buf, "") != 0)
		{
			cout << "Message recv : "<< buf << endl;
			bool exitCheck = false;
			// checks to see if we should exit the program
			if (strcmp(buf, "exit") == 0)
			{
				exitCheck = true;
				msg = "exit";
			}
			else if (strcmp(buf, "hello") == 0)
			{
				msg = "Hey there bud";
			}
			else
			{
				msg = "Message not recognized";
			}
			
			// Send a message back
			cout << "Returning a msg to the client" << endl;
			//cout << msg.c_str() << endl;
			ZeroMemory(buf, 1024);
			strcpy_s(buf, msg.c_str());

			int sendOk = sendto(clientSocket, buf, 1024, 0, (sockaddr*)&client, clientLength);

			if (sendOk == SOCKET_ERROR)
			{
				cout << "that didn't work! " << WSAGetLastError() << endl;
			}

			cout << endl;

			if (exitCheck)
			{
				cout << "Exiting Program..." << endl;
				//break;
			}
		}
		*/
	}

	// close socket
	closesocket(clientSocket);

	// shutdown winsock
	WSACleanup();
}
