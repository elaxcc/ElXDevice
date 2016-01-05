#include "stdafx.h"

#include "ElChipDevice.h"

#define FLAG_GOT_FUNC_CODE 1
#define FLAG_GOT_ALL_DATA 2

ElChipDevice::ElChipDevice() : ElXDevice()
{
	ResizeBuffer(512);
	ReInitParse();
}

ElChipDevice::ElChipDevice(int BufferSize) : ElXDevice()
{
	ResizeBuffer(512);
	ReInitParse();
}

ElChipDevice::ElChipDevice(std::string _PortName, int _BaudRate, int BufferSize) : ElXDevice(_PortName, _BaudRate)
{
	ResizeBuffer(512);
	ReInitParse();
}

ElChipDevice::~ElChipDevice()
{
}

void ElChipDevice::ReInitParse()
{
	flags_ = 0;

	func_code_ = 0;
	if (cmd_data_ != NULL)
	{
		delete[] cmd_data_;
		cmd_data_ = NULL;
	}
	cmd_data_cnt_ = 0;
	cmd_data_siza_ = 0;
}

int ElChipDevice::Parse(unsigned char *Data, int DataSize)
{
	for (int i = 0; i < DataSize; ++i)
	{
		if (!(flags_ & FLAG_GOT_FUNC_CODE))
		{
			flags_ |= FLAG_GOT_FUNC_CODE;

			func_code_ = Data[i];
			cmd_data_siza_ = GetCmdDataSize(func_code_);
			if (cmd_data_siza_ < 0)
			{
				return -1;
			}
			else if (cmd_data_siza_ > 0)
			{
				cmd_data_ = new unsigned char [cmd_data_siza_];
			}
		}
		else
		{
			if (!(flags_ & FLAG_GOT_ALL_DATA))
			{
				cmd_data_[cmd_data_cnt_] = Data[i];
				cmd_data_cnt_++;
				if (cmd_data_cnt_ == cmd_data_siza_)
				{
					flags_ |= FLAG_GOT_ALL_DATA;

					ProcessPacket(func_code_, cmd_data_, cmd_data_siza_);
					ReInitParse();
				}
			}
		}
	}

	return 0;
}

int ElChipDevice::CreatePacket(unsigned char FuncCode, unsigned char *Data, unsigned short DataSize, unsigned char *Buffer)
{
	Buffer[0] = FuncCode;
	memcpy(&(Buffer[1]), Data, DataSize);

	return DataSize + 1;
}

void ElChipDevice::SetCmdDataSize(int FuncCode, int DataSiza)
{
	std::map<int, int>::iterator iter = packet_siza_list_.find(FuncCode);
	if (iter != packet_siza_list_.end())
	{
		iter->second = DataSiza;
	}
	else
	{
		packet_siza_list_.insert(std::pair<int, int>(FuncCode, DataSiza));
	}
}

int ElChipDevice::GetCmdDataSize(int FuncCode)
{
	std::map<int, int>::iterator iter = packet_siza_list_.find(FuncCode);
	if (iter != packet_siza_list_.end())
	{
		return iter->second;
	}
	return -1;
}
