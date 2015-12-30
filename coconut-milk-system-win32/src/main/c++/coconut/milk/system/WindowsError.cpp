#include "WindowsError.hpp"

#include <sstream>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::system;

WindowsError::WindowsError(HRESULT errorCode, const std::string& message) :
	coconut_tools::exceptions::RuntimeError(buildMessage(errorCode, message)),
	errorCode_(errorCode)
{
}

std::string WindowsError::buildMessage(HRESULT errorCode, const std::string& message) {
	std::ostringstream oss;
	oss << message << " Windows error occurred. Error code: " << errorCode << " (0x" << std::hex << errorCode << ").";

	LPSTR messageBuffer = nullptr; // TODO: to mi sie nie podoba - raz traktujemy errorCode jako HRESULT, innym razem jako wynik ::GetLastError
	const auto size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer),
		0,
		nullptr
		);

	oss << " System message: " << messageBuffer;

	LocalFree(messageBuffer); // TODO: RAII

	return oss.str();
}
