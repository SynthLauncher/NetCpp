#ifndef NETCPP_WINDOWS_DESCRIPTOR_H
#define NETCPP_WINDOWS_DESCRIPTOR_H

#if defined(_WIN32)

/*
    The UNICODE macro is a preprocessor directive in Windows programming that enables the use of wide-character strings (wchar_t) and corresponding Unicode APIs, instead of the standard ASCII-based or multi-byte character set (MBCS) versions of functions.
*/
#undef UNICODE

/*
    WIN32_LEAN_AND_MEAN is a preprocessor directive used to reduce the size and compilation time of Windows headers by excluding rarely used components.
*/
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

int createSocketServer(const std::string address, const std::string port);

#endif // (_WIN32)

#endif // !NETCPP_WINDOWS_DESCRIPTOR_H