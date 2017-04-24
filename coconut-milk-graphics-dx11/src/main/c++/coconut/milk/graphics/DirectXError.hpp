#ifndef _COCONUT_MILK_GRAPHICS_DIRECTXERROR_HPP_
#define _COCONUT_MILK_GRAPHICS_DIRECTXERROR_HPP_

#include <string>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/exceptions/RuntimeError.hpp>

namespace coconut {
namespace milk {
namespace graphics {

class DirectXError : public coconut_tools::exceptions::RuntimeError {
public:

	DirectXError(HRESULT errorCode, const std::string& message);

	const std::string& name() const noexcept override {
		static const std::string NAME = "DirectXError";
		return NAME;
	}

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
