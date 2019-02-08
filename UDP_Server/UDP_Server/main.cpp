#pragma once
#include "stdafx.h"
#include "Dungeon.h"

/* 
	Coehl Gleckner
	IGME 589.02 - Networking
	2/11/2019
	Hello Dungeon
	
	I used a tutorial from youtube to set up the basics of the UDP server and client
	Found here: https://www.youtube.com/watch?v=uIanSvWou1M
	And then expanded on it to create the rest of the application
*/ 

void main()
{
	Status statusList;
	Dungeon dungeon;

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
	inet_pton(AF_INET, "129.21.28.31", &serverHint.sin_addr); // address is for THIS computer, use ipconfig in command prompt
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(32954); // Convert from little to big endian

	// binds the socket for the client to use
	if (bind(clientSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}

	// creates the client
	sockaddr_in client;
	int clientLength = sizeof(client);

	cout << "Dungeon Started" << endl;

	// Enter a loop
	while (true)
	{
		// Wait for a client command
		command clientCommand;
		ZeroMemory((char*)&clientCommand, 128);
		ZeroMemory(&client, clientLength);

		int bytesIn = recvfrom(clientSocket, (char*)&clientCommand, 128, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client " << WSAGetLastError() << endl;
			continue;
		}

		// Display the client info and payload
		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		// For the connect command
		if (clientCommand.cmd == 'c')
		{
			// Connected to Client
			cout << "Player has connected from " << clientIp << endl;

			// Create a new player in the dungeon
			dungeon.AddPlayer(clientIp, clientCommand.payload);

			// Send the status to the client
			int sendOk = sendto(clientSocket, (char*)&statusList.welcome, 128, 0, (sockaddr*)&client, clientLength);
			if (sendOk == SOCKET_ERROR)
			{
				cout << "that didn't work! " << WSAGetLastError() << endl;
			}
		}

	}

	// close socket
	closesocket(clientSocket);

	// shutdown winsock
	WSACleanup();
}
