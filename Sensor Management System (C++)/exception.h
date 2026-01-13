#pragma once
#include <string>

using namespace std;

class CException 
{
private:
	string message;

public:
	CException(const char* text):
		message(text)
	{}

	const char* GetMessage()
	{
		return message.c_str();
	}
};
