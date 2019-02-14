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
	// seeds the random
	srand(time(NULL));

	// creates the status list
	Status statusList;

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

	// sets up the IP with input
	//string myIp = "129.21.178.67";
	cout << "Please enter the IP of this machine" << endl;
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

	// converts from string to binary for address
	inet_pton(AF_INET, ip.c_str(), &serverHint.sin_addr); // address is for THIS computer, use ipconfig in command prompt
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(port); // Convert from little to big endian

	// binds the socket for the client to use
	if (bind(clientSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}

	// creates the client
	sockaddr_in client;
	int clientLength = sizeof(client);

	// Setup the Dungeon
	int sizeX = -1;
	while (sizeX < 5 || sizeX > 25)
	{
		cout << "Insert a sizeX value (Btwn 5-25)" << endl;
		string input;
		getline(cin, input);
		cout << endl;

		sizeX = stoi(input);
	}

	int sizeY = -1;
	while (sizeY < 5 || sizeY > 25)
	{
		cout << "Insert a sizeY value (Btwn 5-25)" << endl;
		string input;
		getline(cin, input);
		cout << endl;

		sizeY = stoi(input);
	}

	int maxTreasure = -1;
	while (maxTreasure < floor((sizeX * sizeY) / 10) || maxTreasure > floor((sizeX * sizeY) / 4))
	{
		cout << "Insert a maxTreasure value (Btwn " << floor((sizeX * sizeY) / 10) << " - " << floor((sizeX * sizeY) / 4) <<")" << endl;
		string input;
		getline(cin, input);
		cout << endl;

		maxTreasure = stoi(input);
	}

	Dungeon dungeon(sizeX, sizeY, maxTreasure);
	cout << "Dungeon Created" << endl;

	// Enter a loop
	while (true)
	{
		cout << endl;
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

		// Connect Command
		int sendOk;
		if (clientCommand.cmd == statusList.testConnect.sts)
		{
			// Tested connection accomplished
			sendOk = sendto(clientSocket, (char*)&statusList.testConnect, 128, 0, (sockaddr*)&client, clientLength);
			cout << "Someone has connected from " << clientIp << endl;
		}

		else if (clientCommand.cmd == statusList.connect.sts)
		{
			// Create a new player in the dungeon
			string name(clientCommand.payload);
			dungeon.AddPlayer(clientIp, name);

			// prepare the payload
			string returnMsg = "Welcome to the dungeon " + name + ".";
			strcpy_s(statusList.connect.payload, returnMsg.c_str());

			// Send the status to the client
			sendOk = sendto(clientSocket, (char*)&statusList.connect, 128, 0, (sockaddr*)&client, clientLength);
		}

		// Display Command
		else if (clientCommand.cmd == statusList.display.sts)
		{
			// Send the player names to the player requesting it
			Player* playerArray = dungeon.GetPlayerArray();
			int playerCount = dungeon.GetPlayerCount();
			string playerNames;

			for (int i = 0; i < playerCount; i++)
			{
				playerNames += playerArray[i].GetName() + "  ";
			}

			ZeroMemory((char*)&statusList.display.payload, 127);
			strcpy_s(statusList.display.payload, playerNames.c_str());
			sendOk = sendto(clientSocket, (char*)&statusList.display, 128, 0, (sockaddr*)&client, clientLength);
			if (sendOk == SOCKET_ERROR)
			{
				cout << "that didn't work! " << WSAGetLastError() << endl;
			}

			// Sends a string with the important dungeon information
			ZeroMemory((char*)&statusList.display.payload, 127);
			strcpy_s(statusList.display.payload, dungeon.DisplayValues(clientCommand.payload).c_str());
			sendOk = sendto(clientSocket, (char*)&statusList.display, 128, 0, (sockaddr*)&client, clientLength);
			if (sendOk == SOCKET_ERROR)
			{
				cout << "that didn't work! " << WSAGetLastError() << endl;
			}

			// Making the layout to send to the client, not quite working yet
			// Send how many times to repeat
			int size = sizeX;
			ZeroMemory((char*)&statusList.display.payload, 127);
			strcpy_s(statusList.display.payload, to_string(size).c_str());
			sendOk = sendto(clientSocket, (char*)&statusList.display, 128, 0, (sockaddr*)&client, clientLength);
			if (sendOk == SOCKET_ERROR)
			{
				cout << "that didn't work! " << WSAGetLastError() << endl;
			}

			// creating the dungeon layout
			string dungeonLayout;
			for (int x = sizeX; x > 0; x--)
			{
				dungeonLayout = "";
				for (int y = 0; y < sizeY; y++)
				{
					bool playerCheck = false;
					bool treasureCheck = dungeon.GetTreasure(y, x, false);
					for (int i = 0; i < dungeon.GetPlayerCount(); i++)
					{
						if (dungeon.GetPlayerArray()[i].ColCheck(y, x))
						{
							playerCheck = true;
						}
					}

					if (playerCheck)
						dungeonLayout += " [P] ";
					
					else if (treasureCheck && !playerCheck)
						dungeonLayout += " [T] ";

					else
						dungeonLayout += " [-] ";
				}
				// Send the current Layout Line to the client
				ZeroMemory((char*)&statusList.display.payload, 127);
				strcpy_s(statusList.display.payload, dungeonLayout.c_str());
				sendOk = sendto(clientSocket, (char*)&statusList.display, 128, 0, (sockaddr*)&client, clientLength);
				if (sendOk == SOCKET_ERROR)
				{
					cout << "that didn't work! " << WSAGetLastError() << endl;
				}
			}
		}

		// Leave Command
		else if (clientCommand.cmd == statusList.leave.sts)
		{
			string name(clientCommand.payload);
			int treasureAmt = dungeon.GetPlayer(name).GetTreasureAmt();

			// Send the treasure amt to the player
			strcpy_s(statusList.leave.payload, to_string(treasureAmt).c_str());
			sendOk = sendto(clientSocket, (char*)&statusList.leave, 128, 0, (sockaddr*)&client, clientLength);

			// remove the player from the dungeon
			dungeon.RemovePlayer(name);
			cout << "Removed " << name << " from the dungeon." << endl;
		}

		// Move Command
		else if (clientCommand.cmd == statusList.move.sts)
		{
			string name = clientCommand.payload;

			strcpy_s(statusList.move.payload, "Which way would you like to go?");
			sendOk = sendto(clientSocket, (char*)&statusList.move, 128, 0, (sockaddr*)&client, clientLength);
			
			// Wait for a direction
			bool directionCorrect = false;
			while (!directionCorrect)
			{
				ZeroMemory((char*)&clientCommand, 128);

				int bytesIn = recvfrom(clientSocket, (char*)&clientCommand, 128, 0, (sockaddr*)&client, &clientLength);
				if (bytesIn == SOCKET_ERROR)
				{
					cout << "Error receiving from client " << WSAGetLastError() << endl;
					continue;
				}

				// check which command we have received
				if (clientCommand.cmd == 'n' && (dungeon.GetPlayer(name).positionY < sizeY))
				{
					strcpy_s(statusList.move.payload, "Moved to the North.");
					sendOk = sendto(clientSocket, (char*)&statusList.move, 128, 0, (sockaddr*)&client, clientLength);
					directionCorrect = true;
					dungeon.MovePlayer(name, 0, 1);
					cout << name << " moved North." << endl;
				}

				else if (clientCommand.cmd == 's' && (dungeon.GetPlayer(name).positionY > 1))
				{
					strcpy_s(statusList.move.payload, "Moved to the South.");
					sendOk = sendto(clientSocket, (char*)&statusList.move, 128, 0, (sockaddr*)&client, clientLength);
					directionCorrect = true;
					dungeon.MovePlayer(name, 0, -1);
					cout << name << " moved South." << endl;
				}

				else if (clientCommand.cmd == 'e' && (dungeon.GetPlayer(name).positionX < sizeX))
				{
					strcpy_s(statusList.move.payload, "Moved to the East.");
					sendOk = sendto(clientSocket, (char*)&statusList.move, 128, 0, (sockaddr*)&client, clientLength);
					directionCorrect = true;
					dungeon.MovePlayer(name, 1, 0);
					cout << name << " moved East." << endl;
				}

				else if (clientCommand.cmd == 'w' && (dungeon.GetPlayer(name).positionX > 0))
				{
					strcpy_s(statusList.move.payload, "Moved to the West.");
					sendOk = sendto(clientSocket, (char*)&statusList.move, 128, 0, (sockaddr*)&client, clientLength);
					directionCorrect = true;
					dungeon.MovePlayer(name, -1, 0);
					cout << name << " moved West." << endl;
				}

				else
				{
					strcpy_s(statusList.move.payload, "Try Again");
					sendOk = sendto(clientSocket, (char*)&statusList.move, 128, 0, (sockaddr*)&client, clientLength);
				}
			}
		}

		// Insepct Command
		else if (clientCommand.cmd == statusList.inspect.sts)
		{
			string msg(clientCommand.payload);

			// says which ways the player can move
			msg += " can move ";
			if (dungeon.GetPlayer(clientCommand.payload).positionY < sizeY)
				msg += "North ";
			if (dungeon.GetPlayer(clientCommand.payload).positionY > 0)
				msg += "South ";
			if (dungeon.GetPlayer(clientCommand.payload).positionX < sizeX)
				msg += "East ";
			if (dungeon.GetPlayer(clientCommand.payload).positionX > 0)
				msg += "West";
			msg += ".";

			// checks to see if there is a player in the room
			for (int i = 0; i < dungeon.GetPlayerCount(); i++)
			{
				if (dungeon.GetPlayerArray()[i].GetName() != clientCommand.payload && 
					dungeon.GetPlayer(clientCommand.payload).ColCheck(dungeon.GetPlayerArray()[i].positionX, dungeon.GetPlayerArray()[i].positionY))
				{
					msg += "\nPlayer " + dungeon.GetPlayerArray()[i].GetName() + " is in the room!";
				}
			}

			// checks to see if there is treasure in the room
			if (dungeon.GetTreasure(clientCommand.payload, false))
			{
				msg += "\nThere is a treasure in the room!";
			}

			strcpy_s(statusList.inspect.payload, msg.c_str());
			sendOk = sendto(clientSocket, (char*)&statusList.inspect, 128, 0, (sockaddr*)&client, clientLength);
		}

		// Get Treasure Command
		else if (clientCommand.cmd == statusList.getTreasure.sts)
		{
			if (dungeon.GetTreasure(clientCommand.payload, true))
			{
				string msg(clientCommand.payload);
				msg += " found a treasure!";
				strcpy_s(statusList.getTreasure.payload, msg.c_str());
			}
			else
			{
				strcpy_s(statusList.getTreasure.payload, "There was no treasure in the room.");
			}

			sendOk = sendto(clientSocket, (char*)&statusList.getTreasure, 128, 0, (sockaddr*)&client, clientLength);
		}

		// Treasure Amount Command
		else if (clientCommand.cmd == statusList.treasureAmnt.sts)
		{
			string name(clientCommand.payload);
			int treasureAmt = dungeon.GetPlayer(name).GetTreasureAmt();

			string msg = name + " has " + to_string(treasureAmt) + ".";
			string mostTreasureName = name;
			for (int i = 0; i < dungeon.GetPlayerCount(); i++)
			{
				if (dungeon.GetPlayer(name).GetTreasureAmt() < dungeon.GetPlayerArray()[i].GetTreasureAmt())
					mostTreasureName = dungeon.GetPlayerArray()[i].GetName();
			}

			msg += "\n" + mostTreasureName + " has the most treasure at " + to_string(dungeon.GetPlayer(mostTreasureName).GetTreasureAmt()) + "!";

			// Send the treasure amt to the player
			strcpy_s(statusList.treasureAmnt.payload, msg.c_str());
			sendOk = sendto(clientSocket, (char*)&statusList.treasureAmnt, 128, 0, (sockaddr*)&client, clientLength);

			cout << name << " requsted to see their treasure amount, which is " << treasureAmt << "." << endl;
		}

		if (sendOk == SOCKET_ERROR)
		{
			cout << "that didn't work! " << WSAGetLastError() << endl;
		}
	}

	// close socket
	closesocket(clientSocket);

	// shutdown winsock
	WSACleanup();
}
