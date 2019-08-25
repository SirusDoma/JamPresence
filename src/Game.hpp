#ifndef PRESENCE_PROCESS_HPP
#define PRESENCE_PROCESS_HPP

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <psapi.h>
#include <string>

namespace Nx
{
    class Game
    {
    private:
        std::string winClass;
        std::string moduleName;

        HANDLE hProcess;
        LPCVOID baseAddress;

    public:
        Game(std::string winClass, std::string moduleName);
        virtual ~Game();

        LPCVOID GetBaseAddress();
        HANDLE *GetProcessHandle();

        unsigned int ReadMemory(DWORD address, bool relative = true);
    };
}

#endif
