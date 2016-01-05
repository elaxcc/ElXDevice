#ifndef ELMCDEVICE
#define  ELMCDEVICE

#include "ElXDevice.h"
#include "ElSimpleWinComPort.h"

// Parse flag bits:
#define PARSE_FLAG_BC			1	// Byte counter flag
#define PARSE_FLAG_BC_L			32	// Byte counter flag, low part
#define PARSE_FLAG_BC_H			64	// Byte counter flag, high part
#define PARSE_FLAG_FC			2	// Function code flag
#define PARSE_FLAG_GD			4	// Got data flag
#define PARSE_FLAG_CRCL			8	// Got CRC16 low byte
#define PARSE_FALG_CRCH			16	// Got CRC16 high byte
#define PARSE_FLAG_IS_PARSED	128 // Packet is parsed

// Errors
#define PARSE_ERROR_BC			-1	// Wrong byte counter
#define PARSE_ERROR_FC			-2	// Wring function code
#define PARSE_ERROR_CRC			-3	// Wrong CRC
#define PARSE_ERROR_PP			-4	// Error packet process 
#define PARSE_ERROR_DS			-5	// Calibration DS3231 error

class ElMcDevice : public ElXDevice
{
private:
	unsigned char	ParseFlag;			// Parse flag
	unsigned char	ParseError;			// Error byte
	unsigned char	ParseBCLow;			// Byte counter, low part
	unsigned char	ParseBCHigh;		// Byte counter, high part
	unsigned short  ParseBC;			// Byte counter
	unsigned char	ParseFC;			// Function code
	unsigned short	ParseCRCCalc;		// Calculated CRC16
	unsigned short	ParseCRCInCm;		// Incoming CRC16
	unsigned short	ParseBCBuf;			// Copy of the Byte Counter. Used for indicate end of data field
	unsigned char	*ParseBuf;			// Buffer for incoming data
	unsigned short	ParsePtr;			// Pointer on the current byte in Data Buffer (ParseBuf)

public:
	ElMcDevice();
	ElMcDevice(int BufferSize);
	ElMcDevice(std::string _PortName, int _BaudRate, int BufferSize = 512);
	~ElMcDevice();

	// ElAbstractProtocol
	virtual void ReInitParse();
	virtual int Parse(unsigned char *Data, int DataSize);
	virtual int CreatePacket(unsigned char FuncCode, unsigned char *Data, unsigned short DataSize, unsigned char *Buffer);

	static int CalcCRC16(unsigned char *Data, unsigned short DataSize, unsigned short CurrentValCRC16);
};

#endif // ELMCDEVICE