#include "Commands.h"

Commands::Commands()
{
	// Connect Command
	connect.cmd = 'c';
	ZeroMemory(connect.payload, 127);

	// Display Command
	display.cmd = 'd';
	ZeroMemory(display.payload, 127);

	// Leave Command
	leave.cmd = 'l';
	ZeroMemory(leave.payload, 127);
	
	// Move Command
	move.cmd = 'm';
	ZeroMemory(move.payload, 127);

	// Insepct Command
	inspect.cmd = 'i';
	ZeroMemory(inspect.payload, 127);

	// Get Treasure Command
	getTreasure.cmd = 'g';
	ZeroMemory(getTreasure.payload, 127);

	// Treasure Amount Command
	treasureAmnt.cmd = 'a';
	ZeroMemory(treasureAmnt.payload, 127);

	// Test Connection Command
	testConnect.cmd = 't';
	ZeroMemory(testConnect.payload, 127);
	strcpy_s(testConnect.payload, "Are we connected?");

	// Direction commands
	north.cmd = 'n';
	ZeroMemory(north.payload, 127);

	south.cmd = 's';
	ZeroMemory(south.payload, 127);

	east.cmd = 'e';
	ZeroMemory(east.payload, 127);

	west.cmd = 'w';
	ZeroMemory(west.payload, 127);
}


Commands::~Commands()
{
}
