// Mock RTC: sends the PC's Unix time to the Arduino once per second
// and prints anything the Arduino sends back (Windows only).
// Build: g++ -std=c++17 time_sync.cpp -o time_sync.exe
// Run:   .\time_sync.exe COM5
#include <ctime>
#include <chrono>
#include <thread>
#include <windows.h>
#include <iostream>
#include <string>

HANDLE openPort(const std::string& name) {
    HANDLE h = CreateFileA(("\\\\.\\" + name).c_str(), GENERIC_READ | GENERIC_WRITE,
                           0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) {
        std::cerr << "Could not open " << name << " (error " << GetLastError() << ")\n";
        return h;
    }
    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    GetCommState(h, &dcb);
    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    SetCommState(h, &dcb);

    COMMTIMEOUTS t = {0};
    t.ReadIntervalTimeout = MAXDWORD;   // reads return immediately
    t.WriteTotalTimeoutConstant = 50;
    SetCommTimeouts(h, &t);
    return h;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { std::cerr << "Usage: time_sync.exe COMx\n"; return 1; }
    HANDLE h = openPort(argv[1]);
    if (h == INVALID_HANDLE_VALUE) return 1;

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Uno resets on open
    std::cout << "Syncing time to " << argv[1] << " (Ctrl+C to stop)\n";

    while (true) {
        std::string msg = "T" + std::to_string((long long)std::time(nullptr)) + "\n";
        DWORD w;
        WriteFile(h, msg.c_str(), (DWORD)msg.size(), &w, nullptr);

        char buf[256];
        DWORD n = 0;
        if (ReadFile(h, buf, sizeof(buf) - 1, &n, nullptr) && n > 0) {
            buf[n] = 0;
            std::cout << buf << std::flush;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
