#pragma once
#include "stdafx.h"
#include "Commands.h"

/*
	Coehl Gleckner
	IGME 589.02 - Networking
	2/11/2019
	Hello Dungeon

	I used a tutorial from youtube to set up the basics of the UDP server and client
	Found here: https://www.youtube.com/watch?v=uIanSvWou1M
	And then expanded on it to create the rest of the application
*/

void main(int argc, char* argv[]) // We can pass in a command line option!!
{
	// Initialize the commands
	Commands commandList;

	// Startup Winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock! " << wsOk;
	}

	// Create a hint structure for the server
	sockaddr_in serverHint;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(32954);

	// converts from string to binary for address
	inet_pton(AF_INET, "129.21.28.31", &serverHint.sin_addr); // address is of the server
	int serverLength = sizeof(serverHint);

	// Socket creation
	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// Write out to that socket
	int sendOk = sendto(serverSocket, (char*)&commandList.connect, 128, 0, (sockaddr*)&serverHint, serverLength);

	if (sendOk == SOCKET_ERROR)
	{
		cout << "that didn't work! " << WSAGetLastError() << endl;
	}

	// Check to see if connection was established
	status serverStatus;
	ZeroMemory((char*)&serverStatus, 128);

	int bytesIn = recvfrom(serverSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&serverHint, &serverLength);
	if (bytesIn == SOCKET_ERROR)
	{
		cout << "Error receiving from client " << WSAGetLastError() << endl;
	}

	// If we have connected to the server
	if (serverStatus.status == 'c')
	{
		cout << serverStatus.payload << endl;
	}

	while (true)
	{
		ZeroMemory((char*)&serverStatus, 128);

		// Sending a message
		cout << "Insert a command" << endl;
		string msg;
		getline(cin, msg);
		
		// Check to see if msg is a valid command

		if (msg == "Display")
		{

		}

		// send the command to the server
		sendOk = sendto(serverSocket, buf, 1024, 0, (sockaddr*)&serverHint, serverLength);

		if (sendOk == SOCKET_ERROR)
		{
			cout << "that didn't work! " << WSAGetLastError() << endl;
		}

		cout << endl;

		// Receiving a message
		ZeroMemory(buf, 1024);

		// wait for message
		int bytesIn = recvfrom(serverSocket, buf, 1024, 0, (sockaddr*)&serverHint, &serverLength);
		if (bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client " << WSAGetLastError() << endl;
			continue;
		}

		// Display the server IP
		char serverIp[256];
		ZeroMemory(serverIp, 256);

		inet_ntop(AF_INET, &serverHint.sin_addr, serverIp, 256);

		cout << "Message recv from " << serverIp << " : " << buf << endl;

		// checks to see if we should exit the program
		if (strcmp(buf, "exit") == 0)
		{
			cout << buf << endl;
			cout << "Exiting Program..." << endl;
			//break;
		}
	}
	

	// close the socket
	closesocket(serverSocket);

	// shutdown winsock
	WSACleanup();

	system("pause");
}
