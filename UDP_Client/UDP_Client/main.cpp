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

	cout << "Please enter the IP of the server you wish to connect with" << endl;
	// sets up the IP with input
	//string myIp = "129.21.178.67";
	int ipNum;
	string ip = "";
	for (int i = 0; i < 4;)
	{
		cout << "Insert a value for the " << i << " digit in the ip (0 - 255)" << endl;
		string input;
		getline(cin, input);
		cout << endl;

		ipNum = stoi(input);

		if (ipNum >= 0 || ipNum <= 255)
		{
			if (i < 3)
			{
				ip += input + ".";
				i++;
			}
			else
			{
				ip += input;
				i++;
			}
		}
	}

	// gets input for the port
	int port = -1;
	while (port < 1024 || port > 65535)
	{
		cout << "Insert a port value (1024-65535)" << endl;
		string input;
		getline(cin, input);
		cout << endl;

		port = stoi(input);
	}

	// Create a hint structure for the server
	sockaddr_in serverHint;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(port);

	// converts from string to binary for address
	inet_pton(AF_INET, ip.c_str(), &serverHint.sin_addr); // address is of the server
	int serverLength = sizeof(serverHint);

	// Socket creation
	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// Getting the serverIp
	char serverIp[256];
	ZeroMemory(serverIp, 256);

	inet_ntop(AF_INET, &serverHint.sin_addr, serverIp, 256);

	// Preparing the status for the server
	status serverStatus;
	ZeroMemory((char*)&serverStatus, 128);

	// Test the connection to the server
	cout << "Testing connection to server, if the next message does not appear please restart the program" << endl;
	int testSend = sendto(serverSocket, (char*)&commandList.testConnect, 128, 0, (sockaddr*)&serverHint, serverLength);
	if (testSend == SOCKET_ERROR)
	{
		cout << "Sending the test message didn't work : " << WSAGetLastError() << endl;
	}

	// Wait for a response from the server
	int bytesInTest = recvfrom(serverSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&serverHint, &serverLength);
	if (bytesInTest == SOCKET_ERROR)
	{
		cout << "Error receiving from client " << WSAGetLastError() << endl;
	}

	// Setting some variables for the loop to use
	bool inDungeon = false;
	string name;

	cout << "Insert help to see a list of commands" << endl;

	while (true)
	{
		// Sending a message
		cout << endl << "Insert a command" << endl;
		string msg;
		getline(cin, msg);
		cout << endl;

		// Check to see if msg is a valid command
		for (int i = 0; i < msg.length(); i++)
		{
			msg[i] = tolower(msg[i]);
		}

		// Process the input
		int sendOk;

		// help display
		if (msg == "help")
		{
			cout << "Command List" << endl;
			cout << "help - Displays a list of commands for the player to utilize." << endl;
			cout << "enter - Adds the player to dungeon, must be called first to be in the dungeon." << endl;
			cout << "display - Shows the players names and the current dungeon layout." << endl;
			cout << "leave - Allows the player to escape the dungeon with what treasure they have collected." << endl;
			cout << "move - Takes in an additional command (north south east or west) to move the player in that direction." << endl;
			cout << "inspect - Lets the player look around the room for treasure, other players, and which directions they can go." << endl;
			cout << "gettreasure - Allows the player to take the treasure if there is any in the room." << endl;
			cout << "treasureamnt - Allows the player to see how much treasure they have, and how much is left in the dungeon." << endl;
			continue;
		}
		
		// Out of dungeon commands
		else if (msg == "enter")
		{
			if (!inDungeon)
			{
				// Get the new player's name
				cout << "Please type a name for your character" << endl;
				getline(cin, name);

				// set the payload with the name
				strcpy_s(commandList.connect.payload, name.c_str());
				strcpy_s(commandList.display.payload, name.c_str());
				strcpy_s(commandList.leave.payload, name.c_str());
				strcpy_s(commandList.inspect.payload, name.c_str());
				strcpy_s(commandList.move.payload, name.c_str());
				strcpy_s(commandList.getTreasure.payload, name.c_str());
				strcpy_s(commandList.treasureAmnt.payload, name.c_str());

				sendOk = sendto(serverSocket, (char*)&commandList.connect, 128, 0, (sockaddr*)&serverHint, serverLength);
				inDungeon = true;
			}
			else
			{
				cout << "Your already in the Dungeon" << endl;
				continue;
			}
		}

		else if (msg == "display")
			sendOk = sendto(serverSocket, (char*)&commandList.display, 128, 0, (sockaddr*)&serverHint, serverLength);

		// In dungeon commands
		else if (inDungeon)
		{
			if (msg == "leave")
				sendOk = sendto(serverSocket, (char*)&commandList.leave, 128, 0, (sockaddr*)&serverHint, serverLength);

			else if (msg == "move")
				sendOk = sendto(serverSocket, (char*)&commandList.move, 128, 0, (sockaddr*)&serverHint, serverLength);

			else if (msg == "inspect")
				sendOk = sendto(serverSocket, (char*)&commandList.inspect, 128, 0, (sockaddr*)&serverHint, serverLength);

			else if (msg == "gettreasure")
				sendOk = sendto(serverSocket, (char*)&commandList.getTreasure, 128, 0, (sockaddr*)&serverHint, serverLength);

			else if (msg == "treasureamnt")
				sendOk = sendto(serverSocket, (char*)&commandList.treasureAmnt, 128, 0, (sockaddr*)&serverHint, serverLength);
			else
			{
				cout << "Failure to recognize input, please try again" << endl;
				continue;
			}
		}

		// Failure to recognize the message
		else
		{
			if (!inDungeon)
				cout << "You aren't in the dungeon btw" << endl;

			cout << "Failure to recognize input, please try again" << endl;
			continue;
		}

		// Receiving a message
		while (true)
		{
			// makes sure the sending went well
			if (sendOk == SOCKET_ERROR)
			{
				cout << "Sending the message didn't work : " << WSAGetLastError() << endl;
			}

			// reads in the status from the server
			ZeroMemory((char*)&serverStatus, 128);

			int bytesIn = recvfrom(serverSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&serverHint, &serverLength);
			if (bytesIn == SOCKET_ERROR)
			{
				cout << "Error receiving from client " << WSAGetLastError() << endl;
			}

			// Deal with the response
			if (serverStatus.sts == commandList.connect.cmd)
			{
				cout << serverStatus.payload << endl;

				sendOk = sendto(serverSocket, (char*)&commandList.display, 128, 0, (sockaddr*)&serverHint, serverLength);
			}

			// dealing with the display status
			else if (serverStatus.sts == commandList.display.cmd)
			{
				cout << endl << "Players in the dungeon:" << endl;
				cout << serverStatus.payload << endl;

				ZeroMemory((char*)&serverStatus, 128);

				// receive the rest of the information from the server
				int bytesIn = recvfrom(serverSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&serverHint, &serverLength);
				if (bytesIn == SOCKET_ERROR)
				{
					cout << "Error receiving from client " << WSAGetLastError() << endl;
				}

				cout << serverStatus.payload << endl;
				// Receiving the dungeon layout, not quite working
				
				ZeroMemory((char*)&serverStatus, 128);
				// receive how many lines will be received
				bytesIn = recvfrom(serverSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&serverHint, &serverLength);
				if (bytesIn == SOCKET_ERROR)
				{
					cout << "Error receiving from client " << WSAGetLastError() << endl;
				}

				// Receive the dungeon layout
				string receive(serverStatus.payload);
				int repeat = stoi(receive);

				// Print out each line received from the server
				for (int i = 0; i < repeat; i++)
				{
					ZeroMemory((char*)&serverStatus, 128);
					bytesIn = recvfrom(serverSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&serverHint, &serverLength);
					if (bytesIn == SOCKET_ERROR)
					{
						cout << "Error receiving from client " << WSAGetLastError() << endl;
					}
					cout << serverStatus.payload << endl;
				}
				break;
			}

			// leaving status
			else if (serverStatus.sts == commandList.leave.cmd)
			{
				cout << name << " has left the dungeon with " << serverStatus.payload << " piece(s) of treasure!";
				inDungeon = false;
				name = "";
				break;
			}

			// move status
			else if (serverStatus.sts == commandList.move.cmd)
			{
				bool responseFail = true;
				while (responseFail)
				{
					cout << "Please input a direction (north, south, east, west) to travel in." << endl;
					string dir;
					getline(cin, dir);

					for (int i = 0; i < dir.length(); i++)
					{
						dir[i] = tolower(dir[i]);
					}

					if (dir == "north")
						sendOk = sendto(serverSocket, (char*)&commandList.north, 128, 0, (sockaddr*)&serverHint, serverLength);

					else if (dir == "south")
						sendOk = sendto(serverSocket, (char*)&commandList.south, 128, 0, (sockaddr*)&serverHint, serverLength);

					else if (dir == "east")
						sendOk = sendto(serverSocket, (char*)&commandList.east, 128, 0, (sockaddr*)&serverHint, serverLength);

					else if (dir == "west")
						sendOk = sendto(serverSocket, (char*)&commandList.west, 128, 0, (sockaddr*)&serverHint, serverLength);

					else
						continue;

					// wait for a response
					ZeroMemory((char*)&serverStatus, 128);
					int bytesIn = recvfrom(serverSocket, (char*)&serverStatus, 128, 0, (sockaddr*)&serverHint, &serverLength);
					if (bytesIn == SOCKET_ERROR)
					{
						cout << "Error receiving from client " << WSAGetLastError() << endl;
					}

					// Checks the servers response
					if (serverStatus.payload != "Try Again")
					{
						responseFail = false;
						cout << serverStatus.payload << endl;
					}
					else
					{
						cout << "You couldn't move in the direction due to a wall." << endl;
					}
				}
				break;
			}

			// inspect status
			else if (serverStatus.sts == commandList.inspect.cmd)
			{
				cout << serverStatus.payload << endl;
				break;
			}

			// get treasure status
			else if (serverStatus.sts == commandList.getTreasure.cmd)
			{
				cout << serverStatus.payload << endl;
				break;
			}

			// treasure Amt status
			else if (serverStatus.sts == commandList.treasureAmnt.cmd)
			{
				cout << serverStatus.payload << endl;
				break;
			}
		}
	}
	

	// close the socket
	closesocket(serverSocket);

	// shutdown winsock
	WSACleanup();

	system("pause");
}
