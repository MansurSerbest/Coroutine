#include <iostream>
#include "Coroutine.h"
#include <Windows.h>

int main()
{
	//Define Console Text Colors
	#define COLOR_WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	#define COLOR_RED FOREGROUND_RED | FOREGROUND_INTENSITY 
	#define COLOR_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY 
	#define COLOR_YELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
	std::cout << "Frame0" << "\n==========" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_GREEN);
	std::cout << "Initialize Coroutine" << "\n\n";

	Coroutine<float, float> timer{ [](Caller<float>& c, float maxTimer)
	{
		unsigned int frame{1};
		float currTime{};

		while (currTime < maxTimer)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
			std::cout << "Frame" << frame++ << "\n==========" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
			std::cout << "\tExecute Timer" << std::endl;
			c(++currTime);
		}

	}, 5.0f };

	while (timer.ResumeBool())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_GREEN);
		std::cout << "Current Time: " << timer.GetValue() << "s\n\n";
	}

	std::cin.get();
	return 0;
}



