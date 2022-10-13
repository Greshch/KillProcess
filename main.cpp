#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = L"<unknown>";
    
    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Print the process name and identifier.
    _tprintf(TEXT("%40s  (PID: %u)\n"), szProcessName, processID);

    CloseHandle(hProcess);
}

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

int main(void)
{
    //// Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }


    //// Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    //// Print the name and process identifier for each process.

    /*for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcessNameAndID(aProcesses[i]);
        }
    }*/

    TCHAR szProcessName[MAX_PATH] = L"<unknown>";
    DWORD killed = 12868;
    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] == killed)
        {
            std::cout << "$$$$\n";
            GetNameByIdProcess(aProcesses[i], szProcessName);
            std::wcout << szProcessName;
            //TerminateProcessEx(killed, 0);
            break;
        }
    }

    return 0;
}