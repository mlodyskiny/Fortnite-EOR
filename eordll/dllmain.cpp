#include <Windows.h>
#include <iostream>
#include <vector>
#include "xorstr.hpp"

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

DWORD WINAPI MainThread(HMODULE hModule) {
    AllocConsole();
	FILE* pFile = nullptr;

	freopen_s(&pFile, "CONOUT$", "w", stdout);
	freopen_s(&pFile, "CONIN$", "r", stdin);
	
	SetConsoleTitle(L"Using nyaai eor fast fibre edit");
	std::cout << xorstr_("Edit on release made by nyaai") << std::endl;
	std::cout << xorstr_("Supported fortnite version - 7.30") << std::endl;
	std::cout << xorstr_("Input your edit bind key code (list https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)") << std::endl;
	std::cout << xorstr_("For example if you edit with 'E' you should input 0x45") << std::endl;
	std::cout << xorstr_("(This is only for confirming the edit and the rest of the dll is internal)") << std::endl;
	
	DWORD key;
	std::cin >> std::hex >> key;

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"FortniteClient-Win64-Shipping.exe");

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x04dfe458;
	
	std::vector<unsigned int> offsets = { 0x0, 0x160, 0x0, 0x673 };
	uintptr_t editbool = FindDMAAddy(GetCurrentProcess(), dynamicPtrBaseAddr, offsets);
	
	while (true) {
		if (*(BYTE*)editbool == 1) {
			// Check if mouse button 1 is pressed
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
				while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && *(BYTE*)editbool == 1) {
					Sleep(1);
					// Wait for mouse button 1 to be released
				}
				// Check if still in edit mode
				if (*(BYTE*)editbool == 1) {
					keybd_event(key, 0, 0, 0);
					Sleep(5);
					keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
					std::cout << xorstr_("Edit confirmed") << std::endl;
				}
			}
		}
		Sleep(1);
	}
	
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0 , nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

