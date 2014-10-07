#ifndef	ELSIMPLEWINCOMPORT_H
#define ELSIMPLEWINCOMPORT_H
#include <iostream>
#include <Windows.h>
#include <string>

class ElSimpleWinComPort {
private:
	std::string PortName;
	int BuadRate;
	int ByteSize;
	int Parity;
	int StopBits;
	bool opened;

	// Port configuration
	COMSTAT			ComState;
	DWORD			Btr, mask, temp;
	DWORD			Signal;
	HANDLE			Port;
	DCB				Dcb;
	COMMTIMEOUTS	ComTimeout;

public:
	ElSimpleWinComPort();
	ElSimpleWinComPort(std::string _PortName, int _BaudRate);
	~ElSimpleWinComPort();
	void SetSetting(std::string _PortName, int _BaudRate, int _ByteSize, int _Parity, int _StopBits);
	int open();
	void close();
	bool write(unsigned char* Data, int Size);
	int read(unsigned char* Data);
	bool is_opened() { return opened; }
};

#endif /*ELSIMPLEWINCOMPORT_H*/