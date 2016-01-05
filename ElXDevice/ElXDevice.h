#ifndef ELXDEVICE_H
#define ELXDEVICE_H

#include "ElAbstractProtocol.h"
#include "ElSimpleWinComPort.h"

class ElXDevice : public ElAbstractProtocol, public ElSimpleWinComPort {
private:
	unsigned char *DataBuffer;

public:
	ElXDevice();
	ElXDevice(int BufferSize);
	ElXDevice(std::string _PortName, int _BaudRate, int BufferSize = 512);
	~ElXDevice();

	// ElAbstractProtocol
	virtual void ReInitParse() = 0;
	virtual int Parse(unsigned char *Data, int DataSize) = 0;
	virtual int CreatePacket(unsigned char FuncCode, unsigned char *Data, unsigned short DataSize, unsigned char *Buffer) = 0;

	bool WriteData(unsigned char _FunctionCode, unsigned char* _OutData, int _DataSize);
	int ReadData();

	void ResizeBuffer(unsigned buffer_siza);
};

#endif /*ELXDEVICE_H*/