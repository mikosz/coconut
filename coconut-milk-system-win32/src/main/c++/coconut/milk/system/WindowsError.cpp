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
	oss << "Windows error occurred. Error code: " << errorCode << ". Message: " << message;
	return oss.str();
}
