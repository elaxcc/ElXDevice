#include "stdafx.h"

#include "ElSimpleWinComPort.h"

ElSimpleWinComPort::ElSimpleWinComPort() {
	PortName	= "COM1";
	BuadRate	= 9600;
	ByteSize	= 8;
	Parity		= 0;
	StopBits	= 0;
	Port		= (HANDLE) -1;
	opened = false;
}

ElSimpleWinComPort::ElSimpleWinComPort(std::string _PortName, int _BaudRate) {
	PortName	= _PortName;
	BuadRate	= _BaudRate;
	ByteSize	= 8;
	Parity		= 0;
	StopBits	= 0;
	Port		= (HANDLE) -1;
	opened = false;
}

ElSimpleWinComPort::~ElSimpleWinComPort() {
	if(Port >= 0)
	{
		CloseHandle(Port);
	}
}

void ElSimpleWinComPort::SetSetting(std::string _PortName, int _BaudRate, int _ByteSize, int _Parity, int _StopBits) {
	PortName	= _PortName;
	BuadRate	= _BaudRate;
	ByteSize	= _ByteSize;
	Parity		= _Parity;
	StopBits	= _StopBits;
}

int ElSimpleWinComPort::open() {
	WCHAR Nname[80];
	MultiByteToWideChar(CP_ACP, 0, PortName.c_str(), PortName.length() + 1, (LPWSTR)Nname, sizeof(Nname)/sizeof(Nname[0]));

	Port = CreateFile(Nname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(Port == INVALID_HANDLE_VALUE) {
		Port = 0;
		return -1; // CreateFile failed
	}

	// Port initialization
	Dcb.DCBlength = sizeof(DCB);
	// Read configuration from port
	BOOL tmp_bool = GetCommState(Port, &Dcb);
	if(!tmp_bool) {
		CloseHandle(Port);
		Port = 0;
		return -2; // GetCommState failed
	}

	Dcb.BaudRate		= BuadRate;
	Dcb.fBinary			= true;
	Dcb.fOutxCtsFlow	= false;
	Dcb.fOutxDsrFlow	= false;
	Dcb.fDtrControl		= DTR_CONTROL_DISABLE;
	Dcb.fRtsControl		= RTS_CONTROL_DISABLE;
	Dcb.fDsrSensitivity	= false;
	Dcb.fNull			= false;
	Dcb.fAbortOnError	= false;
	Dcb.ByteSize		= ByteSize;
	Dcb.Parity			= Parity;
	Dcb.StopBits		= StopBits;

	// Load port configuration
	if(!SetCommState(Port, &Dcb)) {
		CloseHandle(Port);
		Port = 0;
		return -3; // SetCommState failed
	}

	// Initialize timeout structure
	ComTimeout.ReadIntervalTimeout				= 10;
	ComTimeout.ReadTotalTimeoutConstant			= 0;
	ComTimeout.ReadTotalTimeoutMultiplier		= 0;
	ComTimeout.WriteTotalTimeoutConstant		= 0;
	ComTimeout.WriteTotalTimeoutMultiplier		= 0;

	// Load timeout structure inti port
	if(!SetCommTimeouts(Port, &ComTimeout)) {
		CloseHandle(Port);
		Port = 0;
		return -4; // SetCommTimeouts failed
	}

	// Set queue size
	SetupComm(Port, 2000, 2000);

	// Erase receiving port buffer
	PurgeComm(Port, PURGE_RXCLEAR);

	SetCommMask(Port, EV_RXCHAR);

	opened = true;

	return 0;
}

void ElSimpleWinComPort::close() {
	if(Port >= 0)
	{
		CloseHandle(Port);
		Port = (HANDLE) -1;
		opened = false;
	}
}

bool ElSimpleWinComPort::write(unsigned char* Data, int Size) {
	DWORD temp;

	return WriteFile(Port, Data, Size, &temp, NULL);
}

int ElSimpleWinComPort::read(unsigned char* Data) {
	Btr = 0;
	ClearCommError(Port, &temp, &ComState);
	Btr = (DWORD) ComState.cbInQue;
	if(Btr)
	{
		ReadFile(Port, Data, Btr, &temp, NULL);
	}

	return Btr;
}
