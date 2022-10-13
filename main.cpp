#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>

void GetNameByIdProcess(DWORD processID, wchar_t* szProcessName)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {

            GetModuleBaseName(hProcess, hMod, szProcessName, MAX_PATH);
        }
    }
}

BOOL TerminateProcessEx(DWORD dwProcessId, UINT uExitCode)
{
    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL  bInheritHandle = FALSE;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    if (hProcess == NULL)
        return FALSE;

    BOOL result = TerminateProcess(hProcess, uExitCode);

    CloseHandle(hProcess);

    return result;
}

int wmain(int argc, wchar_t** argv)
{
    //// Check args 
    if (argc < 2)
    {
        std::cerr << "Lack of args...\n";
        return -1;
    }


    //// Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }


    //// Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    TCHAR szProcessName[MAX_PATH] = L"<unknown>";
    //wchar_t const* killed = L"notepad.exe";
    BOOL wasFound = FALSE;
    for (i = 0; i < cProcesses; i++)
    {
        DWORD id = aProcesses[i];
        GetNameByIdProcess(id, szProcessName);
        if (wcscmp(argv[1], szProcessName) == 0)
        {
            std::wcout << szProcessName << "\n";
            TerminateProcessEx(id, 0);
            wasFound = TRUE;
            break;
        }
    }

    if (!wasFound)
    {
        std::cout << "Process hasn't been founded yet...??\n";
    }

    return 0;
}