#include "WindowsError.hpp"

#include <memory>
#include <sstream>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::system;

WindowsError::WindowsError(HRESULT hResult, const std::string& message) :
	coconut_tools::exceptions::RuntimeError(buildMessage(hResult, message))
{
}

WindowsError::WindowsError(DWORD errorCode, const std::string& message) :
	coconut_tools::exceptions::RuntimeError(buildMessage(errorCode, message))
{
}

std::string WindowsError::buildMessage(DWORD errorCode, const std::string& message) {
	std::ostringstream oss;
	oss << message << " Error code: " << errorCode << " (0x" << std::hex << errorCode << ").";

	LPSTR messageBuffer;
	const auto size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer),
		0,
		nullptr
		);
	std::unique_ptr<LPSTR, decltype(&::LocalFree)> messageBufferDeleter(&messageBuffer, ::LocalFree);

	oss << " System message: " << messageBuffer;

	return oss.str();
}

std::string WindowsError::buildMessage(HRESULT hResult, const std::string& message) {
	std::ostringstream oss;
	oss << message << " hResult: " << hResult << " (0x" << std::hex << hResult << ").";
	return oss.str();
}
