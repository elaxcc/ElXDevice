#include "stdafx.h"

#include "ElXDevice.h"

int func_1(unsigned char* DataBytes, int DataSize)
{
	return 0;
}

class test_class
{
public:
	test_class(int val)
	{
		a = val;
	}

	int Function0(unsigned char* DataBytes, int DataSize)
	{
		int b = a;
		a++;
		return 1;
	}

private:
	int a;
};

int main(int argc, char **argv)
{
	ElXDevice device;
	test_class test_c(4);

	device.SetSetting("COM5", 57600, 8, 0, 0);
	device.AddFunction(0x01, boost::bind(&test_class::Function0, &test_c, _1, _2));
	device.AddFunction(0x02, &func_1);
	if (device.open() == 0)
	{
		std::cout << "Port is opened" << std::endl;
	}
	else
	{
		std::cout << "Can't open port" << std::endl;
	}

	while (1)
	{
		device.ReadData();
	}

	int a;
	std::cin >> a;
	return 0;
}
