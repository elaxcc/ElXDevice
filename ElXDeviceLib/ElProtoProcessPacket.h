#ifndef ELPROTOPROCESSPACKET_H
#define ELPROTOPROCESSPACKET_H

#include <iostream>
#include <list>
#include <String>

class ElProtoPacketProcessItem {
public:
	typedef boost::function<int(unsigned char*, int)> CallBack;

private:
	unsigned int FunctionCode;
	CallBack Function;

public:
	ElProtoPacketProcessItem() {
		FunctionCode = 0;
		Function = NULL;
	}
	ElProtoPacketProcessItem(unsigned int func_code, const CallBack &func) {
		FunctionCode = func_code;
		Function = func;
	}
	~ElProtoPacketProcessItem() {
	}
	unsigned int GetFunctionCode() {
		return FunctionCode;
	}
	void Set(unsigned int func_code, const CallBack &func) {
		FunctionCode = func_code;
		Function = func;
	}
	int exec(unsigned char *DataBytes, int DataSize) {
		if(Function == NULL)
			return -2;
		else
			return Function(DataBytes, DataSize);
	}
};

class ElProtoPacketProcessList
{
private:
	int Counter;
	std::list<class ElProtoPacketProcessItem> ItemList;

public:
	ElProtoPacketProcessList() {
		Counter = 0;
	}
	~ElProtoPacketProcessList() {
		std::list<class ElProtoPacketProcessItem>::iterator it = ItemList.begin();

		ItemList.clear();
	}
	int GetAmount() {
		return Counter;
	}
	void Add(unsigned int func_code, const ElProtoPacketProcessItem::CallBack &func) {
		ElProtoPacketProcessItem *item = new ElProtoPacketProcessItem(func_code, func);
		ItemList.push_back(*item);
		Counter++;
	}
	int exec(unsigned int func_code, unsigned char *DataBytes, int DataSize) {
		std::list<class ElProtoPacketProcessItem>::iterator it = ItemList.begin();

		for(;it != ItemList.end(); it++)
		{
			int tmp_func_code = it->GetFunctionCode();
			if(tmp_func_code == func_code)
				break;
		}

		if(it == ItemList.end())
			return -2; // PARSE_ERROR_FC
		else
			return it->exec(DataBytes, DataSize);
	}
};

#endif /*ELPROTOPROCESSPACKET_H*/