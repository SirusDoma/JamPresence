#include "Game.hpp"

namespace Nx
{
    Game::Game(std::string winClass, std::string moduleName) :
        winClass(winClass),
        moduleName(moduleName),
        hProcess(0),
        baseAddress(0)
    {
        GetProcessHandle();
    }

    Game::~Game()
    {
        if (hProcess)
        {
            delete hProcess;
        }
    }

    HANDLE* Game::GetProcessHandle()
    {
        baseAddress = GetBaseAddress();
        return &hProcess;
    }

    LPCVOID Game::GetBaseAddress()
    {
        // Return existing address if previous query handle still valid
        if (hProcess && baseAddress)
        {
            return baseAddress;
        }

        // Get the window handle
        HWND hwnd = FindWindow(winClass.c_str(), NULL);

        // Define variables to query modules
        DWORD procID = 0;
        HMODULE hMods[1024];
        DWORD cbNeeded;

        bool found = false;
        LPCVOID address = 0;

        // Get Process from window handle
        if (!GetWindowThreadProcessId(hwnd, &procID))
        {
            std::fprintf(stderr, "PRESENCE: Failed to get window process ID\n");
            return address;
        }

        // Open process to read memory
        hProcess = OpenProcess(
            PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION,
            FALSE,
            procID
        );

        // Check process handle
        if (!hProcess)
        {
            std::fprintf(stderr, "PRESENCE: Failed to open process\n");
            return address;
        }

        // Enumerate the list of loaded module in the process handle
        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
        {
            for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                // Resolve module filename
                TCHAR szModName[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
                    // Check if module matched with target module name
                    std::string name = szModName;
                    if (!found && name.find(moduleName) != std::string::npos)
                    {
                        // Module found, get the address
                        found = true;
                        address = hMods[i];

                        break;
                    }
                }
            }
        }

        if (!found)
        {
            std::fprintf(stderr, "PRESENCE: Failed to find module.\n");
        }

        baseAddress = address;
        return address;
    }

    unsigned int Game::ReadMemory(DWORD address, bool relative)
    {
        // Validate process handle
        if (!hProcess)
        {
            std::fprintf(stderr, "PRESENCE: Invalid process handle.\n");
            return 0;
        }

        // Calculate address
        LPCVOID target = (LPCVOID)address;
        if (relative)
        {
            target = (LPCVOID)((DWORD)baseAddress + address);
        }

        // Read data from target address process memory
        unsigned int result = 0;
        if (!ReadProcessMemory(hProcess, target, &result, sizeof(result), NULL))
        {
            std::fprintf(stderr, "PRESENCE: Failed to read process memory.\n");
        }

        return result;
    }
}