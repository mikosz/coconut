#include "DirectXError.hpp"

#include <sstream>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

DirectXError::DirectXError(HRESULT errorCode, const std::string& message) :
	std::runtime_error(buildMessage(errorCode, message)),
	errorCode_(errorCode)
{
}

std::string DirectXError::buildMessage(HRESULT errorCode, const std::string& message) {
	std::ostringstream oss;
	oss << "DirectX error occurred. Error code: " << errorCode << ". Message: " << message;
	return oss.str();
}
