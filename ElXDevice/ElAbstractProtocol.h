#ifndef ELABSTRACTPROTOCOL_H
#define ELABSTRACTPROTOCOL_H

#include "ElProtoProcessPacket.h"

class ElAbstractProtocol {
private:
	ElProtoPacketProcessList FunctionsList;

public:
	virtual ~ElAbstractProtocol(){};
	virtual void ReInitParse() = 0;
	virtual int Parse(unsigned char *Data, int DataSize) = 0;
	virtual int CreatePacket(unsigned char FuncCode, unsigned char *Data, unsigned short DataSize, unsigned char *Buffer) = 0;

	void AddFunction(unsigned int func_code, const ElProtoPacketProcessItem::CallBack &func)
	{
		FunctionsList.Add(func_code, func);
	}

	int ProcessPacket(unsigned int FuncCode, unsigned char *Data, int DataSize)
	{
		int err = FunctionsList.exec(FuncCode, Data, DataSize);

		ReInitParse();
		return err;
	}
};

#endif /*ELABSTRACTPROTOCOL_H*/