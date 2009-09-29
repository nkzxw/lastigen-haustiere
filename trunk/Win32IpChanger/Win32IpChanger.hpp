#ifndef WIN32IPCHANGER_HPP_
#define WIN32IPCHANGER_HPP_

#include <string>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "AbstractIpChanger.hpp"

class Win32IpChanger : public AbstractIpChanger
{
public:
	virtual bool setStatic(const std::string& nicName, const std::string& ipAddress, const std::string& subnetMask, const std::string& defaultGateway)
	{
		//TODO: esto está hardcodeado ... mmm...
		std::string command = "netsh interface ip set address \"" + nicName + "\" static " + ipAddress + " " + subnetMask + " " + defaultGateway + " 1";
		return executeNetshCommand(command);
	}

	virtual bool setDynamic(const std::string& nicName)
	{
		//TODO: esto está hardcodeado ... mmm...
		std::string command = "netsh interface ip set address \"" + nicName + "\" dhcp";
		return executeNetshCommand(command);
	}

	virtual std::string getMessage() const
	{
		return this->message_;
	}

protected:

	virtual bool executeNetshCommand(const std::string& command)
	{
		message_ = "";
		executeCommand(command, message_);

		//TODO: esto está hardcodeado ... mmm...
		return ( message_.substr(0, 3) == "Ok." );
	}

	//TODO: deberia ir en una clase genérica
	static void executeCommand(const std::string& command, std::string& returnMessage) //virtual 
	{

		SECURITY_ATTRIBUTES secattr; 
		ZeroMemory(&secattr, sizeof(secattr));
		secattr.nLength = sizeof(secattr);
		secattr.bInheritHandle = TRUE;

		HANDLE readPipe, writePipe;

		//Create pipes to write and read data

		CreatePipe(&readPipe, &writePipe, &secattr, 0);
		//

		STARTUPINFO sInfo; 
		ZeroMemory(&sInfo, sizeof(sInfo));
		PROCESS_INFORMATION pInfo; 
		ZeroMemory(&pInfo, sizeof(pInfo));
		sInfo.cb = sizeof(sInfo);
		sInfo.dwFlags = STARTF_USESTDHANDLES;
		sInfo.hStdInput = NULL; 
		sInfo.hStdOutput = writePipe; 
		sInfo.hStdError = writePipe;
		
		CreateProcess(0, const_cast<char*>(command.c_str()), 0, 0, TRUE, NORMAL_PRIORITY_CLASS|CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo);
		CloseHandle(writePipe);

		//now read the output pipe here.

		char buf[100];
		unsigned long reDword; 
		
		//std::string output;

		BOOL res;
		do
		{
			res = ::ReadFile(readPipe, buf, 100, &reDword, 0);

			if (res != 0)
			{
				std::string temp(buf);
				returnMessage += temp.substr(0, reDword);
			}

		} while(res != 0);
		


		// Close process and thread handles. 
		CloseHandle( pInfo.hProcess );
		CloseHandle( pInfo.hThread );

		//return output;
	}


};


#endif //WIN32IPCHANGER_HPP_