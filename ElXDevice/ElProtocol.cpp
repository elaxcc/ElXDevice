#include "stdafx.h"

#include "ElProtocol.h"

ElProtocol::ElProtocol() {
	// Initialization of the Parse variables
	ParseBuf			= NULL;
	ParseFlag			= 0;
	ParseError			= 0;
	ParseBCHigh			= 0;
	ParseBCLow			= 0;
	ParseBC				= 0;
	ParseFC				= 0;
	ParseCRCCalc		= 0xFFFF;
	ParseCRCInCm		= 0;
	ParseBCBuf			= 0;
	ParsePtr			= 0;
}

ElProtocol::~ElProtocol() {
	if(ParseBuf != NULL)
		delete [] ParseBuf;
}

void ElProtocol::AddFunction(unsigned int func_code, const ElProtoPacketProcessItem::CallBack &func) {
	FunctionsList.Add(func_code, func);
}

void ElProtocol::ReInitParse() {
	// Initialization of the Parse variables
	ParseBuf		= NULL;
	ParseFlag		= 0;
	ParseError		= 0;
	ParseBCHigh		= 0;
	ParseBCLow		= 0;
	ParseBC			= 0;
	ParseFC			= 0;
	ParseCRCCalc	= 0xFFFF;
	ParseCRCInCm	= 0;
	ParseBCBuf		= 0;
	ParsePtr		= 0;
}

int ElProtocol::Parse(unsigned char *Data, int DataSize) {

	int err = 0;

	for(int i = 0; i < DataSize; i++)
	{
		// Did we get low part of Byte Counter?
		if(!(ParseFlag & PARSE_FLAG_BC_L))
		{
			// Didn't get low part Byte Counter, it is beginning of the packet

			// Save Byte Counter
			ParseBCLow = Data[i];
			// Set PARSE_FLAG_BC flag
			ParseFlag = ParseFlag | PARSE_FLAG_BC_L;
		}
		else 
		{
			// Got low part of Byte counter

			// Did we get high part of byte counter?
			if(!(ParseFlag & PARSE_FLAG_BC_H))
			{	
				// Didn't get high part Byte Counter, it is beginning of the packet

				// Save Byte Counter
				ParseBCHigh = Data[i];
				ParseBC = (unsigned short) ParseBCHigh;
				ParseBC = (ParseBC << 8) | ((unsigned short) ParseBCLow);

				if(ParseBC == 0x00)
				{
					ReInitParse();
					return PARSE_ERROR_BC;
				}				

				// Set PARSE_FLAG_BC flag
				ParseFlag = ParseFlag | PARSE_FLAG_BC_H;
				ParseFlag = ParseFlag | PARSE_FLAG_BC;
				// Save Copy Byte Counter
				ParseBCBuf = ParseBC - 3;
			}
			else
			{
				// Got high part of Byte Counter

				// Got Byte Counter

				// For next bytes except CRCL and CRCH we must calculate CRC16
				if((ParseFlag & PARSE_FLAG_BC) & (!(ParseFlag & PARSE_FLAG_GD)))
				{
					ParseCRCCalc = CalcCRC16(&Data[i], 1, ParseCRCCalc);
				}

				// Did we get Function Code?
				if(!(ParseFlag & PARSE_FLAG_FC))
				{			
					// Didn't get Function Code

					// Save Function Code
					ParseFC = Data[i];
					// Set PARSE_FLAG_FC flag
					ParseFlag = ParseFlag | PARSE_FLAG_FC;

					if(ParseBC > 3)
					{
						// Allocate memory for data
						ParseBuf = (unsigned char*) malloc(((int) ParseBC) - 3);
					}
					else
					{
						// We don't have data
						ParseFlag = ParseFlag | PARSE_FLAG_GD;
					}
				}
				else
				{
					// Got Function Code

					// Did we get all data?
					if(!(ParseFlag & PARSE_FLAG_GD))
					{					
						// Didn't get all data

						// Save data
						ParseBuf[ParsePtr] = Data[i];
						++ParsePtr;

						// Did we get last byte? If we get last byte then ParsePtr equal to ParseBCBuf
						if(ParsePtr == ParseBCBuf)
						{
							// We got last byte

							// Set PARSE_FLAG_GD flag
							ParseFlag = ParseFlag | PARSE_FLAG_GD;
						}
					}
					else
					{
						// Got all data

						// Did we get low part of the CRC16?
						if(!(ParseFlag & PARSE_FLAG_CRCL))
						{
							// Didn't get low part of the CRC16

							// Save low part of the CRC16
							ParseCRCInCm = (unsigned short) Data[i];
							// Set PARSE_FLAG_CRCL flag
							ParseFlag = ParseFlag | PARSE_FLAG_CRCL;
						}
						else
						{
							// Got low part of the CRC16

							// Did we get high part of the CRC16
							if(!(ParseFlag & PARSE_FALG_CRCH))
							{
								// Save high part of the CRC16
								ParseCRCInCm = (ParseCRCInCm) | (((unsigned short) Data[i])<<8);

								// Compare incoming CRC16 with calculated CRC16
								if(ParseCRCInCm == ParseCRCCalc)
								{								
									// CRC16 is right, packet is parsed

									// Set flag PARSE_FLAG_IS_PARSED
									ParseFlag = ParseFlag | PARSE_FLAG_IS_PARSED;

									// Process packet
									err = ProcessPacket();
								}
								else 
								{
									// Reinitialization of the Parse variables
									ReInitParse();


									return PARSE_ERROR_CRC;
								}
							}
						}
					}
				}
			}
		}
	}

	return err;
}

int ElProtocol::ProcessPacket() {
	int err = FunctionsList.exec(ParseFC, ParseBuf, ParseBC - 3);

	ReInitParse();
	return err;
}

int ElProtocol::CreatePacket(unsigned char FuncCode, unsigned char *Data, unsigned short DataSize, unsigned char *Buffer) {

	// Byte counter
	Buffer[0] = (unsigned char) (DataSize + 3); // low part of Byte counter
	Buffer[1] = (unsigned char) ((DataSize + 3) >> 8); // High part of the Byte counter
	// Function code
	Buffer[2] = FuncCode;
	// Data
	if(DataSize > 0)
	{
		memcpy(&(Buffer[3]), Data, DataSize);
	}		
	// CRC
	unsigned short CRC = CalcCRC16(&Buffer[2], DataSize + 1, 0xFFFF);
	Buffer[DataSize + 3] = (unsigned char) CRC; // Low part of the CRC
	Buffer[DataSize + 4] = (unsigned char) (CRC >> 8); // High part of the CRC

	return DataSize + 5;
}

int ElProtocol::CalcCRC16(unsigned char *Data, unsigned short DataSize, unsigned short CurrentValCRC16) {
	for(int i = 0; i < DataSize; i++)
	{
		CurrentValCRC16 = CurrentValCRC16 ^ Data[i];

		for(int j = 0; j < 8; j++)
		{
			if(CurrentValCRC16 & 0x0001) CurrentValCRC16 = (CurrentValCRC16 >> 1) ^ 0xA001;
			else CurrentValCRC16 = CurrentValCRC16 >> 1;
		}
	}	

	return CurrentValCRC16;
}


