#ifndef ELCHIPDEVICE
#define ELCHIPDEVICE

#include "ElXDevice.h"

#include <map>

class ElChipDevice : public ElXDevice
{
private:
	unsigned flags_;

	unsigned char func_code_;
	unsigned char *cmd_data_;
	int cmd_data_cnt_;
	int cmd_data_siza_;

	std::map<int, int> packet_siza_list_;

public:
	ElChipDevice();
	ElChipDevice(int BufferSize);
	ElChipDevice(std::string _PortName, int _BaudRate, int BufferSize = 512);
	~ElChipDevice();

	// ElAbstractProtocol
	virtual void ReInitParse();
	virtual int Parse(unsigned char *Data, int DataSize);
	virtual int CreatePacket(unsigned char FuncCode, unsigned char *Data, unsigned short DataSize, unsigned char *Buffer);

	void SetCmdDataSize(int FuncCode, int DataSiza);

private:
	int GetCmdDataSize(int FuncCode);
};

#endif // ELCHIPDEVICE
