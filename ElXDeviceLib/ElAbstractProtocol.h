#ifndef ELABSTRACTPROTOCOL_H
#define ELABSTRACTPROTOCOL_H

class ElAbstractProtocol {
public:
	virtual ~ElAbstractProtocol(){};
	virtual int Parse(unsigned char *Data, int DataSize) = 0;
	virtual int ProcessPacket()	= 0;
	virtual int CreatePacket(unsigned char FuncCode, unsigned char *Data, unsigned short DataSize, unsigned char *Buffer) = 0;
};

#endif /*ELABSTRACTPROTOCOL_H*/