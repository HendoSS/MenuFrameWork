#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <streambuf>
#include <fstream>
#include <Psapi.h> // for module size stuff
#include <fcntl.h>
#include <stdio.h>
#include <io.h>

#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif
enum colour
{
	DARKBLUE = 1,
	DARKGREEN,
	DARKTEAL,
	DARKRED,
	DARKPINK,
	DARKYELLOW,
	GRAY,
	DARKGRAY,
	BLUE,
	GREEN,
	TEAL,
	RED,
	PINK,
	YELLOW,
	WHITE
};

namespace Tools
{


	void GetModuleAddressAndSize(const char* dllName, DWORD* base, DWORD* size, HMODULE ModuleHandle = NULL) //zoomgod http://www.unknowncheats.me/forum/633534-post5.html
	{

		MODULEINFO module;

		ZeroMemory(&module, sizeof(module));

		*base = 0;
		*size = 0;

		if (!ModuleHandle)
		{
			ModuleHandle = GetModuleHandleA(dllName);
		}
		if (GetModuleInformation(GetCurrentProcess(), ModuleHandle, &module, sizeof(module)))
		{
			*base = (DWORD)module.lpBaseOfDll;
			*size = (DWORD)module.SizeOfImage;
		}

	}

	void SetConsoleColor(colour PreColor, colour PostColor, char* text)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PreColor);
		std::cout << text << "\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PostColor);
	}

	void CreateConsole()
	{
		int hConHandle = 0;
		HANDLE lStdHandle = 0;
		FILE *fp = 0;

		AllocConsole();

		lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);

		fp = _fdopen(hConHandle, "w");

		*stdout = *fp;

		setvbuf(stdout, NULL, _IONBF, 0);
	}


	int GetRand(int min, int max)
	{
		//we do srand((unsigned)time(0)) in main
		int range = max - min;
		int random = min + int(range*rand()) / (RAND_MAX + 1);
		return random;
	}

	void* DetourVTable(void **VTable, int Function, void *Detour)
	{
		if (!VTable)
			return NULL;

		if (!Detour)
			return NULL;

		void *ret = VTable[Function];

		DWORD old;
		VirtualProtect(&(VTable[Function]), sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
		VTable[Function] = Detour;
		VirtualProtect(&(VTable[Function]), sizeof(void*), old, &old);

		return ret;
	}

	void tprintf(const char* format) // base function
	{
		std::cout << format;
	}

	template<typename T, typename... Targs>
	void tprintf(const char* format, T value, Targs... Fargs)
	{
		for (; *format != '\0'; format++) {
			if (*format == '%' && (*(format + 1) == 'V' || *(format + 1) == 'v')) {
				std::cout << value;
				tprintf(format + 2, Fargs...); // recursive call
				return;
			}
			std::cout << *format;
		}
	}
}

#define POINTERCHK( pointer ) ( pointer  && pointer !=0  && HIWORD( pointer ) )

#endif

