#ifndef _COCONUT_MILK_GRAPHICS_DIRECTXERROR_HPP_
#define _COCONUT_MILK_GRAPHICS_DIRECTXERROR_HPP_

#include <string>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace coconut {
namespace milk {
namespace graphics {

class DirectXError : public std::runtime_error {
public:

	DirectXError(HRESULT errorCode, const std::string& message);

	HRESULT errorCode() const {
		return errorCode_;
	}

private:

	HRESULT errorCode_;

	static std::string buildMessage(HRESULT errorCode, const std::string& message);

};

inline void checkDirectXCall(HRESULT result, const std::string& errorMessage) {
	if (FAILED(result)) {
		throw DirectXError(result, errorMessage);
	}
}

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DIRECTXERROR_HPP_ */
