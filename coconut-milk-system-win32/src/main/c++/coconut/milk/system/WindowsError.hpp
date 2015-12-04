#ifndef _COCONUT_MILK_SYSTEM_WINDOWSERROR_HPP_
#define _COCONUT_MILK_SYSTEM_WINDOWSERROR_HPP_

#include <string>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <coconut-tools/exceptions/RuntimeError.hpp>

namespace coconut {
namespace milk {
namespace system {

class WindowsError : public coconut_tools::exceptions::RuntimeError {
public:

	WindowsError(HRESULT errorCode, const std::string& message);

	const std::string& name() const noexcept override {
		static const std::string NAME = "WindowsError";
		return NAME;
	}

	HRESULT errorCode() const {
		return errorCode_;
	}

private:

	HRESULT errorCode_;

	static std::string buildMessage(HRESULT errorCode, const std::string& message);

};

inline void checkWindowsCall(HRESULT result, const std::string& errorMessage) {
	if (FAILED(result)) {
		throw WindowsError(result, errorMessage);
	}
}

} // namespace system
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_SYSTEM_WINDOWSERROR_HPP_ */
