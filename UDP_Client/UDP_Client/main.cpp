#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <chrono>
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

void main(int argc, char* argv[]) // We can pass in a command line option!!
{
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
	inet_pton(AF_INET, "129.21.28.31", &serverHint.sin_addr);
	int serverLength = sizeof(serverHint);

	// Socket creation
	//char buf[1024];
	//ZeroMemory(buf, 1024);
	//strcpy_s(buf, argv[1]);

	// Creating the connect command
	command connect;
	connect.cmd = 'c';
	ZeroMemory(connect.payload, 127);
	strcpy_s(connect.payload, "Coehl");

	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// Write out to that socket
	int sendOk = sendto(serverSocket, (char*)&connect, 128, 0, (sockaddr*)&serverHint, serverLength);

	if (sendOk == SOCKET_ERROR)
	{
		cout << "that didn't work! " << WSAGetLastError() << endl;
	}

	// Check to see if connection was established
	status serverStatus;
	ZeroMemory(serverStatus.payload, 127);

	int bytesIn = recvfrom(serverSocket, (char*)&serverStatus, 1024, 0, (sockaddr*)&serverHint, &serverLength);
	if (bytesIn == SOCKET_ERROR)
	{
		cout << "Error receiving from client " << WSAGetLastError() << endl;
	}

	// If we have connected to the server
	if (serverStatus.status == 'c')
	{
		cout << serverStatus.payload << endl;
	}

	/*
	while (true)
	{
		// Sending a message
		cout << "Type a message to send to the server" << endl;
		string msg;
		getline(cin, msg);
		ZeroMemory(buf, 1024);

		strcpy_s(buf, msg.c_str());
		//cout << msg.c_str() << endl;

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

		// display message and client info
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
	*/

	// close the socket
	closesocket(serverSocket);

	// shutdown winsock
	WSACleanup();

	system("pause");
}
