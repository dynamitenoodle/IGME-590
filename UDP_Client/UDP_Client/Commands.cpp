#include "Commands.h"

Commands::Commands()
{
	// Connect Command
	connect.cmd = 'c';
	ZeroMemory(connect.payload, 127);
	strcpy_s(connect.payload, "Coehl");


}


Commands::~Commands()
{
}
