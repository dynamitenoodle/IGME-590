// TestCPPLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream> 
#include "TestCPPLibrary.h"

using namespace std;
extern "C" {
	float __declspec(dllexport) TestMultiply(float a, float b)
	{
		return a * b;
	}

	float __declspec(dllexport) TestDivide(float a, float b)
	{
		if (b == 0) {
			return 0;
			//throw invalid_argument("b cannot be zero!");
		}

		return a / b;
	}

	char TestString()
	{
		char* str = new char[100];

		str = (char*)"Hello World";
		return str;
	}
}