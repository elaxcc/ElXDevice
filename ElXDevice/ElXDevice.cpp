#include "stdafx.h"

#include "ElXDevice.h"

ElXDevice::ElXDevice() : ElSimpleWinComPort()
{
	DataBuffer = new unsigned char [512];
}

ElXDevice::ElXDevice(int BufferSize) : ElSimpleWinComPort()
{
	DataBuffer = new unsigned char [BufferSize];
}

ElXDevice::ElXDevice(std::string _PortName, int _BaudRate, int BufferSize) : ElSimpleWinComPort(_PortName, _BaudRate)
{
	DataBuffer = new unsigned char [BufferSize];
}

ElXDevice::~ElXDevice()
{
	delete [] DataBuffer;
}

bool ElXDevice::WriteData(unsigned char _FunctionCode, unsigned char* _OutData, int _DataSize)
{
	int ByteForSending = CreatePacket(_FunctionCode, _OutData, _DataSize, DataBuffer);

	return write(DataBuffer, ByteForSending);
}

int ElXDevice::ReadData()
{
	int err = 0;

	int IncomeBytesCnt = read(DataBuffer);
	if (IncomeBytesCnt > 0)
	{
		err = Parse(DataBuffer, IncomeBytesCnt);
		if (err < 0)
		{
			ReInitParse();
			return err;
		}
	}

	return 0;
}

