#include "log.h"

// Global static pointer used to ensure a single instance of the class.
Log* Log::m_pInstance = NULL;  
  
Log* Log::GetInstance()
{
   if (!m_pInstance)
   {
	   m_pInstance = new Log;
   }
   return m_pInstance;
}

Log::Log()
{
	fout.open("log.txt", ios::app);
	WriteToOutput(100, "Log::Log");
}

Log::~Log()
{
	fout.close();
}
void Log::WriteToLogFile(DWORD deltaTime, char* functionName)
{
	// ofstream fout;
	char resultStr[100];
	strcpy_s(resultStr, sizeof(resultStr), GenerateString(deltaTime, functionName));

	// Open a file to write the message to.
	// fout.open("log.txt", ios::app);
	fout << resultStr << "\n";
	// fout.close();
}

void Log::WriteToOutput(DWORD deltaTime, char* functionName)
{
	char resultStr[100];
	strcpy_s(resultStr, sizeof(resultStr), GenerateString(deltaTime, functionName));
	OutputDebugStringA((LPCSTR)resultStr);
	OutputDebugStringA("\n");
}

char* Log::GenerateString(DWORD deltaTime, char* functionName)
{
	char tempString[10];
	char resultStr[100];
	strcpy_s(resultStr, functionName);
	_itoa_s(deltaTime, tempString, 10);
	strcat_s(resultStr, tempString);
	return resultStr;
}