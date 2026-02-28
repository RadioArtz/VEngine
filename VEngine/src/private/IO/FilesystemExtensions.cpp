#include "IO/FilesystemExtensions.h"

#if _WIN32
#include <Windows.h>
std::string FilesystemExtensions::GetExeLocation()
{
	char result[MAX_PATH];

	DWORD len = GetModuleFileNameA(NULL, result, MAX_PATH);
	return std::string(result, len);
}
#else
std::string FilesystemExtensions::GetExeLocation() { return ""; }
#endif