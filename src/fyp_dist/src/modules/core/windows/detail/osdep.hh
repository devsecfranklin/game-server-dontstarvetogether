#pragma once

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>


#include "memutil/module.hh"

size_t BASE_ADDR = 0x00007FF6E3251000;
size_t SCAN_DIST = 0x4F1000;
const char* PATH_LUAJIT = "fyp_luajit.dll";

// Redirecting to alloc'd consoles is tricky
// Taken from Roger Sanders @ https://stackoverflow.com/questions/311955/redirecting-cout-to-a-console-in-windows
void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr) {
    if (bindStdIn) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "r", stdin);
    }
    if (bindStdOut) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "w", stdout);
    }
    if (bindStdErr) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "w", stderr);
    }

    // Redirect unbuffered stdin from the current standard input handle
    if (bindStdIn) {
        HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (stdHandle != INVALID_HANDLE_VALUE) {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
            if (fileDescriptor != -1) {
                FILE* file = _fdopen(fileDescriptor, "r");
                if (file != NULL) {
                    int dup2Result = _dup2(_fileno(file), _fileno(stdin));
                    if (dup2Result == 0) {
                        setvbuf(stdin, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Redirect unbuffered stdout to the current standard output handle
    if (bindStdOut) {
        HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (stdHandle != INVALID_HANDLE_VALUE) {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
            if (fileDescriptor != -1) {
                FILE* file = _fdopen(fileDescriptor, "w");
                if (file != NULL) {
                    int dup2Result = _dup2(_fileno(file), _fileno(stdout));
                    if (dup2Result == 0) {
                        setvbuf(stdout, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Redirect unbuffered stderr to the current standard error handle
    if (bindStdErr) {
        HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
        if (stdHandle != INVALID_HANDLE_VALUE) {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
            if (fileDescriptor != -1) {
                FILE* file = _fdopen(fileDescriptor, "w");
                if (file != NULL) {
                    int dup2Result = _dup2(_fileno(file), _fileno(stderr));
                    if (dup2Result == 0) {
                        setvbuf(stderr, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Clear the error state for each of the C++ standard stream objects. We need to do this, as attempts to access the
    // standard streams before they refer to a valid target will cause the iostream objects to enter an error state. In
    // versions of Visual Studio after 2005, this seems to always occur during startup regardless of whether anything
    // has been read from or written to the targets or not.
    if (bindStdIn) {
        std::wcin.clear();
        std::cin.clear();
    }
    if (bindStdOut) {
        std::wcout.clear();
        std::cout.clear();
    }
    if (bindStdErr) {
        std::wcerr.clear();
        std::cerr.clear();
    }
}

void osdep_init(bool console) {
    if (true == console) {
        auto h_console = AllocConsole();
        BindCrtHandlesToStdHandles(true, true, true);
        //freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        // remove close button on new console
        auto hwnd = GetConsoleWindow();
        auto hmenu = GetSystemMenu(hwnd, FALSE);
        DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
    }
    auto bounds = SectionBounds(L"dontstarve_steam_x64.exe", ".text");
    BASE_ADDR = std::get<0>(bounds);
    SCAN_DIST = std::get<1>(bounds);
}

extern "C" {
__declspec(dllexport) BOOL APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved) {
    if (Reason == DLL_PROCESS_ATTACH) {
        //    MessageBoxA(0, "DllMain called, good time to attach a debugger", "FYP", 0);
    }
    return TRUE;
}
}
