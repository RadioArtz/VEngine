#include "IO/FilesystemExtensions.h"

#if _WIN32
#include <Windows.h>
std::string FilesystemExtensions::GetExeLocation()
{
	char result[MAX_PATH];

	DWORD len = GetModuleFileNameA(NULL, result, MAX_PATH);
	return std::string(result, len);
}
#else if defined(__linux__)

#include <unistd.h>
#include <linux/limits.h>
#include <vector>

std::string FilesystemExtensions::GetExeLocation() 
{
	std::vector<char> buffer(PATH_MAX);

	ssize_t count = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);

	if (count <= 0)
		return {};

	buffer[count] = '\0';
	return std::string(buffer.data());
}
#endif