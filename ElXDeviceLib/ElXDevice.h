#ifndef ELXDEVICE_H
#define ELXDEVICE_H

#include "ElProtocol.h"
#include "ElSimpleWinComPort.h"

class ElXDevice : public ElProtocol, public ElSimpleWinComPort {
private:
	unsigned char *DataBuffer;

public:
	ElXDevice();
	ElXDevice(int BufferSize);
	ElXDevice(std::string _PortName, int _BaudRate, int BufferSize = 512);
	~ElXDevice();
	bool WriteData(unsigned char _FunctionCode, unsigned char* _OutData, int _DataSize);
	int ReadData();
};

#endif /*ELXDEVICE_H*/